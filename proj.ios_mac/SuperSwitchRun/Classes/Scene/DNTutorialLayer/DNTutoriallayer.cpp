//
//  DNTutoriallayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 14/08/17.
//
//

#include "DNTutoriallayer.hpp"
#include "cocos2d.h"
#include "DNConstants.h"
#include "DNDataManager.h"

#pragma -mark Constructor
DNTutoriallayer::DNTutoriallayer() {
    _fadeLayer = NULL;
    DNDataManager::sharedManager()->_tutorialRef = this;
}

#pragma -mark Destructor
DNTutoriallayer::~DNTutoriallayer() {
  
}

#pragma -mark createTutorialLayer
DNTutoriallayer* DNTutoriallayer::createTutorialLayer()
{
    DNTutoriallayer* scene = new DNTutoriallayer();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNTutoriallayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}


#pragma -mark fadeInBackground
void DNTutoriallayer::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer = NULL;
    }
    _fadeLayer = cocos2d::LayerColor::create(cocos2d::Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    //_fadeLayer->retain();
    _fadeLayer->runAction(cocos2d::FadeTo::create(0.5f ,FADE_LAYER_OPACITY));
    this->addChild(_fadeLayer );
}

#pragma -mark fadeOutbackground
void DNTutoriallayer::fadeOutbackground() {
    if (_fadeLayer) {
        _fadeLayer->runAction(cocos2d::FadeTo::create(0.5f ,0));

    }
}

#pragma -mark removeFadeLayerFromParent
void DNTutoriallayer::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        CC_SAFE_RELEASE_NULL(_fadeLayer);
    }
}

#pragma -mark tutorialForCaharacterColor
void::DNTutoriallayer  :: tutorialForCaharacterColor()
{
    tutorialArrow = cocos2d::Sprite::create("Tutorial/Pointer.png");
    tutorialArrow->setPosition(Vec2( DNDataManager::sharedManager()->_HUDLayerRef->_blueButton->getPositionX(), DNDataManager::sharedManager()->_HUDLayerRef->_blueButton->getPositionY() -tutorialArrow->getContentSize().height * 0.275 ));
    tutorialArrow->setAnchorPoint(Vec2(0.5,0));
    this->addChild(tutorialArrow);
    
    tutorialBox = cocos2d::Sprite::create("Tutorial/Box.png");
    tutorialBox->setPosition(Vec2( tutorialArrow->getPositionX() - tutorialBox->getContentSize().width/2 + tutorialArrow->getContentSize().width *0.25 ,tutorialArrow->getPositionY() + tutorialArrow->getContentSize().height));
    tutorialBox->setAnchorPoint(Vec2(0.5,0));
    this->addChild(tutorialBox);
    
    _tutorialLabel  = cocos2d::Label::createWithTTF("PRESS BLUE BUTTON \nTO SWITCH THE COLOR AND \nPASS THROUGH THE LIGHT.","fonts/motschcc.ttf", 20);
    _tutorialLabel->setColor(Color3B::BLACK);
    _tutorialLabel->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.50));
    tutorialBox->addChild(_tutorialLabel);
    
    DNDataManager::sharedManager()->isButtonTutorial = true;
}

#pragma -mark tutorialForGreenButtonIntrduce
void::DNTutoriallayer  :: tutorialForGreenButtonIntrduce()
{
    
    
    tutorialArrow = cocos2d::Sprite::create("Tutorial/Pointer.png");
    tutorialArrow->setPosition(Vec2( DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->getPositionX(), DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->getPositionY() -tutorialArrow->getContentSize().height * 0.275 ));
    tutorialArrow->setAnchorPoint(Vec2(0.5,0));
    this->addChild(tutorialArrow);
    
    tutorialBox = cocos2d::Sprite::create("Tutorial/Box.png");
    tutorialBox->setPosition(Vec2( tutorialArrow->getPositionX() - tutorialBox->getContentSize().width/2 + tutorialArrow->getContentSize().width *0.25 ,tutorialArrow->getPositionY() + tutorialArrow->getContentSize().height));
    tutorialBox->setAnchorPoint(Vec2(0.5,0));
    this->addChild(tutorialBox);
    
    _tutorialLabel  = cocos2d::Label::createWithTTF("A new challenge!","fonts/motschcc.ttf", 20);
    _tutorialLabel->setColor(Color3B::BLACK);
    _tutorialLabel->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.70));
    tutorialBox->addChild(_tutorialLabel);
    
    cocos2d::Label *_tutorialLabel1  = cocos2d::Label::createWithTTF("Now you gotta take control of three colours...","fonts/motschcc.ttf", 19);
    _tutorialLabel1->setColor(Color3B::BLACK);
    _tutorialLabel1->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.50));
    tutorialBox->addChild(_tutorialLabel1);
    
    cocos2d::Label *_tutorialLabel2  = cocos2d::Label::createWithTTF("Good luck!.","fonts/motschcc.ttf", 20);
    _tutorialLabel2->setColor(Color3B::BLACK);
    _tutorialLabel2->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.30));
    tutorialBox->addChild(_tutorialLabel2);
    
    DNDataManager::sharedManager()->isButtonTutorial = true;
}

