//
//  DNBackGroundLayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNBackGroundLayer.hpp"
#include "cocos2d.h"
#include "DNGameScene.hpp"
#include "DNDataManager.h"
#include "DNConstants.h"

#pragma -mark Constructor
DNBackGroundLayer::DNBackGroundLayer() {
    DNDataManager::sharedManager()->_backGroundLayerRef = this;
       m_LayerBackground = 0;
}

#pragma -mark Destructor
DNBackGroundLayer::~DNBackGroundLayer() {
    
}

#pragma -mark createBackground
DNBackGroundLayer* DNBackGroundLayer::createBackground()
{
    DNBackGroundLayer* scene = new DNBackGroundLayer();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNBackGroundLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    this->visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    this->origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    auto path =  UserDefault::getInstance()->getStringForKey("Path","BG_Assets/Daylight_BG/");
    
    //BackGroundLayer
    m_LayerBackground = cocos2d::Sprite::create(path + "Background/BG.png");
    m_LayerBackground->setContentSize(Size(visibleSize.width,visibleSize.height));
    m_LayerBackground->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerBackground->setPosition(cocos2d::Vec2(0,0 ));
    
    //Running Ground
    m_LayerGround1 = cocos2d::Sprite::create(path + "Track/ground.png");
    m_LayerGround1->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerGround1->setPosition(cocos2d::Vec2(origin.x ,origin.y));
    
    m_LayerGround2 = cocos2d::Sprite::create(path + "Track/ground.png");
    m_LayerGround2->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerGround2->setPosition(cocos2d::Vec2(m_LayerGround1->getPositionX() + m_LayerGround1->getContentSize().width,origin.y));
    
    //Grass Fence And Trees
    m_LayerGrass1 = cocos2d::Sprite::create(path + "Track/trees.png");
    m_LayerGrass1->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerGrass1->setPosition(cocos2d::Vec2(0,m_LayerGround1->getPositionY() + m_LayerGround1->getContentSize().height * 0.68));
    
    m_LayerGrass2 = cocos2d::Sprite::create(path + "Track/trees.png");
    m_LayerGrass2->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerGrass2->setPosition(cocos2d::Vec2(m_LayerGrass1->getPositionX() + m_LayerGrass1->getContentSize().width,m_LayerGround1->getPositionY()+ m_LayerGround1->getContentSize().height * 0.68));
    
    //Building Layer 1
    m_LayerBuilding1 = cocos2d::Sprite::create(path + "layer2.png");
    m_LayerBuilding1->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerBuilding1->setPosition(cocos2d::Vec2(0,m_LayerGround1->getPositionY() + m_LayerGround1->getContentSize().height * 0.68));
    
    m_LayerBuilding2 = cocos2d::Sprite::create(path + "layer2.png");
    m_LayerBuilding2->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerBuilding2->setPosition(cocos2d::Vec2(m_LayerBuilding1->getPositionX() + m_LayerBuilding1->getContentSize().width ,m_LayerGround1->getPositionY() + m_LayerGround1->getContentSize().height * 0.68));
    
    //Building Layer 2
    m_LayerBuilding3 = cocos2d::Sprite::create(path + "layer3.png");
    m_LayerBuilding3->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerBuilding3->setPosition(cocos2d::Vec2(0,m_LayerGround1->getPositionY() + m_LayerGround1->getContentSize().height * 0.68));
    
    m_LayerBuilding4 = cocos2d::Sprite::create(path + "layer3.png");
    m_LayerBuilding4->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerBuilding4->setPosition(cocos2d::Vec2(m_LayerBuilding3->getPositionX() + m_LayerBuilding3->getContentSize().width,m_LayerGround1->getPositionY() + m_LayerGround1->getContentSize().height * 0.68));
    
    //CLOUDS
    m_LayerCloud1 = cocos2d::Sprite::create(path + "Clouds.png");
    m_LayerCloud1->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerCloud1->setPosition(cocos2d::Vec2(0,origin.y + visibleSize.height - m_LayerCloud1->getContentSize().height));
    
    m_LayerCloud2 = cocos2d::Sprite::create(path + "Clouds.png");
    m_LayerCloud2->setAnchorPoint(cocos2d::Vec2(0,0));
    m_LayerCloud2->setPosition(cocos2d::Vec2(m_LayerCloud1->getPositionX()+ m_LayerCloud1->getContentSize().width,origin.y + visibleSize.height - m_LayerCloud1->getContentSize().height));
    
    
    // add the sprite as a child to this layer
    this->addChild(m_LayerBackground, 0);
    //    add the sprite as a child to this layer
    m_LayerBackground->addChild(m_LayerGround1,10);
    m_LayerBackground->addChild(m_LayerGround2,10);
    m_LayerBackground->addChild(m_LayerGrass1,9);
    m_LayerBackground->addChild(m_LayerGrass2,9);
    m_LayerBackground->addChild(m_LayerBuilding1,8);
    m_LayerBackground->addChild(m_LayerBuilding2,8);
    m_LayerBackground->addChild(m_LayerBuilding3,7);
    m_LayerBackground->addChild(m_LayerBuilding4,7);
    m_LayerBackground->addChild(m_LayerCloud1,3);
    m_LayerBackground->addChild(m_LayerCloud2,3);
    
//setting game bg scale
    m_LayerGround1->setScale(1,0.75);
    m_LayerGround2->setScale(1,0.75);
    m_LayerGrass1->setScale(1,0.75);
    m_LayerGrass2->setScale(1,0.75);
    m_LayerBuilding1->setScale(1,1);
    m_LayerBuilding2->setScale(1,1);
    m_LayerBuilding3->setScale(1,1);
    m_LayerBuilding4->setScale(1,1);
    m_LayerCloud1->setScale(1,0.75);
    m_LayerCloud2->setScale(1,0.75);

    
    return true;
}

