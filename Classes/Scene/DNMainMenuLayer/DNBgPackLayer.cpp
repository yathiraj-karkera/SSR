//
//  DNBgPackLayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 09/08/17.
//
//

#include "DNBgPackLayer.hpp"
#include "ui/UIPageView.h"
#include "DNDataManager.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

#pragma -mark Constructor
DNBgPackLayer::DNBgPackLayer() {
    
    DNDataManager::sharedManager()->_bgPackRef = this;
    DNDataManager::sharedManager()->_isinGameScene = false;
}

#pragma -mark Destructor
DNBgPackLayer::~DNBgPackLayer() {
    
}

#pragma -mark createBGPackLayer
DNBgPackLayer* DNBgPackLayer::createBGPackLayer()
{
    DNBgPackLayer* scene = new DNBgPackLayer();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNBgPackLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    //PageView
    pageview = cocos2d::ui::PageView::create();
    pageview->setTouchEnabled(true);
    pageview->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    pageview->setContentSize(cocos2d::Size(visibleSize.width,visibleSize.height));
    pageview->setPosition(cocos2d::Vec2(origin.x + visibleSize.width/2,origin.y+visibleSize.height/2));
    
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/BackgroundDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    
    _typeOneDetailsArr.clear();
    _typeOneDetailsArr = rootDict["Background"].asValueVector();
    
    for(int i = 0;i< _typeOneDetailsArr.size() ; i++)
    {
        ValueMap typeDetailDict = _typeOneDetailsArr.at(i).asValueMap();
        auto path =  typeDetailDict["Path"].asString();
        
        //BackGroundLayer
        cocos2d::Sprite* m_LayerBackground1 = cocos2d::Sprite::create(path + "Background/BG.png");
        m_LayerBackground1->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerBackground1->setPosition(cocos2d::Vec2(0  ,0 ));
        
        //Running Ground
        cocos2d::Sprite* m_LayerGround1 = cocos2d::Sprite::create(path + "Track/ground.png");
        m_LayerGround1->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerGround1->setPosition(cocos2d::Vec2(0,0));
        
        //Grass Fence And Trees
        cocos2d::Sprite* m_LayerGrass1 = cocos2d::Sprite::create(path + "Track/trees.png");
        m_LayerGrass1->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerGrass1->setPosition(cocos2d::Vec2(0,m_LayerGround1->getContentSize().height * 0.68));
        
        //Building Layer 1
        cocos2d::Sprite* m_LayerBuilding1 = cocos2d::Sprite::create(path + "layer2.png");
        m_LayerBuilding1->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerBuilding1->setPosition(cocos2d::Vec2(0,m_LayerGround1->getContentSize().height * 0.68));
        
        //Building Layer 2
        cocos2d::Sprite* m_LayerBuilding3 = cocos2d::Sprite::create(path + "layer3.png");
        m_LayerBuilding3->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerBuilding3->setPosition(cocos2d::Vec2(0,m_LayerGround1->getContentSize().height * 0.68));
        
        //CLOUDS
        cocos2d::Sprite* m_LayerCloud1 = cocos2d::Sprite::create(path + "Clouds.png");
        m_LayerCloud1->setAnchorPoint(cocos2d::Vec2(0,0));
        m_LayerCloud1->setPosition(cocos2d::Vec2(0,origin.y + visibleSize.height - m_LayerCloud1->getContentSize().height));
        
        m_LayerBackground1->addChild(m_LayerGround1,10);
        
        m_LayerBackground1->addChild(m_LayerGrass1,9);
        
        m_LayerBackground1->addChild(m_LayerBuilding1,8);
        
        m_LayerBackground1->addChild(m_LayerBuilding3,7);
        m_LayerBackground1->addChild(m_LayerCloud1,3);
        
        m_LayerGround1->setScale(1,0.75);
        
        m_LayerGrass1->setScale(1,0.75);
        
        m_LayerBuilding1->setScale(1,1);
        
        m_LayerBuilding3->setScale(1,1);
        
        m_LayerCloud1->setScale(1,0.75);
        
        
        cocos2d::ui::Layout *layout1 = cocos2d::ui::Layout::create();
        layout1->setContentSize(cocos2d::Size(visibleSize.width,visibleSize.height));
        layout1->addChild(m_LayerBackground1);
        
        pageview->insertPage(layout1,i);
        
    }
    
    int indexValue = UserDefault::getInstance()->getIntegerForKey("BGIndex", 0);
    UserDefault::getInstance()->setIntegerForKey("BGIndex1", indexValue);
    pageview->setCurrentPageIndex(indexValue);
    setBg();
    
    pageview->addEventListener(CC_CALLBACK_2(DNBgPackLayer::pageViewEvent, this));
    
    this->addChild(pageview);
    
    return true;
    
}

#pragma -mark pageViewEvent
void DNBgPackLayer::pageViewEvent(cocos2d::Ref* sender,cocos2d::ui::PageView::EventType type)
{
    switch (type) {
        case cocos2d::ui::PageView::EventType::TURNING:
            cocos2d::ui::PageView *pageView = dynamic_cast<cocos2d::ui::PageView*>(sender);
            auto index = pageView->getCurrentPageIndex();
            //UserDefault::getInstance()->setIntegerForKey("BgIndex", index);
            UserDefault::getInstance()->setIntegerForKey("BGIndex1", index);
            checkBg(index);
            break;
            
    }
}

