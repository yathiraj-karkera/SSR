//
//  DNPauseMenu.cpp
//  DesiNinja
//
//  Created by Yathiraj on 07/08/17.
//
//

#include "DNPauseMenu.hpp"
#include "DNHUDLayer.hpp"
#include "DNDataManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
//#include "PluginFirebase/PluginFirebase.h"
#include    "DNConstants.h"
#include "DNGameUtility.hpp"


#pragma -mark Constructor
DNPauseMenu::DNPauseMenu() {
    
    //touch listeners
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2( DNPauseMenu::onTouchBegan, this);
    listener->onTouchEnded=CC_CALLBACK_2(DNPauseMenu::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    DNDataManager::sharedManager()->isInPauseMenu =true;
    DNDataManager::sharedManager()->_isinGameScene = false;
    
    //keyboard listener
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(DNPauseMenu::onExitButtonPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    _fadeLayer = NULL;
}

#pragma -mark Destructor
DNPauseMenu::~DNPauseMenu() {
    DNDataManager::sharedManager()->isInPauseMenu =false;
}

#pragma -mark createBackground
DNPauseMenu* DNPauseMenu::createPauseMenu()
{
    DNPauseMenu* scene = new DNPauseMenu();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNPauseMenu::init() {
    if (!Layer::init()) {
        return false;
    }
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    //background
    _background = cocos2d::Sprite::create("PauseScreen/Popup.png");
    _background->setPosition(cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height  * 1.2 + origin.y));
    addChild(_background,1000);
    
    //Play Button
    
    Sprite* normalplayButton = Sprite::create("PlayReplaySettings/Play_Btn.png");
    Sprite* selectedplayButton = Sprite::create("PlayReplaySettings/Play-BtnOC.png");
    
    cocos2d::MenuItemSprite* playButton= cocos2d::MenuItemSprite::create(normalplayButton, selectedplayButton,CC_CALLBACK_0(DNPauseMenu::playButtonTapped, this));
    playButton->setTag(1);
    playButton->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.60 ));
    
    cocos2d::Label* playButtonLabel = cocos2d::Label::createWithTTF("RESUME","fonts/motschcc.ttf", 30);
    playButtonLabel->setAnchorPoint(Vec2(0,0.5));
    playButtonLabel->setPosition(cocos2d::Vec2( playButton->getContentSize().width * 0.40,playButton->getContentSize().height /2));
    playButton->addChild(playButtonLabel,1000);
    
    cocos2d::Sprite* playButtonIcon = cocos2d::Sprite::create("PauseScreen/PlayIcon.png");
    playButtonIcon->setPosition(cocos2d::Vec2(playButton->getContentSize().width * 0.30 ,playButton->getContentSize().height / 2));
    playButton->addChild(playButtonIcon,1000);
    
    
    //Menu Button
    
    Sprite* normalmenuButton = Sprite::create("PlayReplaySettings/Btn1.png");
    Sprite* selectedmenuButton = Sprite::create("PlayReplaySettings/Btn1OC.png");
    
    cocos2d::MenuItemSprite* menuButton= cocos2d::MenuItemSprite::create(normalmenuButton, selectedmenuButton,CC_CALLBACK_0(DNPauseMenu::menuButtonTapped, this));
    menuButton->setTag(2);
    menuButton->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.45 ));
    
    cocos2d::Sprite* menuButtonIcon = cocos2d::Sprite::create("Victoryscreen/Home_Btn.png");
    menuButtonIcon->setPosition(cocos2d::Vec2(menuButton->getContentSize().width * 0.30, menuButton->getContentSize().height/2 ));
    menuButton->addChild(menuButtonIcon,1000);
    
    cocos2d::Label* menuButtonLabel = cocos2d::Label::createWithTTF("MENU","fonts/motschcc.ttf", 30);
    menuButtonLabel->setAnchorPoint(Vec2(0,0.5));
    menuButtonLabel->setColor(Color3B(119,74,59));
    menuButtonLabel->setPosition(cocos2d::Vec2( menuButton->getContentSize().width * 0.40,menuButton->getContentSize().height /2));
    menuButton->addChild(menuButtonLabel,1000);
    
    
    
    //Replay Button
    Sprite* normalreplayButton = Sprite::create("PlayReplaySettings/Btn1.png");
    Sprite* selectedreplayButton = Sprite::create("PlayReplaySettings/Btn1OC.png");
    
    cocos2d::MenuItemSprite* replayButton= cocos2d::MenuItemSprite::create(normalreplayButton, selectedreplayButton,CC_CALLBACK_0(DNPauseMenu::replayButtonTapped, this));
    replayButton->setTag(2);
    replayButton->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.30 ));
    
    cocos2d::Sprite* replayButtonIcon = cocos2d::Sprite::create("PauseScreen/ReplayBtn.png");
    replayButtonIcon->setPosition(cocos2d::Vec2(replayButton->getContentSize().width * 0.30, replayButton->getContentSize().height/2 ));
    replayButton->addChild(replayButtonIcon,1000);
    
    cocos2d::Label* replayButtonLabel = cocos2d::Label::createWithTTF("REPLAY","fonts/motschcc.ttf", 30);
    replayButtonLabel->setAnchorPoint(Vec2(0,0.5));
    replayButtonLabel->setColor(Color3B(119,74,59));
    replayButtonLabel->setPosition(cocos2d::Vec2( replayButton->getContentSize().width * 0.40,replayButton->getContentSize().height /2));
    replayButton->addChild(replayButtonLabel,1000);
    
    
    //Setting Button
    Sprite* normalsettingButton = Sprite::create("PlayReplaySettings/Btn1.png");
    Sprite* selectedsettingButton = Sprite::create("PlayReplaySettings/Btn1OC.png");
    
    cocos2d::MenuItemSprite* settingButton= cocos2d::MenuItemSprite::create(normalsettingButton, selectedsettingButton,CC_CALLBACK_0(DNPauseMenu::settingButtonTapped, this));
    settingButton->setTag(3);
    settingButton->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2,_background->getContentSize().height * 0.15 ));
    
    cocos2d::Sprite* settingButtonIcon = cocos2d::Sprite::create("PauseScreen/SettingBtn.png");
    settingButtonIcon->setPosition(cocos2d::Vec2(settingButton->getContentSize().width * 0.30, settingButton->getContentSize().height/2 ));
    settingButton->addChild(settingButtonIcon,1000);
    
    cocos2d::Label* settingButtonLabel = cocos2d::Label::createWithTTF("SETTINGS","fonts/motschcc.ttf", 30);
    settingButtonLabel->setColor(Color3B(119,74,59));
    settingButtonLabel->setAnchorPoint(Vec2(0,0.5));
    settingButtonLabel->setPosition(cocos2d::Vec2( settingButton->getContentSize().width * 0.40,settingButton->getContentSize().height /2));
    settingButton->addChild(settingButtonLabel,1000);
    
    
    cocos2d::Label* heading = cocos2d::Label::createWithTTF("Daily Objectives","fonts/motschcc.ttf", 30);
    heading->setColor(cocos2d::Color3B(119,74,59));
    heading->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height * 0.90));
    _background->addChild(heading,1000);
    
    //Daily Objective 1
    cocos2d::Label* objective1 = cocos2d::Label::createWithTTF(UserDefault::getInstance()->getStringForKey("alphaObjective", "abc"),"fonts/motschcc.ttf", 15);
    objective1->setColor(cocos2d::Color3B(119,74,59));
    objective1->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height * 0.80));
    _background->addChild(objective1,1000);
    
    if(UserDefault::getInstance()->getBoolForKey("DailyObjectiveAlphaComplete",false))
    {
        cocos2d::Sprite* tickMark = cocos2d::Sprite::create("Tutorial/TickMark.png");
        tickMark->setAnchorPoint(Vec2(0.5,0.5));
        tickMark->setPosition(cocos2d::Vec2( objective1->getPositionX() + objective1->getContentSize().width * 0.55, objective1->getPositionY() ));
        _background->addChild(tickMark,1000);
        
    }
    
    //Daily Objective 2
    cocos2d::Label* objective2 = cocos2d::Label::createWithTTF(UserDefault::getInstance()->getStringForKey("wallObjective", "abc"),"fonts/motschcc.ttf", 15);
    objective2->setColor(cocos2d::Color3B(119,74,59));
    objective2->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height * 0.725));
    _background->addChild(objective2,1000);
    
    if( UserDefault::getInstance()->getBoolForKey("DailyObjectiveWallComplete",false))
    {
        cocos2d::Sprite* tickMark = cocos2d::Sprite::create("Tutorial/TickMark.png");
        tickMark->setAnchorPoint(Vec2(0.5,0.5));
        tickMark->setPosition(cocos2d::Vec2( objective2->getPositionX()+ objective2->getContentSize().width * 0.62, objective2->getPositionY() ));
        _background->addChild(tickMark,1000);
    }
    
    
    //pause button
    _pauseMenu = cocos2d::Menu::create(playButton,menuButton,replayButton,settingButton,NULL);
    _pauseMenu->setPosition(cocos2d::Vec2::ZERO);
    _pauseMenu->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    _background->addChild(_pauseMenu, 1);
    
    return true;
}