#pragma -mark update
void DNBackGroundLayer::bgUpdate(float dtSpeed)
{
    auto dataManager = DNDataManager::sharedManager();
    
    //Updating Position
    m_LayerGround1->setPosition(Vec2(m_LayerGround1->getPositionX()- dataManager->_gameSpeed * dtSpeed,m_LayerGround1->getPositionY()));
    m_LayerGround2->setPosition(Vec2(m_LayerGround2->getPositionX()- dataManager->_gameSpeed * dtSpeed,m_LayerGround2->getPositionY()));
    
    m_LayerGrass1->setPosition(Vec2(m_LayerGrass1->getPositionX()- dataManager->_groundSpeed * dtSpeed,m_LayerGrass1->getPositionY()));
    m_LayerGrass2->setPosition(Vec2(m_LayerGrass2->getPositionX()- dataManager->_groundSpeed * dtSpeed,m_LayerGrass2->getPositionY()));
    
    m_LayerBuilding1->setPosition(Vec2(m_LayerBuilding1->getPositionX()- dataManager->_BuildingLayer1Speed * dtSpeed,m_LayerBuilding1->getPositionY()));
    m_LayerBuilding2->setPosition(Vec2(m_LayerBuilding2->getPositionX()- dataManager->_BuildingLayer1Speed * dtSpeed,m_LayerBuilding2->getPositionY()));
//    m_LayerBuilding3->setPosition(Vec2(m_LayerBuilding3->getPositionX()- dataManager->_BuildingLayer2Speed*dtSpeed,m_LayerBuilding3->getPositionY()));
//    m_LayerBuilding4->setPosition(Vec2(m_LayerBuilding4->getPositionX()- dataManager->_BuildingLayer2Speed*dtSpeed,m_LayerBuilding4->getPositionY()));
//
//    m_LayerCloud1->setPosition(Vec2(m_LayerCloud1->getPositionX()- dataManager->_cloudSpeed,m_LayerCloud1->getPositionY()));
//    m_LayerCloud2->setPosition(Vec2(m_LayerCloud2->getPositionX()- dataManager->_cloudSpeed,m_LayerCloud2->getPositionY()));

    //Resetting Background Objects
    if(POSX(m_LayerGround1) + WIDTH(m_LayerGround1) <= 0)
    {
        m_LayerGround1->setPosition(Vec2(m_LayerGround2->getPositionX() + m_LayerGround2->getContentSize().width,m_LayerGround1->getPositionY()));
    }
    else if(POSX(m_LayerGround2)+WIDTH(m_LayerGround2) <= 0)
    {
        m_LayerGround2->setPosition(Vec2(m_LayerGround1->getPositionX() + m_LayerGround1->getContentSize().width   ,m_LayerGround2->getPositionY()));
    }
    
    if(POSX(m_LayerGrass1) + WIDTH(m_LayerGrass1) <= 0)
    {
        m_LayerGrass1->setPosition(Vec2(m_LayerGrass2->getPositionX() + m_LayerGrass2->getContentSize().width,m_LayerGrass1->getPositionY()));
    }
    else if(POSX(m_LayerGrass2)+WIDTH(m_LayerGrass2) <= 0)
    {
        m_LayerGrass2->setPosition(Vec2(m_LayerGrass1->getPositionX() + m_LayerGrass1->getContentSize().width  ,m_LayerGrass2->getPositionY()));
    }
    
    if(POSX(m_LayerBuilding1) + WIDTH(m_LayerBuilding1) <= 0)
    {
        m_LayerBuilding1->setPosition(Vec2(m_LayerBuilding2->getPositionX() + m_LayerBuilding2->getContentSize().width,m_LayerBuilding1->getPositionY()));
    }
    else if(POSX(m_LayerBuilding2)+WIDTH(m_LayerBuilding2) <= 0)
    {
        m_LayerBuilding2->setPosition(Vec2(m_LayerBuilding1->getPositionX() + m_LayerBuilding1->getContentSize().width  ,m_LayerBuilding2->getPositionY()));
    }
//
//    if(POSX(m_LayerBuilding3) + WIDTH(m_LayerBuilding3) <= 0)
//    {
//        m_LayerBuilding3->setPosition(Vec2(m_LayerBuilding4->getPositionX() + m_LayerBuilding4->getContentSize().width,m_LayerBuilding3->getPositionY()));
//    }
//    else if(POSX(m_LayerBuilding4) + WIDTH(m_LayerBuilding4) <= 0)
//    {
//        m_LayerBuilding4->setPosition(Vec2(m_LayerBuilding3->getPositionX() + m_LayerBuilding3->getContentSize().width  ,m_LayerBuilding4->getPositionY()));
//    }
//    
//    if(POSX(m_LayerCloud1) +WIDTH(m_LayerCloud1) <= 0)
//    {
//       m_LayerCloud1->setPosition(Vec2(m_LayerCloud2->getPositionX() + m_LayerCloud2->getContentSize().width,m_LayerCloud1->getPositionY()));
//    }
//    else if(POSX(m_LayerCloud2) + WIDTH(m_LayerCloud2) <= 0)
//    {
//        m_LayerCloud2->setPosition(Vec2(m_LayerCloud1->getPositionX() + m_LayerCloud1->getContentSize().width  ,m_LayerCloud2->getPositionY()));
//    }
}
