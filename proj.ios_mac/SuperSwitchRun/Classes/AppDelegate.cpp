#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "Scene/DNGameScene/DNGameScene.hpp"
#include "DNSplashScreen/DNSplashScreen.hpp"
#include "DNDataManager/DNDataManager.h"
#include "PluginFacebook/PluginFacebook.h"
#include "PluginShare/PluginShare.h"
#include "PluginIAP/PluginIAP.h"
#include "PluginAdMob/PluginAdMob.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "DNAcheivementManager/DNAcheivementManager.hpp"
//#include "PluginFirebase/PluginFirebase.h"
#include "LVJniCommunicator.hpp"
#include "DNAcheivementManager.hpp"

#include "cocos2d.h"
USING_NS_CC;

static cocos2d::Size workingArea;
static float scaleFactor;
typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;

using namespace std;
USING_NS_CC;


static cocos2d::Size designResolutionSize;


#pragma mark - Constructor
AppDelegate::AppDelegate()
{

}

#pragma mark - Destructor
AppDelegate::~AppDelegate()
{

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
#pragma mark - InitGlContextAttrs
void AppDelegate::initGLContextAttrs()
{


    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
#pragma mark - register_all_packages
static int register_all_packages()
{
    return 0; //flag for packages manager
}

#pragma mark - applicationDidFinishLaunching

bool AppDelegate::applicationDidFinishLaunching()
{
    sdkbox::PluginShare::init();
    sdkbox::PluginFacebook::init();
    sdkbox::PluginAdMob::init();
    sdkbox::IAP::init();
    sdkbox::PluginSdkboxPlay::init();
//    sdkbox::Firebase::Analytics::init();

    this->onGameLaunchCheckForTime();

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 /60);


    designResolutionSize = cocos2d::Size(1212, 768);
    Resource mediumResource =  { cocos2d::Size(1212, 768),  "Normal" };
    // Resource largeResource  =  { cocos2d::Size(2424,1536), "HD" };
    Resource largeResource  =  { cocos2d::Size(1212,768), "Normal" };

    Size frameSize = glview->getFrameSize();


    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);



    vector<std::string> searchPath;

    float genericDeviceWidth = (frameSize.width > frameSize.height) ? frameSize.width : frameSize.height;
    float genericDeviceHeight = (frameSize.width > frameSize.height) ? frameSize.height : frameSize.width;

    if (genericDeviceWidth > designResolutionSize.width || genericDeviceHeight > designResolutionSize.height)
    {
        scaleFactor = largeResource.size.height/designResolutionSize.height;
        searchPath.push_back(largeResource.directory);

        director->setContentScaleFactor(scaleFactor);

    }
    else
    {

        workingArea = Size(1024.0f, ceilf((1024.0f / 960.0f) * 640.0f));


        float scaleFactor_deviceToResources = genericDeviceWidth / mediumResource.size.width;

        if(scaleFactor_deviceToResources * mediumResource.size.height < genericDeviceHeight)
        {
            scaleFactor_deviceToResources = genericDeviceHeight / mediumResource.size.height;
        }

        float scaleFactor_workingAreaToDevice = (scaleFactor_deviceToResources * workingArea.height) / genericDeviceHeight;

        if((scaleFactor_workingAreaToDevice * genericDeviceWidth) < (scaleFactor_deviceToResources * workingArea.width))
        {
            scaleFactor_workingAreaToDevice = (scaleFactor_deviceToResources * workingArea.width) / genericDeviceWidth;
        }

        scaleFactor = scaleFactor_workingAreaToDevice;
        searchPath.push_back(mediumResource.directory);
        director->setContentScaleFactor(scaleFactor);

    }

    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);

    DNDataManager::sharedManager()->setTutorialPlayed(UserDefault::getInstance()->getBoolForKey("TutorialPlayed", false)); //Automatically save to userDefault

    DNDataManager::sharedManager()->setMashroomTutorialPlayed(UserDefault::getInstance()->getBoolForKey("MashroomTutorial", false)); //Automatically save to userDefault

    UserDefault::getInstance()->setBoolForKey("userDialogue", false);


    //timeMili = clock();

    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(UserDefault::getInstance()->getFloatForKey("volume", 0.5) );
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("effect", 0.5));


    // create a scene. it's an autorelease object
    Scene *scene = DNSplashScreen::createScene();


    // run
    director->runWithScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    // LVJniCommunicator::sharedManager()->GooglSignin();
