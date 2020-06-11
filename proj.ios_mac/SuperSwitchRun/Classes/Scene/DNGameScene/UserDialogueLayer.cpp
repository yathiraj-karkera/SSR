//
//  UserDialogueLayer.cpp
//  MyGame3
//
//  Created by Yathiraj on 09/09/17.
//
//

#include "UserDialogueLayer.hpp"
#include "SimpleAudioEngine.h"
#include "DNConstants.h"
#include "DNDataManager.h"
#include "cocos2d.h"

#pragma -mark Constructor
UserDialogueLayer::UserDialogueLayer() {
    _fadeLayer = NULL;
    DNDataManager::sharedManager()->isInPauseMenu =false;
    DNDataManager::sharedManager()->_dialogueLayer = this;
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2( UserDialogueLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(UserDialogueLayer::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    DNDataManager::sharedManager()->_isinGameScene = false;
    DNDataManager::sharedManager()->_isinGameScene = false;
    UserDefault::getInstance()->setBoolForKey("userDialogue", true);
    
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(UserDialogueLayer::onExitButtonPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

#pragma -mark Destructor
UserDialogueLayer::~UserDialogueLayer() {
    
    
}

#pragma -mark createBackground
UserDialogueLayer* UserDialogueLayer::createDialogueMenu()
{
    UserDialogueLayer* scene = new UserDialogueLayer();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool UserDialogueLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    fadeInBackground();
    
    return true;
    
}

void UserDialogueLayer::createLoadingPopUp()
{
    cocos2d::Label* description = cocos2d::Label::createWithTTF("Loading........","fonts/motschcc.ttf", 40);
    description->setColor(cocos2d::Color3B(119,74,59));
    description->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.50));
    _quitMenuBackground->addChild(description,100000);
    
    _isinternetCheck = true;
}


void UserDialogueLayer::createQuitDialogue()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    DNDataManager::sharedManager()->_isUserDialogueCreated = true;
    _quitMenuBackground = cocos2d::Sprite::create("WatchAddScreen/Popup1.png");
    _quitMenuBackground->setPosition(cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height * 1.2 + origin.y));
    addChild(_quitMenuBackground,1000003);
    
    
    
    cocos2d::Label* description = cocos2d::Label::createWithTTF("Do you want to quit","fonts/motschcc.ttf", 40);
    description->setColor(cocos2d::Color3B(119,74,59));
    description->setPosition(cocos2d::Vec2( _quitMenuBackground->getContentSize().width/2,_quitMenuBackground->getContentSize().height * 0.75));
    _quitMenuBackground->addChild(description,100000);
    
    
    
    
    cocos2d::MenuItemImage* yesButton = cocos2d::MenuItemImage::create("WatchAddScreen/Yes.png", "WatchAddScreen/Yes.png",CC_CALLBACK_0(UserDialogueLayer::quitGame, this));
    yesButton->setTag(3);
    yesButton->setPosition(cocos2d::Vec2(_quitMenuBackground->getContentSize().width * 0.30 ,_quitMenuBackground->getContentSize().height * 0.30 ));
    
    
    cocos2d::Label* yesButtonlabel = cocos2d::Label::createWithTTF("Yes","fonts/motschcc.ttf", 30);
    yesButtonlabel->setColor(cocos2d::Color3B(119,74,59));
    yesButtonlabel->setPosition(cocos2d::Vec2( yesButton->getContentSize().width/2,yesButton->getContentSize().height / 2));
    yesButton->addChild(yesButtonlabel,1000);
    
    
    cocos2d::MenuItemImage* quit = cocos2d::MenuItemImage::create("WatchAddScreen/No.png", "WatchAddScreen/No.png",CC_CALLBACK_0(UserDialogueLayer::hideDoyouWantToQuitMenu, this));
    quit->setTag(4);
    quit->setPosition(cocos2d::Vec2(_quitMenuBackground->getContentSize().width * 0.70 ,_quitMenuBackground->getContentSize().height *0.30));
    
    cocos2d::Label* quitlabel = cocos2d::Label::createWithTTF("No","fonts/motschcc.ttf", 30);
    quitlabel->setColor(cocos2d::Color3B(119,74,59));
    quitlabel->setPosition(cocos2d::Vec2( quit->getContentSize().width/2,quit->getContentSize().height / 2));
    quit->addChild(quitlabel,1000);
    
    cocos2d::Menu* menu = cocos2d::Menu::create(yesButton,quit,NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    menu->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    _quitMenuBackground->addChild(menu, 1);
    
    _isinternetCheck = true;
    DNDataManager::sharedManager()->_mainmenu->menu->setEnabled(false);
    DNDataManager::sharedManager()->_mainmenu->arrowMenu->setEnabled(false);
    DNDataManager::sharedManager()->_mainmenu->_chracterPackMenu->setEnabled(false);
    
    showDoyouWantToQuitMenu();
}

void UserDialogueLayer::showDoyouWantToQuitMenu()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    _quitMenuBackground->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.6,cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height /2+origin.y))), NULL));
}

