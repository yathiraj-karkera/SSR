//
//  DNGameScene.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//



#include "DNGameScene.hpp"
#include "DNBackGroundLayer.hpp"
#include "DNHUDLayer.hpp"
#include "DNGameSceneLayer.hpp"
#include "DNDataManager.h"
#include "DNMainMenuLayer.hpp"
#include "DNConstants.h"
#include "DNPauseMenu.hpp"
#include "cocos2d.h"
#include   "DNGameUtility.hpp"



USING_NS_CC;

#pragma -mark Constructor
DNGameScene::DNGameScene() {
}

#pragma -mark Destructor
DNGameScene::~DNGameScene() {
    
}


#pragma -mark CreateScene
Scene* DNGameScene::createScene()
{
    
    return DNGameScene::create();
}

// on "init" you need to initialize your instance
#pragma -mark Initialization
bool DNGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    createBackgroundLayer();
    createHUDLayer();
    hideHUDLayer();
    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(false);
    createGameSceneLayer();
    
    return true;
}

#pragma -mark enablePauseMenu
void  DNGameScene::enablePauseMenu()
{
    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
}

#pragma -mark showHUDLayer
void  DNGameScene::showHUDLayer()
{
    DNDataManager::sharedManager()->_HUDLayerRef->fadeOutbackground();
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setEnabled(true);
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setVisible(true);
    DNDataManager::sharedManager()->_HUDLayerRef->score->setVisible(true);
    DNDataManager::sharedManager()->_HUDLayerRef->menu1->setEnabled(true);
    DNDataManager::sharedManager()->_HUDLayerRef->menu1->setVisible(true);
    DNDataManager::sharedManager()->_HUDLayerRef->levelBar->setVisible(true);
    
}

#pragma -mark hideHUDLayer
void DNGameScene::hideHUDLayer()
{
    DNDataManager::sharedManager()->_HUDLayerRef->fadeInBackground();
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setEnabled(false);
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setVisible(false);
    DNDataManager::sharedManager()->_HUDLayerRef->score->setVisible(false);
    DNDataManager::sharedManager()->_HUDLayerRef->menu1->setEnabled(false);
    DNDataManager::sharedManager()->_HUDLayerRef->menu1->setVisible(false);
    DNDataManager::sharedManager()->_HUDLayerRef->levelBar->setVisible(false);
}

#pragma -mark loadGame
void DNGameScene::loadGame()
{
    resumeGame();
}

#pragma -mark createBackgroundLayer
void DNGameScene::createBackgroundLayer()
{
    _background  = DNBackGroundLayer::createBackground();
    this->addChild(_background,1);
}

#pragma -mark createGameSceneLayer
void DNGameScene::createGameSceneLayer()
{
    _gameLayer  = DNGameSceneLayer::createGameLayer();
    this->addChild(_gameLayer,3);
}

#pragma -mark createHUDLayer
void DNGameScene::createHUDLayer()
{
    _HUDLayer  = DNHUDLayer::createHUD();
    this->addChild(_HUDLayer,10000);
}

#pragma -mark pauseGame
void DNGameScene::pauseGame()
{
    DNDataManager::sharedManager()->_gameLayerRef->unscheduleUpdate();
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setEnabled(false);
}



#pragma -mark resumeGame
void DNGameScene::resumeGame()
{
   //Analytics
    //DNDataManager::sharedManager()->callAnalytics("Resume_button", "Gameplay_Resume", "Unlocks",1);
    
    
    DNDataManager::sharedManager()->_gameLayerRef->scheduleUpdate();
    
    DNDataManager::sharedManager()->_HUDLayerRef->_menu->setEnabled(true);
    
    //setting running animation
    DNDataManager::sharedManager()->_gameLayerRef->setRunAnimation();
}