#endif

    // sdkbox::PluginSdkboxPlay::signin();
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

    UserDefault::getInstance()->setIntegerForKey("SessionEndTime", clock());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    int secondsForDailyReward = getTimeForDailyReward();

    //   LVJniCommunicator::sharedManager()->fireLocalNotification(5, 0,"Come back! there is a lot of cleaning to do.", "SuperSwitchRun", 1);


#endif
    //  LocalNotification::show("you have pressed the pause button", 1, 1);
}




int AppDelegate::getTimeForDailyReward()
{
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int currentHour = timeinfo->tm_hour;
    int hour = 24 - currentHour + 7;
    int secondsRemaining = hour * 60;
    return secondsRemaining;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {

    //    time_t  timev;
    //
    //    tm * time;
    //
    //    std::time(&timev);
    //    time = localtime(&timev);
    //
    //    UserDefault::getInstance()->setIntegerForKey("SessionStartTime", //timeMili = clock(););
    //    CCLOG("d",UserDefault::getInstance()->getIntegerForKey("SessionStartTime", time->tm_sec));

    Director::getInstance()->startAnimation();
    // UserDefault::getInstance()->setBoolForKey("userDialogue", false);
    this->onGameEnteredFwdCheckForTime();


    if(UserDefault::getInstance()->getBoolForKey("shouldPauseScreenAppear",false))
    {
        if(DNDataManager::sharedManager()->_isinGameScene && DNDataManager::sharedManager()->getMashroomTutorialPlayed() && (!DNDataManager::sharedManager()->_isGreenbuttonIntroduced))
        {
            DNDataManager::sharedManager()->_HUDLayerRef->pauseButtonTapped();
        }
    }

    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();





}

#pragma mark - Daily Bonus/objectives
void AppDelegate::onGameLaunchCheckForTime()
{
    //Timer
    time_t  timev;
    tm * time;

    std::time(&timev);
    time = localtime(&timev);


    if(UserDefault::getInstance()->getBoolForKey("isGameLaunchedFirstTime", true))
    {
        UserDefault::getInstance()->setBoolForKey("isGameLaunchedFirstTime",false);
        this->updateTimerValues(time);
        UserDefault::getInstance()->setIntegerForKey("GameLaunchDay",time->tm_yday);
        UserDefault::getInstance()->setIntegerForKey("GameLaunchYear",time->tm_year);
    }
    else
    {
        this->onGameEnteredFwdCheckForTime();
    }
}


void AppDelegate::updateTimerValues(tm * time)
{
    __String day = __String::createWithFormat("%d",time->tm_mday)->getCString();
    __String month = __String::createWithFormat("%d",time->tm_mon)->getCString();
    __String year = __String::createWithFormat("%d",time->tm_year)->getCString();

    std::string date = day.getCString();
    date = date + ","+month.getCString()+","+year.getCString();

    if(UserDefault::getInstance()->getStringForKey("GameLaunchTime", "Null") == "Null")
    {
        UserDefault::getInstance()->setStringForKey("GameLaunchTime",date);
    }
    else
    {
        UserDefault::getInstance()->setStringForKey("GameLaunchTime",date);

    }

    UserDefault::getInstance()->setStringForKey("DOTypes","abc");
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveDone",false);
    UserDefault::getInstance()->setBoolForKey("dailyalphaObjectiveChecked",false);
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveAlphaComplete",false);
    UserDefault::getInstance()->setIntegerForKey("DOType1CharArrIndexVal",0);

    UserDefault::getInstance()->setIntegerForKey("DOTypeTwo",-1);
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveWallComplete",false);
    UserDefault::getInstance()->setBoolForKey("dailyWallObjectiveChecked",false);
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveWallCompleteMessageDisplayed",false);
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveAlphaCompleteMessageDisplayed",false);

    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(UserDefault::getInstance()->getFloatForKey("volume", 0.5) );
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("effect", 0.5));

}


