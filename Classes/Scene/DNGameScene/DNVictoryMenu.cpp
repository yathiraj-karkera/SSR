//
//  DNVictoryMenu.cpp
//  DesiNinja
//
//  Created by Yathiraj on 21/08/17.
//
//

#include "DNVictoryMenu.hpp"
#include "DNHUDLayer.hpp"
#include "DNDataManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "PluginFacebook/PluginFacebook.h"
#include "PluginShare/PluginShare.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "DNGameUtility.hpp"
#include "DNGameSceneLayer.hpp"
#include "DNAcheivementManager.hpp"
//#include "PluginFirebase/PluginFirebase.h"

#pragma -mark Constructor
DNVictoryMenu::DNVictoryMenu() {
    
    DNDataManager::sharedManager()->_isinGameScene = false;
    
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(DNVictoryMenu::onExitButtonPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

}

#pragma -mark Destructor
DNVictoryMenu::~DNVictoryMenu() {
    sdkbox::PluginFacebook::removeListener();
}

#pragma -mark createBackground
DNVictoryMenu* DNVictoryMenu::createVictoryMenu()
{
    DNVictoryMenu* scene = new DNVictoryMenu();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNVictoryMenu::init() {
    if (!Layer::init()) {
        return false;
    }
    sdkbox::PluginFacebook::setListener(this);
    
    DNVictoryMenu::send_score(this);
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    Sprite *companyLogo = cocos2d::Sprite::create("Victoryscreen/CompanyLogo.png");
    companyLogo->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - companyLogo->getContentSize().width/2,origin.y + visibleSize.height - companyLogo->getContentSize().height/2 ));
    addChild(companyLogo,1000);

    _background = cocos2d::Sprite::create("PauseScreen/Popup.png");
    _background->setPosition(cocos2d::Vec2(visibleSize.width/2 + origin.x,visibleSize.height/2 + origin.y));
    addChild(_background,1000);
    _background->setScale(0);
    
    //High score Label Button
    cocos2d::Sprite* scoreSprite= cocos2d::Sprite::create("Victoryscreen/BtnBrownOC.png");
    scoreSprite->setTag(1);
    scoreSprite->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.80 ));
    _background->addChild(scoreSprite);
    
    
    
    cocos2d::Label* scoreLabel = cocos2d::Label::createWithTTF("SCORE : " + StringUtils::toString(DNDataManager::sharedManager()->_score),"fonts/motschcc.ttf", 30);
    scoreLabel->setAnchorPoint(Vec2(0,0.5));
    scoreLabel->setPosition(cocos2d::Vec2( scoreSprite->getContentSize().width * 0.30,scoreSprite->getContentSize().height /2));
    scoreSprite->addChild(scoreLabel,1000);
    
    
    
    
    if(DNDataManager::sharedManager()->_score >= UserDefault::getInstance()->getIntegerForKey("HighScore",0))
    {
        
        facebookFriendPostScore();
        
        scoreLabel->setPosition(cocos2d::Vec2( scoreSprite->getContentSize().width * 0.20,scoreSprite->getContentSize().height /2));
        cocos2d::Sprite* highScore = cocos2d::Sprite::create("test/NewHighScore.png");
        highScore->setScale(1.2);
        
        highScore->setPosition(cocos2d::Vec2(scoreSprite->getContentSize().width - highScore->getContentSize().width * 1.1 ,scoreSprite->getContentSize().height / 2));
        scoreSprite->addChild(highScore,1000);
        UserDefault::getInstance()->setIntegerForKey("HighScore", DNDataManager::sharedManager()->_score);
        
        //for high score send
        {
            //Analytics
            std::map<std::string, std::string> params;
            params["Parameter"] = "High_Score";
            params["Action"] = "ScoreSend";
            params["HighScore"]= StringUtils::toString (DNDataManager::sharedManager()->_score);
            //params[sdkbox::Firebase::Analytics::kFIRParameterValue] = UserDefault::getInstance()->getIntegerForKey("HighScore",0);
//            params[sdkbox::Firebase::Analytics::kFIRParameterScore] =  (int)DNDataManager::sharedManager()->_score;
            if(DNGameUtility::checkIsNetworkAvailable())
            {
//                sdkbox::Firebase::Analytics::logEvent("GamePlayHighScore", params);
            }
        }
    }
    
    myScoreForSharing =DNDataManager::sharedManager()->_score;
    
    
    //High score Label Button
    cocos2d::Sprite* highScoreSprite1= cocos2d::Sprite::create("Victoryscreen/BtnBrownOC.png");
    highScoreSprite1->setTag(2);
    highScoreSprite1->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.60 ));
    _background->addChild(highScoreSprite1);
    
    
    
    cocos2d::Label* highScoreLabel = cocos2d::Label::createWithTTF("BEST SCORE : " + StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("HighScore",0)),"fonts/motschcc.ttf", 30);
    highScoreLabel->setAnchorPoint(Vec2(0,0.5));
    highScoreLabel->setPosition(cocos2d::Vec2( highScoreSprite1->getContentSize().width * 0.20,highScoreSprite1->getContentSize().height /2));
    highScoreSprite1->addChild(highScoreLabel,1000);
    
    
    
    DNDataManager::sharedManager()->_score = 0;
    DNDataManager::sharedManager()->_HUDLayerRef->score->setString("0");
    //
    //Menu Button
    cocos2d::Sprite* menuButton= cocos2d::Sprite::create("PlayReplaySettings/Btn1OC_2.png");
    menuButton->setTag(2);
    menuButton->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.40 ));
    _background->addChild(menuButton);
    
    
    //Facebook
    
    Sprite* normalhomeButtonIcon = Sprite::create("test/Home.png");
    Sprite* selectedhomeButtonIcon = Sprite::create("test/Home.png");
    
    MenuItemSprite* homeButtonIcon = cocos2d::MenuItemSprite::create(normalhomeButtonIcon, selectedhomeButtonIcon,CC_CALLBACK_0(DNVictoryMenu::menuButtonTapped,this));
    homeButtonIcon->setPosition(cocos2d::Vec2(menuButton->getContentSize().width * 0.33, menuButton->getContentSize().height/2 ));
    
    Sprite* normalreplayButtonIcon = Sprite::create("test/Replay.png");
    Sprite* selectedreplayButtonIcon = Sprite::create("test/Replay.png");
    
    MenuItemSprite* replayButtonIcon = cocos2d::MenuItemSprite::create(normalreplayButtonIcon, selectedreplayButtonIcon,CC_CALLBACK_0(::DNVictoryMenu::replayButtonTapped,this));
    replayButtonIcon->setPosition(cocos2d::Vec2(menuButton->getContentSize().width * 0.66, menuButton->getContentSize().height/2 ));
    
    cocos2d::Menu* menu = cocos2d::Menu::create(homeButtonIcon,replayButtonIcon,NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    menu->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    menuButton->addChild(menu, 1);
    
    
    //Scocial Network Tab
    
    cocos2d::Sprite* scocialNetworkSprite = cocos2d::Sprite::create("PlayReplaySettings/Btn1OC_2.png");
    scocialNetworkSprite->setTag(1);
    scocialNetworkSprite->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.20 ));
    _background->addChild(scocialNetworkSprite);
    
    //Facebook Button
    Sprite* normalfacebookButton = Sprite::create("test/Facebook.png");
    Sprite* selectedfacebookButton = Sprite::create("test/Facebook.png");
    
    cocos2d:: MenuItemSprite* facebookButton = cocos2d:: MenuItemSprite::create(normalfacebookButton, selectedfacebookButton, CC_CALLBACK_0(DNVictoryMenu::facebookShareScoreOrScreenShot, this));
    facebookButton->setTag(3);
    facebookButton->setPosition(cocos2d::Vec2(scocialNetworkSprite->getContentSize().width * 0.33,scocialNetworkSprite->getContentSize().height / 2 ));
    
    //twitter Button
    Sprite* normaltwitterButton = Sprite::create("test/Twitter.png");
    Sprite* selectedtwitterButton = Sprite::create("test/Twitter.png");
    
    cocos2d::MenuItemSprite* twitterButton= cocos2d::MenuItemSprite::create(normaltwitterButton, selectedtwitterButton, CC_CALLBACK_0(DNVictoryMenu::screenshotForTwitter, this));
    twitterButton->setTag(3);
    twitterButton->setPosition(cocos2d::Vec2(scocialNetworkSprite->getContentSize().width * 0.50,scocialNetworkSprite->getContentSize().height / 2 ));
    
    
    cocos2d::Menu* menu1 = cocos2d::Menu::create(/*facebookButton,*/twitterButton,NULL);
    menu1->setPosition(cocos2d::Vec2::ZERO);
    menu1->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    scocialNetworkSprite->addChild(menu1, 1);
    
    return true;
    
}