#pragma mark - checkBg
void DNBgPackLayer::checkBg(int index)
{
    DNDataManager::sharedManager()->_mainmenu->_leftArrow->setVisible(true);
    DNDataManager::sharedManager()->_mainmenu->_leftArrow->setEnabled(true);
    DNDataManager::sharedManager()->_mainmenu->_rightArrow->setVisible(true);
    DNDataManager::sharedManager()->_mainmenu->_rightArrow->setEnabled(true);
    if(index == 0)
    {
        index = 0;
        DNDataManager::sharedManager()->_mainmenu->_leftArrow->setVisible(false);
        DNDataManager::sharedManager()->_mainmenu->_leftArrow->setEnabled(false);
    }
    else if(index == 5)
    {
        index = 5;
        DNDataManager::sharedManager()->_mainmenu->_rightArrow->setVisible(false);
        DNDataManager::sharedManager()->_mainmenu->_rightArrow->setEnabled(false);
        
    }
    
    ValueMap typeDetailDict;
    typeDetailDict.clear();
    typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    
    
    auto bgNameForLockedOrUnlocked = typeDetailDict["BgName"].asString();
    if(index == 0)
    {
        DNDataManager::sharedManager()->_isDefaultBg = true;
        UserDefault::getInstance()->setBoolForKey(bgNameForLockedOrUnlocked.c_str(), true);
    }
    else
    {
        DNDataManager::sharedManager()->_isDefaultBg = false;
    }
    
    auto isLocked = UserDefault::getInstance()->getBoolForKey(bgNameForLockedOrUnlocked.c_str(), false);
    
    if(!isLocked)
    {
        DNDataManager::sharedManager()->_isBgAvailable = false;
        
        DNDataManager::sharedManager()->_mainmenu->unlockDescription->setString(typeDetailDict["UnlockTask"].asString());
        DNDataManager::sharedManager()->_mainmenu->enabledLockScreen();
        if(DNDataManager::sharedManager()->_mainmenu->_productPrice.size() > 0)
        {
            DNDataManager::sharedManager()->_mainmenu->_price->setString(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("BgAmount",10)));
        }
    }
    else{
        
        UserDefault::getInstance()->setStringForKey("Path",typeDetailDict["Path"].asString());
        auto path =  UserDefault::getInstance()->getStringForKey("Path","BG_Assets/Daylight_BG/");
        
        UserDefault::getInstance()->setIntegerForKey("BGIndex", index);
        DNDataManager::sharedManager()->_isBgAvailable = true;
        DNDataManager::sharedManager()->_mainmenu->disableLockScreen();
        
    }
}


#pragma mark - setBg
void DNBgPackLayer::setBg()
{
    auto path = UserDefault::getInstance()->getStringForKey("Path","BG_Assets/Daylight_BG/");
    int index = UserDefault::getInstance()->getIntegerForKey("BGIndex", 0);
    UserDefault::getInstance()->setIntegerForKey("BgIndex1", index);
    
    int remainder = index %  2;
    
    switch (remainder) {
        case 0:
            if(!UserDefault::getInstance()->getBoolForKey("morningmusicPlaying",false))
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/morning.mp3",true);
                UserDefault::getInstance()->setBoolForKey("morningmusicPlaying",true);
                UserDefault::getInstance()->setBoolForKey("nightmusicPlaying",false);
            }
            break;
        case 1:
            if(!UserDefault::getInstance()->getBoolForKey("nightmusicPlaying",false))
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/night.mp3",true);
                UserDefault::getInstance()->setBoolForKey("nightmusicPlaying",true);
                UserDefault::getInstance()->setBoolForKey("morningmusicPlaying",false);
            }
            break;
            
    }
    
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto bgLayerRef = DNDataManager::sharedManager()->_backGroundLayerRef;
    bgLayerRef->m_LayerBackground->setTexture(path + "Background/BG.png");
    bgLayerRef->m_LayerGround1->setTexture(path + "Track/ground.png");
    bgLayerRef->m_LayerGround2->setTexture(path + "Track/ground.png");
    bgLayerRef->m_LayerGrass1->setTexture(path + "Track/trees.png");
    bgLayerRef->m_LayerGrass2->setTexture(path + "Track/trees.png");
    bgLayerRef->m_LayerBuilding1->setTexture(path + "layer2.png");
    bgLayerRef->m_LayerBuilding2->setTexture(path + "layer2.png");
    bgLayerRef->m_LayerBuilding3->setTexture(path + "layer3.png");
    bgLayerRef->m_LayerBuilding4->setTexture(path + "layer3.png");
    bgLayerRef->m_LayerCloud1->setTexture(path + "Clouds.png");
    bgLayerRef->m_LayerCloud2->setTexture(path + "Clouds.png");
    
    bgLayerRef->m_LayerBuilding1->setPosition(cocos2d::Vec2(0,bgLayerRef->m_LayerGround1->getPositionY() + bgLayerRef->m_LayerGround1->getContentSize().height * 0.68));
    bgLayerRef->m_LayerBuilding2->setPosition(cocos2d::Vec2(bgLayerRef->m_LayerBuilding1->getPositionX() + bgLayerRef->m_LayerBuilding1->getContentSize().width ,bgLayerRef->m_LayerGround1->getPositionY() + bgLayerRef->m_LayerGround1->getContentSize().height * 0.68));
    bgLayerRef->m_LayerBuilding3->setPosition(cocos2d::Vec2(0,bgLayerRef->m_LayerGround1->getPositionY() + bgLayerRef->m_LayerGround1->getContentSize().height * 0.68));
    bgLayerRef->m_LayerBuilding4->setPosition(cocos2d::Vec2(bgLayerRef->m_LayerBuilding3->getPositionX() + bgLayerRef->m_LayerBuilding3->getContentSize().width,bgLayerRef->m_LayerGround1->getPositionY() + bgLayerRef->m_LayerGround1->getContentSize().height * 0.68));
    
    DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setPosition(Vec2(DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width/2  ,15));
    
}