#pragma -mark showPauseMenu
void DNPauseMenu::showPauseMenu()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    //runaction for pause menu
    _background->runAction(cocos2d::Sequence::create((cocos2d::MoveTo::create(0.1,cocos2d::Vec2(visibleSize.width/2 + origin.x,visibleSize.height /2+origin.y))), NULL));
    DNDataManager::sharedManager()->isInPauseMenu = true;
    fadeInBackground();
}

#pragma -mark hidePauseMenu
void DNPauseMenu::hidePauseMenu()
{
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    //runaction for pause menu
    _background->runAction(cocos2d::Sequence::create((cocos2d::MoveTo::create(0.6,cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height * 2 +origin.y))), NULL));
    
}

#pragma -mark playButtonTapped
void DNPauseMenu::playButtonTapped()
{
    _pauseMenu->setEnabled(false);
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    
    
    DNDataManager::sharedManager()->_HUDLayerRef->playButtonTapped();
    
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Resume_button", "Clicked", "Gameplay_Pause",1);
    
    
}


void DNPauseMenu:: replayButtonTapped()
{
    _pauseMenu->setEnabled(false);
    
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    
    DNDataManager::sharedManager()->_isReplayButtonTapped = true;
    auto scene = DNGameScene::createScene();
    DNDataManager::sharedManager()->_score = 0;
    DNDataManager::sharedManager()->_HUDLayerRef->score->setString("0");
    cocos2d::TransitionFade* transition = cocos2d::TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(transition);
    
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Replay_button", "Clicked", "Gameplay_Pause",1);
    
    
    
}