void DNVictoryMenu::facebookFriendPostScore()
{
    if(DNGameUtility::checkIsNetworkAvailable())
    {
        if(sdkbox::PluginFacebook::isLoggedIn())
        {
            {
                sdkbox::FBAPIParam params;
                params.insert(std::pair<std::string, std::string>("score", StringUtils::toString(DNDataManager::sharedManager()->_score).c_str()));
                sdkbox::PluginFacebook::api("/me/scores", "POST", params, "UserScore");
            }
        }
    }
}

void DNVictoryMenu::send_score(cocos2d::Ref *sender) {
    sdkbox::PluginSdkboxPlay::submitScore("ldb1", DNDataManager::sharedManager()->_score);
}


void DNVictoryMenu::screenshotForTwitter()
{
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Twitter_share", "Clicked", "SocialFeatures",1);
    
    if(!DNGameUtility::checkIsNetworkAvailable())
    {
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();
        return;
    }
    else
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sdkbox::SocialShareInfo info;
        info.image = _captureFilename1;
        info.link = "http://onelink.to/9e6m82";
        info.text = "Guess what !! I scored " + StringUtils::toString(myScoreForSharing) +" in Super Switch Run, Its your move now." ;
        info.showDialog = true; //if you want share with dialog，set the value true
        info.platform = sdkbox::SocialPlatform::Platform_Native;
        sdkbox::PluginShare::nativeShare(info);
#endif
        
       
 
        
#if (CC_TARGET_PLATFORM == TARGET_OS_IOS)
        
        utils::captureScreen(CC_CALLBACK_2(DNVictoryMenu::afterCaptureScreen1, this), "screenshot.png");
#endif
        // sdkbox::PluginShare::init();
        //==============utils::captureScreen(CC_CALLBACK_2(DNVictoryMenu::afterCaptureScreen1, this), "screenshot.png");
        
        //for Achiement
        int val = UserDefault::getInstance()->getIntegerForKey("scoreShare", 0);
        
        switch (val) {
            case 100:
                //unlock achiement for 100
                DNAcheivementManager::sharedManager()->AcheivementXp(30,"Societal");
               // sdkbox::PluginSdkboxPlay::unlockAchievement("Societal");
                break;
            case 500:
                //unlock achiement for 500
                DNAcheivementManager::sharedManager()->AcheivementXp(60,"Social_Butterfly");
               // sdkbox::PluginSdkboxPlay::unlockAchievement("Social_Butterfly");
                break;
                
            default:
                break;
        }

        UserDefault::getInstance()->setIntegerForKey("scoreShare", val+1);
        
    }
}

