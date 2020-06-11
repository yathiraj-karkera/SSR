//
//  DNSettingScene.cpp
//  DesiNinja
//
//  Created by Yathiraj on 07/08/17.
//
//
#include "DNSettingScene.hpp"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "DNDataManager.h"
#include "cocos2d.h"
//#include "PluginFirebase/PluginFirebase.h"
#include    "DNConstants.h"
#include "DNPauseMenu.hpp"
#include "audio/include/AudioEngine.h"


USING_NS_CC;
using namespace cocos2d::ui;
#pragma -mark Constructor
DNSettingScene::DNSettingScene()
{
    _fadeLayer = NULL;
    auto listener=cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2( DNSettingScene::onTouchBegan, this);
    listener->onTouchEnded=CC_CALLBACK_2(DNSettingScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
}

#pragma -mark Destructor
DNSettingScene::~DNSettingScene()
{
    
}

#pragma -mark createScene
DNSettingScene* DNSettingScene::createScene()
{
    DNSettingScene* scene = new DNSettingScene();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark init
// on "init" you need to initialize your instance
bool DNSettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    fadeInBackground();
    
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(DNSettingScene::onExitButtonPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    
    _background = cocos2d::Sprite::create("SettingScene/Popup.png");
    _background->setPosition(cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height * 1.2 + origin.y));
    addChild(_background,1000);
    
    cocos2d::Label* headingText = cocos2d::Label::createWithTTF("SETTINGS","fonts/motschcc.ttf", 40);
    headingText->setColor(Color3B(119,74,59));
    headingText->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height - headingText->getContentSize().height * 1.5));
    _background->addChild(headingText,1000);
    
    Sprite *maskBackground = cocos2d::Sprite::create("SettingScene/Transparnt-Box.png");
    maskBackground->setPosition(cocos2d::Vec2(_background->getContentSize().width/2,_background->getContentSize().height * 0.40));
    _background->addChild(maskBackground,1000);
    
    //slide bar for volume
    auto volumeSlider = Slider::create();
    volumeSlider->loadBarTexture("SettingScene/volume_bar_empty.png"); // what the slider looks like
    volumeSlider->loadSlidBallTextures("SettingScene/volume_pointer.png");
    volumeSlider->loadProgressBarTexture("SettingScene/volume_bar_fill.png");
    volumeSlider->setPosition(Vec2(maskBackground->getContentSize().width * 0.625 ,maskBackground->getContentSize().height * 0.80));
    volumeSlider->addEventListener(CC_CALLBACK_2(DNSettingScene::sliderEvent, this));
    volumeSlider->setTag(1);
    
    float volume = UserDefault::getInstance()->getFloatForKey("volume", 0.5) * 100;
    ////CCLOG("%f",volume);
    volumeSlider->setPercent(volume );
    maskBackground->addChild(volumeSlider);
    
    
    cocos2d::Label* volumeSliderLabel = cocos2d::Label::createWithTTF("Music","fonts/motschcc.ttf", 25);
    volumeSliderLabel->setColor(Color3B(119,74,59));
    volumeSliderLabel->setAnchorPoint(Vec2(0,0.5));
    volumeSliderLabel->setPosition(cocos2d::Vec2( maskBackground->getContentSize().width * 0.05,volumeSlider->getPosition().y));
    maskBackground->addChild(volumeSliderLabel,1000);
    
    //slide bar for volume
    auto musicSlider = Slider::create();
    musicSlider->loadBarTexture("SettingScene/volume_bar_empty.png"); // what the slider looks like
    musicSlider->loadSlidBallTextures("SettingScene/volume_pointer.png");
    musicSlider->loadProgressBarTexture("SettingScene/volume_bar_fill.png");
    musicSlider->setPosition(Vec2(maskBackground->getContentSize().width * 0.625 ,maskBackground->getContentSize().height * 0.50));
    musicSlider->addEventListener(CC_CALLBACK_2(DNSettingScene::sliderEvent, this));
    musicSlider->setTag(2);
    float effect = UserDefault::getInstance()->getFloatForKey("effect", 0.5) * 100;
    //CCLOG("%f",effect);
    musicSlider->setPercent(effect);
    maskBackground->addChild(musicSlider);
    
    cocos2d::Label* musicSliderLabel = cocos2d::Label::createWithTTF("Sound","fonts/motschcc.ttf", 25);
    musicSliderLabel->setColor(Color3B(119,74,59));
    musicSliderLabel->setAnchorPoint(Vec2(0,0.5));
    musicSliderLabel->setPosition(cocos2d::Vec2( maskBackground->getContentSize().width * 0.05,musicSlider->getPosition().y));
    maskBackground->addChild(musicSliderLabel,1000);
    
    return true;
}

