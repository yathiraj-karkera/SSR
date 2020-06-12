//
//  DNSplashScreen.cpp
//  DesiNinja
//
//  Created by Yathiraj on 28/08/17.
//
//

#include "DNSplashScreen.hpp"
#include "DNGameScene.hpp"
#include "cocos2d.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "DNDataManager.h"
#include "SimpleAudioEngine.h"
#include "LVJniCommunicator.hpp"
//#include "PluginFirebase/PluginFirebase.h"
#include "DNGameUtility.hpp"

USING_NS_CC;

#pragma -mark Constructor
DNSplashScreen::DNSplashScreen() {
    
    _reviveCount =  UserDefault::getInstance()->getIntegerForKey("PreviousCountForTotalRevive",0);
     _deathCount =  UserDefault::getInstance()->getIntegerForKey("PreviousCountFordeathCount",0);
    _wallCount =  UserDefault::getInstance()->getIntegerForKey("PreviousCountForTotalwallpassed",0);
     _powerUpCount =  UserDefault::getInstance()->getIntegerForKey("PreviousCountFor1Up",0);
    
    
}

#pragma -mark Destructor
DNSplashScreen::~DNSplashScreen() {
    
    //resetting all the analytics values of session
    UserDefault::getInstance()->setIntegerForKey("RetryCount",0);
    UserDefault::getInstance()->setIntegerForKey("AlphaCount",0);
    UserDefault::getInstance()->setIntegerForKey("AchievmentCount",0);
    

    UserDefault::getInstance()->setIntegerForKey("PreviousCountForTotalRevive",UserDefault::getInstance()->getIntegerForKey("TotalRevive",0));
    UserDefault::getInstance()->setIntegerForKey("PreviousCountFordeathCount",UserDefault::getInstance()->getIntegerForKey("deathCount",0));
    UserDefault::getInstance()->setIntegerForKey("PreviousCountForTotalwallpassed",UserDefault::getInstance()->getIntegerForKey("Totalwallpassed",0));
    UserDefault::getInstance()->setIntegerForKey("PreviousCountFor1Up",UserDefault::getInstance()->getIntegerForKey("1Up",0));
    
     UserDefault::getInstance()->setIntegerForKey("SessionStartTime", clock());
}


#pragma -mark CreateScene
Scene* DNSplashScreen::createScene()
{
    
    return DNSplashScreen::create();
}

// on "init" you need to initialize your instance
#pragma -mark Initialization
bool DNSplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    initializingUi();
    return true;
}

#pragma -mark initializingUi
void DNSplashScreen::initializingUi()
{
//    bool isFirstTimePlaying = UserDefault::getInstance()->getBoolForKey("isFirstTimePlaying", false);
//    if(isFirstTimePlaying)
//    {
//        //Analytics
//        DNDataManager::sharedManager()->callAnalytics("Times_game_is_enteredn", "Game_Open", "Main_Menu",1);
//        
//        if(UserDefault::getInstance()->getIntegerForKey("RetryCount",0) > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("RetryCount",0);
//            DNDataManager::sharedManager()->callAnalytics("Retries_in_overall_game", "Quanitity", "GamePlay",value);
//            
//        }
//        if(UserDefault::getInstance()->getIntegerForKey("TotalRevive",0) - _reviveCount > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("TotalRevive",0) - _reviveCount;
//            DNDataManager::sharedManager()->callAnalytics("Total_Revives", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        if( UserDefault::getInstance()->getIntegerForKey("deathCount",0) - _deathCount > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("deathCount",0) - _deathCount;
//            DNDataManager::sharedManager()->callAnalytics("Total_Deaths", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        if(UserDefault::getInstance()->getIntegerForKey("Totalwallpassed",0) - _wallCount > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("Totalwallpassed",0) - _wallCount;
//            DNDataManager::sharedManager()->callAnalytics("Total_Obstacles", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        if(UserDefault::getInstance()->getIntegerForKey("AchievmentCount",0) > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("AchievmentCount",0);
//            DNDataManager::sharedManager()->callAnalytics("Number_of_Achievements", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        if(UserDefault::getInstance()->getIntegerForKey("AlphaCount",0) > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("AlphaCount",0);
//            DNDataManager::sharedManager()->callAnalytics("Number_of_alphabets", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        if(UserDefault::getInstance()->getIntegerForKey("1Up",0) - _powerUpCount > 0){
//            
//            //Analytics
//            int value = UserDefault::getInstance()->getIntegerForKey("1Up",0) - _powerUpCount;
//            DNDataManager::sharedManager()->callAnalytics("Number_of_collected_mushrooms", "Quanitity", "GamePlay",value);
//            
//        }
//        
//        UserDefault::getInstance()->setBoolForKey("isFirstTimePlaying", true);
//        
//    }
//    
//        //send analytics
//        {
//            //Analytics Game Session
//            std::map<std::string, std::string> params;
//            params["Parameter"] = "Game_session";
//            params["Action"] = "TimeCount";
//            params["SessionTime"] = (int)(((((int)(UserDefault::getInstance()->getIntegerForKey("SessionEndTime", clock()) - UserDefault::getInstance()->getIntegerForKey("SessionStartTime", clock())))/1000)/60)/60);
//            params[sdkbox::Firebase::Analytics::kFIRParameterValue] = (int)(UserDefault::getInstance()->getIntegerForKey("SessionEndTime", clock()) - UserDefault::getInstance()->getIntegerForKey("SessionStartTime", clock())); //pass time in seconds
//            sdkbox::Firebase::Analytics::logEvent("GamePlaySession", params);
//           
//    
//            
//                    
//        }

    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    

    
    Sprite *sp = Sprite::create("Splash_Screen/GC_lndscp.png");
    this->addChild(sp);
    sp->setPosition(Vec2(origin.x + visibleSize.width/2  ,origin.y + visibleSize.height/2));
    
    /*Replacing a splash screen with home scene after some delay.*/
    Sequence *finSeq = Sequence::create(DelayTime::create(3),CallFunc::create(CC_CALLBACK_0(DNSplashScreen::replaceSceneAfterDelay,this)),NULL);
    this->runAction(finSeq);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    LVJniCommunicator::sharedManager()->GooglSignin();
#endif
    
}

//Replacing scene with Home Scene.
#pragma -mark replaceSceneAfterDelay
void DNSplashScreen::replaceSceneAfterDelay()
{
    auto myScene = DNGameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(2, myScene));
    //googleplay sign in
    if(DNGameUtility::checkIsNetworkAvailable())
    {
        sdkbox::PluginSdkboxPlay::signin();
    }
}