void DNVictoryMenu::afterCaptureScreen1(bool yes, const std::string &outputFilename1) {
    if (yes) {
        //CCLOG("SharePlugin1 capture screen success: %s", outputFilename1.c_str());
        _captureFilename1 = outputFilename1;
        // captured = true;
    }
    DNVictoryMenu::twittershareScreenshot();
}

void DNVictoryMenu::twittershareScreenshot()
{
    
    if(DNGameUtility::checkIsNetworkAvailable())
    {
 

       sdkbox::SocialShareInfo info;
    info.image = _captureFilename1;
    info.link = "http://onelink.to/9e6m82";
    info.text = "Guess what !! I scored "+ StringUtils::toString(myScoreForSharing) +" in Super Switch Run, Its your move now." ;
    info.showDialog = true; //if you want share with dialog，set the value true
        info.platform = sdkbox::SocialPlatform::Platform_Native;
    sdkbox::PluginShare::nativeShare(info);
    
    }
    
}





void DNVictoryMenu::facebookShareScoreOrScreenShot()
{
    
    
    if(!DNGameUtility::checkIsNetworkAvailable())
    {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();
        
        return;
    }
    else
    {
        //std::string path = "screenshot.png";
        if(sdkbox::PluginFacebook::isLoggedIn())
        {
            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Facebook_share", "Clicked", "SocialFeatures",1);
            
            utils::captureScreen(CC_CALLBACK_2(DNVictoryMenu::afterCaptureScreen, this), "screenshot.png");
        }
        else
        {
            sdkbox::PluginFacebook::login();
        }
        
    }
}
void DNVictoryMenu::afterCaptureScreen(bool yes, const std::string &outputFilename) {
    if (yes) {
        //CCLOG("SharePlugin capture screen success: %s", outputFilename.c_str());
        _captureFilename = outputFilename;
        // captured = true;
    }
    DNVictoryMenu::facebookDialog();
}