#pragma -mark sliderEvent
void DNSettingScene::sliderEvent(Ref* pSender, ui::Slider::EventType type)
{
    if ( type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        ui::Slider* slider = dynamic_cast<ui::Slider *>(pSender);
        float percent = slider->getPercent();
        //CCLOG("%f",percent);
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        if(slider->getTag() == 1)
        {
            
            UserDefault::getInstance()->setFloatForKey("volume", percent/100);
            audio->setBackgroundMusicVolume(percent / 100 );
            
            //test
            if(percent==100)
            {
                //Analytics
                DNDataManager::sharedManager()->callAnalytics("SoundOn_button", "Clicked", "Sound",1);
            }
            //test
            if(percent==0)
            {
                //Analytics
                DNDataManager::sharedManager()->callAnalytics("SoundOff_button", "Clicked", "Sound",1);
            }
            
        }
        else if(slider->getTag() == 2)
        {
           
            UserDefault::getInstance()->setFloatForKey("effect", percent/100);
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(percent/100);
           
            //test
            if(percent==100)
            {
                //Analytics
                DNDataManager::sharedManager()->callAnalytics("MusicOn_button", "Clicked", "Music",1);
            }
            //test
            if(percent==0)
            {
                //Analytics
                DNDataManager::sharedManager()->callAnalytics("MusicOff_button", "Clicked", "Music",1);
            }
        }
    }
}

#pragma -mark showSettingMenu
void DNSettingScene::showSettingMenu()
{
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    _background->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.6,cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height /2+origin.y))), NULL));
}

#pragma -mark hideSettingMenu
void DNSettingScene::hideSettingMenu()
{
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    _background->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.6,cocos2d::Vec2(visibleSize.width/2+origin.x,visibleSize.height * 2 +origin.y))), NULL));
}


#pragma mark - onExitButtonPressed
void DNSettingScene::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        okButtonPressed();
    }
}

#pragma -mark okButtonPressed
void DNSettingScene::okButtonPressed()
{
    fadeOutbackground();
    DNDataManager::sharedManager()->_HUDLayerRef->playButtonTapped();
}

//handling Touch
#pragma mark - Touch_events
bool DNSettingScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
    
}

void DNSettingScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(_background->getBoundingBox().containsPoint(touch->getLocation()))
    {
        return;
    }
    okButtonPressed();
}
#pragma -mark fadeInBackground
void DNSettingScene::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
    }
    _fadeLayer = LayerColor::create(Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    _fadeLayer->retain();
    _fadeLayer->runAction(FadeTo::create(0.5f ,200));
    addChild(_fadeLayer ,100);
}

#pragma -mark fadeOutbackground
void DNSettingScene::fadeOutbackground() {
    if (_fadeLayer) {
        
        _fadeLayer->runAction(Sequence::create(FadeTo::create(0.5f, 0), NULL));
    }
}

#pragma -mark removeFadeLayerFromParent
void DNSettingScene::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        CC_SAFE_RELEASE_NULL(_fadeLayer);
    }
}