void DNPauseMenu::menuButtonTapped()
{
    _pauseMenu->setEnabled(false);
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->_isReplayButtonTapped = false;
    auto scene = DNGameScene::createScene();
    DNDataManager::sharedManager()->_score = 0;
    DNDataManager::sharedManager()->_HUDLayerRef->score->setString("0");
    
    cocos2d::TransitionFade* transition = cocos2d::TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(transition);
    
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Home_button", "Clicked", "Gameplay_Pause",1);
    
}

//handling Touch
#pragma mark - Touch_events
bool DNPauseMenu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
    
}

void DNPauseMenu::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(_background->getBoundingBox().containsPoint(touch->getLocation()))
    {
        return;
    }
    else if(DNDataManager::sharedManager()->_HUDLayerRef->_buttonToggle->getBoundingBox().containsPoint(touch->getLocation()))
    {
        return;
    }
    if(DNDataManager::sharedManager()->isInPauseMenu)
    {
        DNDataManager::sharedManager()->_isinGameScene = true;
        DNDataManager::sharedManager()->_HUDLayerRef->playButtonTapped();
    }
    
}


#pragma mark - settingButtonTapped
void DNPauseMenu::settingButtonTapped()
{
    _pauseMenu->setEnabled(false);
    
    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Setting_button", "Clicked", "Gameplay_Pause",1);
    
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->isInPauseMenu = false;
    DNDataManager::sharedManager()->_HUDLayerRef->settingButtonTapped();
    
}
#pragma mark - onExitButtonPressed
void DNPauseMenu::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(DNDataManager::sharedManager()->isInPauseMenu)
        {
            DNDataManager::sharedManager()->_HUDLayerRef->playButtonTapped();
        }
        
    }
}
#pragma -mark fadeInBackground
void DNPauseMenu::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
    }
    _fadeLayer = LayerColor::create(Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    _fadeLayer->retain();
    _fadeLayer->runAction(FadeTo::create(0.5f ,FADE_LAYER_OPACITY));
    addChild(_fadeLayer ,1000-1);
}

#pragma -mark fadeOutbackground
void DNPauseMenu::fadeOutbackground() {
    if (_fadeLayer) {
        CallFunc* calSelector = CallFunc::create(this, callfunc_selector(DNHUDLayer::removeFadeLayerFromParent));
        _fadeLayer->runAction(Sequence::create(FadeTo::create(0.5f, 0), calSelector, NULL));
    }
}

#pragma -mark removeFadeLayerFromParent
void DNPauseMenu::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        CC_SAFE_RELEASE_NULL(_fadeLayer);
    }
}