void DNVictoryMenu::facebookDialog()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
   // sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_PHOTO;
    info.title = "My Photo";
    info.image = _captureFilename;
    sdkbox::PluginFacebook::dialog(info);
    
#endif
    
    
#if (CC_TARGET_PLATFORM == TARGET_OS_IOS)
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_PHOTO;
    info.title = "My Photo";
    info.image = _captureFilename;
    sdkbox::PluginFacebook::dialog(info);
    
#endif
    
}

void DNVictoryMenu::showVictoryMenu()
{
    _background->runAction(ScaleTo::create(0.30, 1));
    _background->runAction(RotateBy::create(0.30, 360));
    if(UserDefault::getInstance()->getIntegerForKey("deathCount") % 3 == 0 && UserDefault::getInstance()->getIntegerForKey("deathCount") > 0)
    {
        //CCLOG("Death =============");
        DNDataManager::sharedManager()->_isFromGameOver = false;
        if(DNGameUtility::checkIsNetworkAvailable())
        {
            if(!(UserDefault::getInstance()->getBoolForKey("isnoAdPackPurchased",false)))
            {
                sdkbox::PluginAdMob::show("gameover");
            }

        }
    
    }
    
}
void DNVictoryMenu::hideVictoryMenu()
{
    
    _background->runAction(ScaleTo::create(0.30, 0));
    _background->runAction(RotateBy::create(0.30, 360));
}




void DNVictoryMenu:: menuButtonTapped()
{
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Home_button", "Clicked", "Game_Over",1);
    
   DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->_isReplayButtonTapped = false;
    DNDataManager::sharedManager()->_gameLayerRef->removeVictoryScren();
    
}

void DNVictoryMenu::replayButtonTapped()
{
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Replay_button", "Clicked", "Game_Over",1);
    
    UserDefault::getInstance()->setIntegerForKey("RetryCount",(UserDefault::getInstance()->getIntegerForKey("RetryCount",0)+1));
    
   DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->_isReplayButtonTapped = true;
    DNDataManager::sharedManager()->_gameLayerRef->removeVictoryScren();
}


//MARK: FaceBook
void DNVictoryMenu::onLogin(bool isLogin, const std::string& error)
{
    //CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
    //CCLOG("onLogin Access token %s ***", sdkbox::PluginFacebook::getAccessToken().c_str());
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();
}



void DNVictoryMenu::onAPI(const std::string& tag, const std::string& jsonData)
{
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();
    //CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
    
}

void DNVictoryMenu::onSharedSuccess(const std::string& message)
{
    //CCLOG("##FB onSharedSuccess:%s", message.c_str());
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();
    sdkbox::PluginSdkboxPlay::unlockAchievement("Societal");
    //    MessageBox(message.c_str(), "share success");
}

void DNVictoryMenu::onSharedFailed(const std::string& message)
{
    //CCLOG("##FB onSharedFailed:%s", message.c_str());
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();
    //    MessageBox(message.c_str(), "share failed");
}



void DNVictoryMenu::onSharedCancel()
{
    //CCLOG("##FB onSharedCancel");
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();
    //    MessageBox("", "share cancel");
}


void DNVictoryMenu::onPermission(bool isLogin, const std::string& error)
{
    //CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_PHOTO;
    info.title = "Check out my score!!!";
    info.image = _captureFilename;
    sdkbox::PluginFacebook::dialog(info);
    
}

void DNVictoryMenu::onInviteFriendsResult( bool result, const std::string& msg )
{
    //CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
}

void DNVictoryMenu::onFetchFriends(bool ok, const std::string& msg)
{
    //////CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
}


void DNVictoryMenu::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
{
   }

void DNVictoryMenu::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    ////CCLOG("on invite friends with invite ids %s= '%s'", result?"ok":"error", msg.c_str());
}


void DNVictoryMenu::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
}


#pragma mark - onExitButtonPressed
void DNVictoryMenu::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
            menuButtonTapped();
        
    }
}