void UserDialogueLayer::internetIssuePopUp()
{
    _isinternetCheck = true;
    Label* internetPopup  = cocos2d::Label::createWithTTF("No Internet","fonts/motschcc.ttf", 50);
    internetPopup->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.55));
    this->addChild(internetPopup,100);
    
    Label* internetPopup2  = cocos2d::Label::createWithTTF("Please Try Again!","fonts/motschcc.ttf", 50);
    internetPopup2->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.45));
    this->addChild(internetPopup2,100);
    
    cocos2d::MenuItemImage* quit = cocos2d::MenuItemImage::create("PlayReplaySettings/Play_Btn.png", "PlayReplaySettings/Play-BtnOC.png",CC_CALLBACK_0(UserDialogueLayer::removeInternetPopup, this));
    quit->setTag(4);
    quit->setPosition(cocos2d::Vec2(this->getContentSize().width * 0.50 ,this->getContentSize().height *0.30));
    
    cocos2d::Label* quitlabel = cocos2d::Label::createWithTTF("Okay","fonts/motschcc.ttf", 30);
    quitlabel->setColor(cocos2d::Color3B(119,74,59));
    quitlabel->setPosition(cocos2d::Vec2( quit->getContentSize().width/2,quit->getContentSize().height / 2));
    quit->addChild(quitlabel,1000);
    
    cocos2d::Menu* menu = cocos2d::Menu::create(quit,NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    menu->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    this->addChild(menu, 1000);
    
    
    
}

void UserDialogueLayer::hideDoyouWantToQuitMenu()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    _quitMenuBackground->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.6,cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height * 1.3 +origin.y))), NULL));
    
    
    removeInternetPopup();
}

#pragma -mark fadeInBackground
void UserDialogueLayer::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
    }
    _fadeLayer = LayerColor::create(Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    _fadeLayer->retain();
    _fadeLayer->runAction(FadeTo::create(0.5f ,200));
    addChild(_fadeLayer ,100);
}

#pragma -mark fadeOutbackground
void UserDialogueLayer::fadeOutbackground() {
    if (_fadeLayer) {
        
        _fadeLayer->runAction(Sequence::create(FadeTo::create(0.5f, 0), NULL));
    }
}

#pragma -mark removeFadeLayerFromParent
void UserDialogueLayer::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        CC_SAFE_RELEASE_NULL(_fadeLayer);
    }
}

bool UserDialogueLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
    if(_isinternetCheck)
    {
        return true;
    }
    else{
        return false;
        
    }
}



#pragma mark - onTouchEnded
void UserDialogueLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
    if(DNDataManager::sharedManager()->_isFromReviveAd)
    {
        DNDataManager::sharedManager()->_isFromReviveAd = false;
        DNDataManager::sharedManager()->_gameLayerRef->removeNoAdPopUp();
        _isinternetCheck = false;
        return;
    }
    removeInternetPopup();
    _isinternetCheck = false;
    
    
}

#pragma mark - quitGame
void UserDialogueLayer::quitGame()
{
    exit(0);
}

#pragma mark - removeInternetPopup
void UserDialogueLayer::removeInternetPopup()
{
    
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if(DNDataManager::sharedManager()->_isFromReviveAd)
    {
        DNDataManager::sharedManager()->_isFromReviveAd = false;
        DNDataManager::sharedManager()->_gameLayerRef->removeNoAdPopUp();
        _isinternetCheck = false;
        return;
    }
    if(DNDataManager::sharedManager()->_mainmenu)
    {
        DNDataManager::sharedManager()->_mainmenu->menu->setEnabled(true);
        DNDataManager::sharedManager()->_mainmenu->arrowMenu->setEnabled(true);
        DNDataManager::sharedManager()->_mainmenu->_chracterPackMenu->setEnabled(true);
    }
    
    DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
    
}

#pragma mark - connectingPopUp
void UserDialogueLayer::connectingPopUp()
{
    _isinternetCheck = true;
    Label* internetPopup1  = cocos2d::Label::createWithTTF("Please Wait","fonts/motschcc.ttf", 50);
    internetPopup1->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.60));
    this->addChild(internetPopup1,100);
    
    Label* internetPopup2  = cocos2d::Label::createWithTTF("Or","fonts/motschcc.ttf", 50);
    internetPopup2->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.50));
    this->addChild(internetPopup2,100);
    
    Label* internetPopup3  = cocos2d::Label::createWithTTF("Press anywhere to continue","fonts/motschcc.ttf", 50);
    internetPopup3->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.40));
    this->addChild(internetPopup3,100);
    
}

#pragma mark - noAdPopUp
void UserDialogueLayer::noAdPopUp()
{
    _isinternetCheck = true;
    Label* internetPopup1  = cocos2d::Label::createWithTTF("No ads available","fonts/motschcc.ttf", 50);
    internetPopup1->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.60));
    this->addChild(internetPopup1,100);
    
    Label* internetPopup2  = cocos2d::Label::createWithTTF("Tap to continue","fonts/motschcc.ttf", 50);
    internetPopup2->setPosition(cocos2d::Vec2( this->getContentSize().width/2,this->getContentSize().height * 0.50));
    this->addChild(internetPopup2,100);
    
    
    
}

#pragma mark - onExitButtonPressed
void UserDialogueLayer::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(DNDataManager::sharedManager()->isInPauseMenu)
        {
            hideDoyouWantToQuitMenu();
        }
        
    }
}