void AppDelegate::onGameEnteredFwdCheckForTime()
{
    //Timer
    time_t  timev;

    tm * time;

    std::time(&timev);
    time = localtime(&timev);


    this->checkDailyBonusStatus(time);

    std::string text = UserDefault::getInstance()->getStringForKey("GameLaunchTime", "NULL");
    std::istringstream ss(text);
    std::string token;

    ValueVector dayArr;

    while(std::getline(ss, token, ','))
    {
        dayArr.push_back(Value(token.c_str()));
    }

    for(int i=0;i<dayArr.size();i++)
    {
        if(i==0)
        {
            std::string dayStr = dayArr.at(0).asString();
            __String *a = __String::createWithFormat("%s",dayStr.c_str());

            int day = a->intValue();
            if(day<time->tm_mday)
            {
                this->updateTimerValues(time);
                break;
            }
            else
            {
                continue;
            }
        }
        if(i==1)
        {
            std::string dayStr = dayArr.at(1).asString();

            __String *a = __String::createWithFormat("%s",dayStr.c_str());
            int month = a->intValue();

            if(month<time->tm_mon)
            {
                this->updateTimerValues(time);

                break;
            }
            else
            {
                continue;
            }
        }
        if(i==2)
        {
            std::string dayStr = dayArr.at(2).asString();

            __String *a = __String::createWithFormat("%s",dayStr.c_str());
            int year = a->intValue();

            if(year<time->tm_year)
            {
                this->updateTimerValues(time);
                break;
            }
            else
            {
                continue;
            }
        }
    }

}

void AppDelegate::checkDailyBonusStatus(tm* time)
{
    int previousDay =  UserDefault::getInstance()->getIntegerForKey("GameLaunchDay",time->tm_yday);
    int currentYear = UserDefault::getInstance()->getIntegerForKey("GameLaunchYear",time->tm_year);

    bool isBonusClaimed = UserDefault::getInstance()->getBoolForKey("isBonusClaimed",false);

    if(currentYear == time->tm_year)
    {
        if(previousDay < time->tm_yday )
        {
            if((time->tm_yday-previousDay)==1 && !isBonusClaimed)
            {


                int bonus = UserDefault::getInstance()->getIntegerForKey("BonusCount",0);
                if(bonus<15)
                {
                    bonus = bonus + 1;
                }
                else
                {
                    bonus = 15;
                }
                if(bonus == 7 )
                {
                    if(!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1))
                    {
                        UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount",0) + 1);
                    }

                    DNAcheivementManager::sharedManager()->unloackBg(1);
                }
                else if(bonus == 15)
                {
                    if(!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1))
                    {
                        UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount",0) + 1);
                    }
                    DNAcheivementManager::sharedManager()->unloackCharacter(1);
                }


                UserDefault::getInstance()->setIntegerForKey("BonusCount",bonus);
                UserDefault::getInstance()->setBoolForKey("isBonusClaimed",false);
            }
            else
            {
                log("start from begging");
                UserDefault::getInstance()->setBoolForKey("isNewDay",true);
                UserDefault::getInstance()->setIntegerForKey("BonusCount",1);

            }

            UserDefault::getInstance()->setIntegerForKey("GameLaunchDay",time->tm_yday);
        }
    }
    else
    {
        if(currentYear<time->tm_year&&(time->tm_year-currentYear)==1)
        {
            if(previousDay==365 && time->tm_yday==0 && !isBonusClaimed)
            {
                int bonus = UserDefault::getInstance()->getIntegerForKey("BonusCount",0);
                if(bonus<15)
                {
                    bonus = bonus+1;
                }
                else
                {
                    bonus = 15;
                }
                if(bonus == 7 )
                {
                    if(!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1))
                    {
                        UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount",0) + 1);
                    }

                    DNAcheivementManager::sharedManager()->unloackBg(1);
                }
                else if(bonus == 15)
                {
                    if(!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1))
                    {
                        UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount",0) + 1);
                    }
                    DNAcheivementManager::sharedManager()->unloackCharacter(1);
                }

                UserDefault::getInstance()->setIntegerForKey("BonusCount",bonus);
                UserDefault::getInstance()->setBoolForKey("isBonusClaimed",false);
            }
            else
            {
                UserDefault::getInstance()->setBoolForKey("isNewDay",true);
                UserDefault::getInstance()->setIntegerForKey("BonusCount",1);
            }
        }
        UserDefault::getInstance()->setIntegerForKey("GameLaunchYear",time->tm_year);
        UserDefault::getInstance()->setIntegerForKey("GameLaunchDay",time->tm_yday);
    }

}