#pragma -mark tutorialForControl
void DNTutoriallayer::tutorialForControl()
{
    
    tutorialArrow->removeFromParentAndCleanup(true);
    tutorialBox->removeFromParentAndCleanup(true);
    DNDataManager::sharedManager()->_gameLayerRef->scheduleUpdate();
  
    //    DNDataManager::sharedManager()->_gameLayerRef->schedule(schedule_selector(DNGameSceneLayer::fixedUpdate),GAME_DT_VALUE);
    
    //    tutorialArrow = cocos2d::Sprite::create("Tutorial/Pointer.png");
    //    tutorialArrow->setPosition(Vec2(DNDataManager::sharedManager()->_HUDLayerRef->_buttonToggle->getPositionX() - DNDataManager::sharedManager()->_HUDLayerRef->_buttonToggle->getContentSize().width * 1.5, DNDataManager::sharedManager()->_HUDLayerRef->_buttonToggle->getPositionY()));
    //    tutorialArrow->setRotation(90);
    //    tutorialArrow->setAnchorPoint(Vec2(0.5,0));
    //    this->addChild(tutorialArrow);
    //
    //    tutorialBox = cocos2d::Sprite::create("Tutorial/Box.png");
    //    tutorialBox->setPosition(Vec2( tutorialArrow->getPositionX() + tutorialArrow->getContentSize().width +tutorialBox->getContentSize().width * 0.725 ,tutorialArrow->getPositionY() - 20 ));
    //    tutorialBox->setAnchorPoint(Vec2(0.5,0));
    //    this->addChild(tutorialBox);
    //
    //    _tutorialLabel  = cocos2d::Label::createWithTTF("PRESS THIS BUTTON \n TO SWITCH THE ARRANGEMENTS  \n OF THE CONTROL","fonts/motschcc.ttf", 20);
    //    _tutorialLabel->setColor(Color3B::BLACK);
    //    _tutorialLabel->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.50));
    //    tutorialBox->addChild(_tutorialLabel);
    
    //  DNDataManager::sharedManager()->isJoysticTutorial = true;
    
}

#pragma -mark tutorialForControl
void DNTutoriallayer::tutorialForMashroom()
{
    tutorialArrow = cocos2d::Sprite::create("Tutorial/Pointer.png");
    tutorialArrow->setPosition(Vec2(DNDataManager::sharedManager()->_gameLayerRef->_mashroom->getPositionX(),DNDataManager::sharedManager()->_gameLayerRef->_mashroom->getPositionY()));
    tutorialArrow->setAnchorPoint(Vec2(0.5,0.25));
    this->addChild(tutorialArrow);
    
    tutorialBox = cocos2d::Sprite::create("Tutorial/Box.png");
    tutorialBox->setPosition(Vec2( tutorialArrow->getPositionX()  + tutorialArrow->getContentSize().width,tutorialArrow->getPositionY() + tutorialArrow->getContentSize().height * 0.75));
    tutorialBox->setAnchorPoint(Vec2(0.5,0));
    this->addChild(tutorialBox);
    
    _tutorialLabel  = cocos2d::Label::createWithTTF("COLLECT MUSHROOM \nTO PASS THROUGH A LIGHT \nWITHOUT CHANGING COLOR. \nTAP ANYWHERE TO CONTINUE","fonts/motschcc.ttf", 20);
    _tutorialLabel->setColor(Color3B::BLACK);
    _tutorialLabel->setPosition(cocos2d::Vec2( tutorialBox->getContentSize().width/2,tutorialBox->getContentSize().height * 0.50));
    tutorialBox->addChild(_tutorialLabel);
    
    
}



