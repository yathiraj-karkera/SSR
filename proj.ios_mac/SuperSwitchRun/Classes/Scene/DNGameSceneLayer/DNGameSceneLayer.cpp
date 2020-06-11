//
//  DNGameSceneLayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNGameSceneLayer.hpp"
#include "cocos2d.h"
#include "DNGameScene.hpp"
#include "DNDataManager.h"
#include "DNObstecleSprite.hpp"
#include "DNCharacterSprite.hpp"
#include "DNConstants.h"
#include <spine/spine-cocos2dx.h>
#include "DNMashroomSprite.hpp"
#include "DNTutoriallayer.hpp"
#include "DNDOCharLabel.hpp"
#include "DNDOManager.hpp"
#include "DNMainMenuLayer.hpp"
#include "DNVictoryMenu.hpp"
#include "SimpleAudioEngine.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "DNAcheivementManager.hpp"
#include "PluginAdMob/PluginAdMob.h"
#include "DNGameUtility.hpp"
#include "DNDOCharLabel.hpp"

#include "audio/include/AudioEngine.h"

using namespace spine;
using namespace std;

#pragma -mark Constructor
DNGameSceneLayer::DNGameSceneLayer() {
    //Safty Remove All Listeners
    //    this->getEventDispatcher()->removeAllEventListeners();
    
    //Datamanager reference
    this->dataManager = DNDataManager::sharedManager();
    
    //Touch Listener
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan=CC_CALLBACK_2( DNGameSceneLayer::onTouchBegan, this);
    listener->onTouchEnded=CC_CALLBACK_2(DNGameSceneLayer::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    DNDataManager::sharedManager()->_isinGameScene = true;
    DNDataManager::sharedManager()->_gameLayerRef = this;
    _isStarting = true;
    UserDefault::getInstance()->setBoolForKey("shouldPauseScreenAppear",true);
    
    DNDataManager::sharedManager()->_gameSpeed = GAMEMEDIUMSPEED;
    DNDataManager::sharedManager()->_BuildingLayer1Speed = BUILDINGLAYER1SPEED;
    DNDataManager::sharedManager()->_BuildingLayer2Speed = BUILDINGLAYER2SPEED;
    DNDataManager::sharedManager()->_cloudSpeed = CLOUDSPEED;
    DNDataManager::sharedManager()->_groundSpeed = GROUNDSPEED;
    DNDataManager::sharedManager()->_isReviveAdshouldPlay = true;
    _userDialogue = NULL;
    
    _mashroom = DNMashroomSprite::create("GameScene/Mashroom.png");
    this->addChild(_mashroom,1000);
    _mashroom->setVisible(false);
    
    //Daily Reward Initialization
    if(!UserDefault::getInstance()->getBoolForKey("DailyObjectiveAlphaComplete",false))
    {
        if(!UserDefault::getInstance()->getBoolForKey("dailyalphaObjectiveChecked",false))
        {
            DNDataManager::sharedManager()->_doManager->getRandomDoType1Arr();
            UserDefault::getInstance()->setBoolForKey("dailyalphaObjectiveChecked",true);
        }
        
        _charLabel = DNDOCharLabel::createWithTTF("", "fonts/motschcc.ttf", 80);
        _charLabel->setColor(Color3B(250,167,14));
        _charLabel->setVisible(false);
        
        this->addChild(_charLabel,110);
        
        _charLabel->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.30, Vec2(0,10)),MoveBy::create(0.1, Vec2(0,-10)), NULL)));
    }
    
    if(!UserDefault::getInstance()->getBoolForKey("DailyObjectiveWallComplete",false))
    {
        if(!UserDefault::getInstance()->getBoolForKey("dailyWallObjectiveChecked",false))
        {
            DNDataManager::sharedManager()->_doManager->getType2FromArr();
            UserDefault::getInstance()->setBoolForKey("dailyWallObjectiveChecked",true);
        }
        
        int wall = UserDefault::getInstance()->getIntegerForKey("wallsPassedForDO",0);
        DNDataManager::sharedManager()->_WallPassForDO = wall;
        
        DNDataManager::sharedManager()->_colorCode = UserDefault::getInstance()->getIntegerForKey("_colorCode", 0);
        DNDataManager::sharedManager()->_totalNoOfWallPassForDO = UserDefault::getInstance()->getIntegerForKey("_totalNoOfWallPassForDO", 0);
    }
    
    {
    DNDOManager::sharedManager()->_alphabetLayer = Layer::create();
    DNDataManager::sharedManager()->_gameLayerRef->addChild(DNDOManager::sharedManager()->_alphabetLayer,100);
    DNDOManager::sharedManager()->_alphabetLayer->setPosition(Vec2(0,-200));
        
        //creating layer for pop up to show collected alphabet
         auto dummy = DNDOManager::sharedManager()->getAlphabetFromArr();
        auto visibleSize1 = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin1 = cocos2d::Director::getInstance()->getVisibleOrigin();
        
        
        float freespace = (visibleSize1.width + origin1.x);
        float spaceVal =  freespace - (DNDOManager::sharedManager()->_doType1CharArr.size() * 70);
        spaceVal =  spaceVal/2;
        
        //position
        float xPos = spaceVal + 60;
        auto yPos = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1->getPositionY() + DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1-> getContentSize().height/2;
        
       
        DNDOManager::sharedManager()->charLabelVec.clear();
        for(int i=0; i<DNDOManager::sharedManager()->_doType1CharArr.size();i++)
        {
            std::string alphabet = DNDOManager::sharedManager()->_doType1CharArr.at(i).asString();
            
            Label *charLabel = Label::createWithTTF(alphabet.c_str(), "fonts/motschcc.ttf", 80);
            DNDOManager::sharedManager()->_alphabetLayer->addChild(charLabel,6);
            charLabel->setPosition(Vec2(xPos,yPos));
            
            charLabel->setColor(Color3B(250,167,14));
            charLabel->enableOutline(Color4B(0,0,0,200),2);
            charLabel->setOpacity(0);
            xPos = xPos+70;

            DNDOManager::sharedManager()->charLabelVec.pushBack(charLabel);
        }
    }
    
    _wallCount = 0;
    
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/WallsOrder.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    _walls = rootDict["Wall"].asValueVector();
    
    //Adding keyboard Listners
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(DNGameSceneLayer::onExitButtonPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    
    //AI Implementation
    if(UserDefault::getInstance()->getIntegerForKey("deathCount",0) % 3 == 0)
    {
        int score1 = UserDefault::getInstance()->getIntegerForKey("Score1",-1);
        int score2 = UserDefault::getInstance()->getIntegerForKey("Score2",-1);
        int score3 = UserDefault::getInstance()->getIntegerForKey("Score3",-1);
        
        int averageScore = (score1 + score2 + score3)/3;
        
        
        if(averageScore > 9  &&  averageScore < 16)
        {
            UserDefault::getInstance()->setFloatForKey("gameSpeed",10);
            UserDefault::getInstance()->setIntegerForKey("maxmushroom",3);
            UserDefault::getInstance()->setIntegerForKey("wallDistance1",350);
            UserDefault::getInstance()->setIntegerForKey("wallDistance",400);
        }
        else if(averageScore > 15  &&  averageScore < 20)
        {
            UserDefault::getInstance()->setFloatForKey("gameSpeed",10.5);
            UserDefault::getInstance()->setIntegerForKey("maxmushroom",2);
            UserDefault::getInstance()->setIntegerForKey("wallDistance1",325);
            UserDefault::getInstance()->setIntegerForKey("wallDistance",375);
        }
        else if(averageScore > 20)
        {
            UserDefault::getInstance()->setFloatForKey("gameSpeed",11);
            UserDefault::getInstance()->setIntegerForKey("maxmushroom",1);
            UserDefault::getInstance()->setIntegerForKey("wallDistance1",300);
            UserDefault::getInstance()->setIntegerForKey("wallDistance",350);
        }
    }
    
    DNDataManager::sharedManager()->_gameSpeed = UserDefault::getInstance()->getFloatForKey("gameSpeed",10.5);
    _maxmushroom = UserDefault::getInstance()->getIntegerForKey("maxmushroom",2);
    _wallDistance1 = UserDefault::getInstance()->getIntegerForKey("wallDistance1",325);
    _wallDistance2 = UserDefault::getInstance()->getIntegerForKey("wallDistance",375);
    _mushroomcreated = 0;
    
    charLabel= NULL;
    
    
    
}

#pragma -mark Destructor
DNGameSceneLayer::~DNGameSceneLayer() {
    
}

#pragma -mark createGameLayer
DNGameSceneLayer* DNGameSceneLayer::createGameLayer()
{
    DNGameSceneLayer* scene = new DNGameSceneLayer();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNGameSceneLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    this->visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    this->origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    createCharacter();
    if(!DNDataManager::sharedManager()->_isReplayButtonTapped)
    {
        createBGPack();
        createMainMenuLayer();
    }
    else
    {
        replayGame();
    }
    createObstecles();
    DNDataManager::sharedManager()->_isPauseButtoncreated = false;
    
    return true;
}

#pragma -mark createCharacter
void DNGameSceneLayer::createCharacter()
{
    
    auto m_LayerGround = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    
    //Creating Charcter Mask
    _character = cocos2d::Sprite::create("GameScene/Character_Mask.png");
    _character->setAnchorPoint(Vec2(0,0));
    _character->setTag(0);
    _character->setPosition(cocos2d::Vec2(m_LayerGround->getContentSize().width * 0.15 +_character->getContentSize().width/2 ,(origin.y +  m_LayerGround->getContentSize().height * 0.80)));
    this->addChild(_character, 1000);
    
    //Creating Charcter Animation
    auto character = UserDefault::getInstance()->getStringForKey("character", "GameScene/Animation/Character/char2/Character2");
    
    //Tesing new character
    // character = "GameScene/Animation/Character/char3/Character3";
    
    characterRunAnimation = spine::SkeletonAnimation::createWithBinaryFile(character +".skel", character +".atlas", 0.5f);
    characterRunAnimation->setScale(0.60);
    characterRunAnimation->setAnchorPoint(Vec2(0,0));
    characterRunAnimation->setPosition(Vec2(_character->getContentSize().width/2,15));
    _character->addChild(characterRunAnimation,100);
    
    //sheild creation
    _sheild = Sprite::create("GameScene/Shield.png");
    _sheild->setPosition(Vec2(_character->getContentSize().width/2,_character->getContentSize().height * 0.50));
    _character->addChild(_sheild,characterRunAnimation->getLocalZOrder() + 1);
    
    _sheild->setVisible(false);
    
    //Setting Random Charcter Animation
    int rand = random(1, 3);
    
    switch (rand) {
        case 1:
            characterRunAnimation->setAnimation(1, "Red_Idle", true);
            _character->setTag(1);
            break;
        case 2:
            characterRunAnimation->setAnimation(1, "Blue_Idle", true);
            _character->setTag(3);
            rand = 3;
            
            break;
        case 3:
            characterRunAnimation->setAnimation(1, "Blue_Idle", true);
            _character->setTag(3);
            break;
            
        default:
            break;
    }
    
    DNDataManager::sharedManager()->buttonNumber = rand;
    /******************* Testing Unlock tutorial *****************/
    
    //    UserDefault::getInstance()->setBoolForKey("TutorialPlayed", false);
    //    UserDefault::getInstance()->setBoolForKey("MashroomTutorial", false);
    //
    /*******************************************************************/
    
    //initializing tutoria values
    if(!dataManager->getTutorialPlayed())
    {
        dataManager->setMashroomTutorialPlayed(false);
        characterRunAnimation->setAnimation(1, "Red_Idle", true);
        _character->setTag(1);
        DNDataManager::sharedManager()->_gameSpeed = 6;
        DNDataManager::sharedManager()->buttonNumber = 1;
    }
    else{
        dataManager->_gameSpeed = GAMEMEDIUMSPEED;
    }
}

#pragma -mark createBGPack
void DNGameSceneLayer::createBGPack()
{
    _bgPackLayer = DNBgPackLayer::createBGPackLayer();
    this->addChild(_bgPackLayer,99);
    
}
#pragma -mark removeBGPack
void DNGameSceneLayer::removeBGPack()
{
    _bgPackLayer->removeAllChildrenWithCleanup(true);
    this->removeChild(_bgPackLayer);
    
}

#pragma -mark createObstecles
void DNGameSceneLayer::createObstecles()
{
    wallVector.clear();
    obstecleVector.clear();
    DNObstecleSprite *Obstecle;
    auto m_LayerGround = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    
    //creating tutorial for tutorial
    if(!DNDataManager::sharedManager()->getTutorialPlayed())
    {
        DNObstecleSprite *Obstecle1 = DNObstecleSprite::create("GameScene/LightMask.png");
        Obstecle1->setTag(3);
        spine::SkeletonAnimation* wall1 = spine::SkeletonAnimation::createWithBinaryFile("GameScene/Animation/Wall/wall.skel", "GameScene/Animation/Wall/wall.atlas", 0.25f);
        wall1->setAnimation(1, "Blue", true);
        wall1->setPosition(Vec2(Obstecle1->getContentSize().width/2 + 190  ,90));
        Obstecle1->addChild(wall1,100);
        tutorialVector.pushBack(Obstecle1);
        Obstecle1->setAnchorPoint(Vec2(0,0));
        this->addChild(Obstecle1);
        
        DNObstecleSprite *Obstecle2 = DNObstecleSprite::create("GameScene/LightMask.png");
        Obstecle2->setTag(1);
        spine::SkeletonAnimation* wall2 = spine::SkeletonAnimation::createWithBinaryFile("GameScene/Animation/Wall/wall.skel", "GameScene/Animation/Wall/wall.atlas", 0.25f);
        wall2->setAnimation(1, "Red", true);
        wall2->setPosition(Vec2(Obstecle2->getContentSize().width/2 + 190  ,90));
        Obstecle2->addChild(wall2,100);
        tutorialVector.pushBack(Obstecle2);
        Obstecle2->setAnchorPoint(Vec2(0,0));
        this->addChild(Obstecle2);
        
        for(int i=0;i<2;i++)
        {
            tutorialVector.at(i)->isScoreSet = true;
            
            tutorialVector.at(i)->isScoreDeducted = false ;
            tutorialVector.at(i)->setPosition(cocos2d::Vec2((m_LayerGround->getContentSize().width * 0.80) + (i * 400),m_LayerGround->getPositionY() + m_LayerGround->getContentSize().height * 0.75));
        }
        
        createMashroom();
        isMashroomcreated = true;
        _mashroom->setPosition(cocos2d::Vec2(tutorialVector.at(0)->getPositionX() + m_LayerGround->getContentSize().width/4 ,origin.y + m_LayerGround->getContentSize().height));
    }
    
    //taking walls details from plist for first few walls
    int randomWalls = random(0, 4);
    std::string text =   _walls.at(randomWalls).asString();
    
    std::istringstream ss(text);
    std::string token;
    
    _doType1CharArr.clear();
    
    while(std::getline(ss, token, ','))
    {
        _doType1CharArr.push_back(Value(token));
    }
    
    //creating walls
    for(int i = 0;i < 6;i++)
    {
        int rand = _doType1CharArr.at(_index).asInt();
        _index++;
        spine::SkeletonAnimation* wall = spine::SkeletonAnimation::createWithBinaryFile("GameScene/Animation/Wall/wall.skel", "GameScene/Animation/Wall/wall.atlas", 0.325f);
        
        wallVector.pushBack(wall);
        
        switch (rand) {
            case 1:
                Obstecle = DNObstecleSprite::create("GameScene/LightMask.png");
                Obstecle->setScale(0.25, 1);
                wall->setAnchorPoint(Vec2(0.5,0));
                wall->setAnimation(1, "Red", true);
                wall->setPosition(Vec2(Obstecle->getContentSize().width/2 + 300  ,100));
                wall->setContentSize(Size(Obstecle->getContentSize().width ,wall->getContentSize().height));
                
                
                Obstecle->addChild(wall,100);
                Obstecle->setTag(1);
                
                break;
            case 2:
                Obstecle = DNObstecleSprite::create("GameScene/LightMask.png");
                Obstecle->setScale(0.25, 1);
                wall->setAnchorPoint(Vec2(0.5,0));
                wall->setAnimation(1, "Green", true);
                wall->setPosition(Vec2(Obstecle->getContentSize().width/2 + 300 ,100));
                wall->setContentSize(Size(Obstecle->getContentSize().width ,wall->getContentSize().height));
                
                
                Obstecle->addChild(wall,100);
                Obstecle->setTag(2);
                break;
            case 3:
                Obstecle = DNObstecleSprite::create("GameScene/LightMask.png");
                wall->setAnchorPoint(Vec2(0.5,0));
                wall->setPosition(Vec2(Obstecle->getContentSize().width/2 + 300  ,100));
                wall->setContentSize(Size(Obstecle->getContentSize().width ,wall->getContentSize().height));
                wall->setAnimation(1, "Blue", true);
                
                Obstecle->setScale(0.25, 1);
                Obstecle->addChild(wall,100);
                Obstecle->setTag(3);
                break;
                
            default:
                break;
        }
        
        Obstecle->setAnchorPoint(Vec2(0,0));
        this->addChild(Obstecle,100);
        obstecleVector.pushBack(Obstecle);
    }
    // positioning the walls
    for(int i = 0;i < 6;i++)
    {
        auto currentObstacle = obstecleVector.at(i);
        currentObstacle->isScoreSet = true;
        currentObstacle->setScale(0.75);
        currentObstacle->isScoreDeducted = false ;
        currentObstacle->setPosition(cocos2d::Vec2((this->getContentSize().width * 2 * 0.75 ) + (i * 400),m_LayerGround->getPositionY() + m_LayerGround->getContentSize().height * 0.75));
    }
}

#pragma -mark createRevivemenu
void DNGameSceneLayer::createReviveMenu()
{
    if(!DNDataManager::sharedManager()->_isReviveButtoncreated && DNDataManager::sharedManager()->_isReviveAdshouldPlay)
    {
        DNDataManager::sharedManager()->_isReviveButtoncreated = true;
        DNDataManager::sharedManager()->_isinGameScene = false;
        _reviveMenu = DNReviveMenu::createReviveMenu();
        _reviveMenu->fadeInBackground();
        this->addChild(_reviveMenu,1000);
    }
    else
    {
        _reviveMenu = NULL;
        createVictoryScren();
    }
}

#pragma -mark removeReviveMenu
void DNGameSceneLayer::removeReviveMenu()
{
    if(DNDataManager::sharedManager()->_isAdWatched)
    {
        _reviveMenu->fadeOutbackground();
        DNDataManager::sharedManager()->_isReviveButtoncreated = false;
        DNDataManager::sharedManager()->_isAdWatched = false;
        DNDataManager::sharedManager()->_gameSceneRef->showHUDLayer();
        DNDataManager::sharedManager()->_isvictoryScreenCreated = false;
        _reviveMenu->fadeOutbackground();
        this->removeChild(_reviveMenu);
        setGamePositionAfterRevive();
        if(DNDataManager::sharedManager()->_score >50)
        {
            DNDataManager::sharedManager()->_HUDLayerRef->menu->setEnabled(true);
            DNDataManager::sharedManager()->_HUDLayerRef->menu->setVisible(true);
        }
        startCountDown();
    }
    else if(!DNDataManager::sharedManager()->_isvictoryScreenCreated)
    {
        DNDataManager::sharedManager()->_isReviveButtoncreated = false;
        DNDataManager::sharedManager()->_HUDLayerRef->_isTapAllowed = true;
        _reviveMenu->hideReviveMenu();
        CallFunc* calSelector = CallFunc::create(this, callfunc_selector(DNGameSceneLayer::createVictoryScren));
        this->runAction(Sequence::create(DelayTime::create(0.31),calSelector, NULL));
    }
}

#pragma mark - setGamePositionAfterRevive
void DNGameSceneLayer::setGamePositionAfterRevive()
{
    DNDataManager::sharedManager()->_gameLayerRef->_character->setPositionX(300);
    for(int i = 0;i < 6;i++)
    {
        obstecleVector.at(i)->isScoreSet = true;
        obstecleVector.at(i)->setScale(0.75);
        obstecleVector.at(i)->isScoreDeducted = false ;
        obstecleVector.at(i)->setPositionX((this->getContentSize().width * 1 * 0.75 ) + (i * 300));
    }
}

#pragma mark - timer
void DNGameSceneLayer::timer(float d)
{
    _countvalue--;
    _countDown->setString(StringUtils::format("%s",StringUtils::toString(_countvalue).c_str()));
    
    if(_countvalue == 0)
    {
        _countvalue = 3;
        DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
        DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(true);
        DNDataManager::sharedManager()->_HUDLayerRef->_isTapAllowed = true;
        DNDataManager::sharedManager()->_isinGameScene = true;
        DNDataManager::sharedManager()->isgreenButtonTutorial = false;
        _countDown->setVisible(false);
        this->removeChild(_countDown);
        this->unschedule(schedule_selector(DNGameSceneLayer::timer));
        DNDataManager::sharedManager()->_gameSceneRef->resumeGame();
    }
}

#pragma -mark createVictoryScren
void DNGameSceneLayer::createVictoryScren()
{
    
    DNDataManager::sharedManager()->_isvictoryScreenCreated = true;
    DNDataManager::sharedManager()->_isinGameScene = false;
    _victoryMenu = DNVictoryMenu::createVictoryMenu();
    this->addChild(_victoryMenu,999999991);
    _victoryMenu->showVictoryMenu();
}

#pragma -mark removeVictoryScren
void DNGameSceneLayer::removeVictoryScren()
{
    DNDataManager::sharedManager()->_isvictoryScreenCreated = false;
    
    _victoryMenu->hideVictoryMenu();
    if(!(_reviveMenu == NULL))
    {
        this->removeChild(_reviveMenu);
    }
    
    this->removeChild(_victoryMenu);
    
    auto scene = DNGameScene::createScene();
    cocos2d::TransitionFade* transition = cocos2d::TransitionFade::create(0.05, scene);
    Director::getInstance()->replaceScene(transition);
}

#pragma -mark update
//void DNGameSceneLayer::fixedUpdate(float dt)
void DNGameSceneLayer::update(float dt)
{
    auto dtSpeed = 1;// dt * GAME_DT_DIVIDER;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    
//    dtSpeed = dt * GAME_DT_DIVIDER;
//    
//#endif
    //Removed Background Schedule update and called from here
    dataManager->_backGroundLayerRef->bgUpdate(dtSpeed);
    
    moveObstecles();
    
    this->updateTutorialsActions(dtSpeed);
    this->updateMashroomMovements(dtSpeed);
    this->updateObstacleMovments(dtSpeed);
    
    
    int rand = random(1,1000);
    //Mushroom update and creation
    if(isMashroomcreated)
    {
        this->updateMushroomPositionAndCheckCollision(dtSpeed);
    }
    else if(rand == 123 && dataManager->_mashroomCount == 0 && (_mushroomcreated < _maxmushroom) )
    {
        createMashroom();
        isMashroomcreated = true;
    }
    
    
    //alphabet update and creation
    if(isAlphabetCreated)
    {
        if(this->isBothAreCollided(_character, _charLabel))
        {
            
            DNDataManager::sharedManager()->play2dSound("Sound/Mashroom.mp3");
            
            DNDOManager::sharedManager()->onAlphabetCollision(_charLabel);
            isAlphabetCreated =false;
            
            UserDefault::getInstance()->setIntegerForKey("AlphaCount",UserDefault::getInstance()->getIntegerForKey("AlphaCount",0)+1);
            return;
        }
        else
        {
            _charLabel->setPosition(POSX(_charLabel) - dataManager->_gameSpeed * dtSpeed,POSY(_charLabel));
        }
        
        
        
        
        
    }
    else if(rand == 499 && !UserDefault::getInstance()->getBoolForKey("DailyObjectiveAlphaComplete",false))
    {
        if(dataManager->getTutorialPlayed())
        {
            isAlphabetCreated = true;
            
            DNDOManager::sharedManager()->addingAlphabet();
            
            _charLabel->setVisible(true);
            auto layGround = dataManager->_backGroundLayerRef->m_LayerGround1;
            
            _charLabel->setPosition(Vec2(DNDataManager::sharedManager()->alphaPosX ,POSY(layGround) + HEIGHT(layGround) * 0.80));
            
            //for(int j = 0;j < 6 ; j++)
            //{
            //    if(this->isBothAreCollided(obstecleVector.at(j), _charLabel))
            //    {
            //        _charLabel->setPositionX(obstecleVector.at(j)->getPositionX() + 150);
            //        break;
            //    }
            //}
            
            
            
        }
    }
    
}

#pragma -mark update
void DNGameSceneLayer::doDeathAction()
{
    DNDataManager::sharedManager()->_isinGameScene = false;
    UserDefault::getInstance()->setBoolForKey("shouldPauseScreenAppear",false);
    int _deathCount = ( UserDefault::getInstance()->getIntegerForKey("deathCount",0)+ 1);
    UserDefault::getInstance()->setIntegerForKey("deathCount", _deathCount);
    
    
    DNDataManager::sharedManager()->play2dSound("Sound/death.mp3");
    int remainder = _deathCount % 3;
    
    //  updating AI Data
    switch (remainder) {
        case 1:
            UserDefault::getInstance()->setIntegerForKey("Score1",_wallCount);
            break;
        case 2:
            UserDefault::getInstance()->setIntegerForKey("Score2",_wallCount);
            break;
        case 3:
            UserDefault::getInstance()->setIntegerForKey("Score3",_wallCount);
            break;
        default:
            break;
    }
    
    if(_deathCount == 1000)
    {
        DNAcheivementManager::sharedManager()->AcheivementXp(40,"Noob");
        //        if(DNGameUtility::checkIsNetworkAvailable())
        //        {
        //            sdkbox::PluginSdkboxPlay::unlockAchievement("Noob");
        //        }
    }
    
    dataManager->_gameSceneRef->hideHUDLayer();
    dataManager->_isinGameScene = false;
    dataManager->_HUDLayerRef->menu->setEnabled(false);
    dataManager->_HUDLayerRef->menu->setVisible(false);
    
    //death animation
    switch (dataManager->buttonNumber) {
        case 1:
            characterRunAnimation->setAnimation(1, "Red_Death", false);
            break;
        case 2:
            characterRunAnimation->setAnimation(1, "Green_Death", false);
            break;
        case 3:
            characterRunAnimation->setAnimation(1, "Blue_Death", false);
            break;
        default:
            break;
    }
    
    dataManager->_HUDLayerRef->_isTapAllowed = false;
    unscheduleUpdate();
    dataManager->_isinGameScene = false;
    CallFunc* calSelector = CallFunc::create(this, callfunc_selector(DNGameSceneLayer::createReviveMenu));
    this->runAction(Sequence::create(DelayTime::create(2),calSelector, NULL));
}

#pragma -mark updateObstacleMovments
void DNGameSceneLayer::updateObstacleMovments(float dtSpeed)
{
    bool objectCollided = false;
    for(int i = 0; i < 6 ; i++)
    {
        auto currentObstacle = obstecleVector.at(i);
        
        //obstecle movement
        currentObstacle->setPosition(Vec2(currentObstacle->getPositionX() - dataManager->_gameSpeed*dtSpeed ,currentObstacle->getPositionY()));
        
        if (objectCollided == false && this->isBothAreCollided(_character, currentObstacle))
        {
            objectCollided = true;
            //score  updation
            if(_character->getTag() ==  currentObstacle->getTag() && currentObstacle->isScoreSet)
            {
                this->doScoreUpdation(currentObstacle);
            }
            
            else if(_character->getTag() !=  currentObstacle->getTag()  && !currentObstacle->isScoreDeducted)
            {
                //checking death condition
                if(dataManager->_mashroomCount > 0)
                {
                    //checking sheild
                    currentObstacle->isScoreDeducted = true;
                    dataManager->_mashroomCount--;
                    dataManager->_HUDLayerRef->mashroomBG->setVisible(false);
                    if(dataManager->_mashroomCount == 0)
                    {
                        
                        DNDataManager::sharedManager()->play2dSound("Sound/Shield_break_sound.mp3");
                        _sheild->setVisible(false);
                        
                    }
                    
                }
                else
                {
                    this->doDeathAction();
                }
            }
        }
        
        if(currentObstacle->getPositionX() - _character->getPositionX() <=  100 )
        {
            
            dataManager->_isinGameScene = false;
        }
        else{
            
            dataManager->_isinGameScene = true;
        }
    }
}

#pragma -mark doScoreUpdation
void DNGameSceneLayer::doScoreUpdation(DNObstecleSprite *currentObstacle)
{
    int score =   dataManager->_score;
    score = score + UserDefault::getInstance()->getIntegerForKey("Points",1);
    
    dataManager->_score = score;
    dataManager->_HUDLayerRef->score->setString(StringUtils::toString(score));
    
    _wallCount++;
    auto _totalWallPassed = UserDefault::getInstance()->getIntegerForKey("Totalwallpassed",0) + 1;
    UserDefault::getInstance()->setIntegerForKey("Totalwallpassed", _totalWallPassed);
    
    if(_wallCount % 10 == 0)
    {
        
        
        DNDataManager::sharedManager()->play2dSound("Sound/score.mp3");
    }
    
    //Acheivements
    switch (score) {
        case 500:
            DNAcheivementManager::sharedManager()->AcheivementXp(50,"Reckoning");
            if(!DNAcheivementManager::sharedManager()->isBGUnlocked(3))
            {
                UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount",0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackBg(3);
            break;
        case 1000:
            DNAcheivementManager::sharedManager()->AcheivementXp(80,"Top Dog");
            
            break;
            
        default:
            break;
    }
    
    //game speed updation for the first time of game play
    if(dataManager->_gameSpeed < 10.5 && UserDefault::getInstance()->getIntegerForKey("deathCount",0) == 0)
    {
        dataManager->_gameSpeed += 0.5;
    }
    //    CCLOG("game speed %f",dataManager->_gameSpeed);
    if(_wallCount == 300 ){
        
        dataManager->_HUDLayerRef->addGreenButton();
        
        dataManager->_HUDLayerRef->_greenButton->runAction(Sequence::create(ScaleTo::create(0.30,0.80),ScaleTo::create(0.30,1),ScaleTo::create(0.30,0.80),ScaleTo::create(0.30,1),NULL));
        
        // creating tutorial
        if(!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial",false))
        {
            
            setIdleAnimation();
            DNDataManager::sharedManager()->_isinGameScene = false;
            DNDataManager::sharedManager()->isgreenButtonTutorial = true;
            dataManager->_HUDLayerRef->_pauseButton->setEnabled(false);
            unscheduleUpdate();
            DNDataManager::sharedManager()->_backGroundLayerRef->unscheduleUpdate();
            
            _tutorialLayer = DNTutoriallayer::createTutorialLayer();
            DNDataManager::sharedManager()->_isGreenbuttonIntroduced =true;
            this->addChild(_tutorialLayer,120);
            _tutorialLayer->fadeInBackground();
            _tutorialLayer->tutorialForGreenButtonIntrduce();
            DNDataManager::sharedManager()->_HUDLayerRef->_isTapAllowed = false;
        }
        else
        {
            setIdleAnimation();
            DNDataManager::sharedManager()->_isinGameScene = false;
            dataManager->_HUDLayerRef->_pauseButton->setEnabled(false);
            unscheduleUpdate();
            DNDataManager::sharedManager()->_backGroundLayerRef->unscheduleUpdate();
            startCountDown();
        }
        
        
        
        
    }
    // updating game play hard level
    if(_wallCount % 50 == 0 && _wallCount < 201)
    {
        dataManager->_gameSpeed += 0.5;
    }
    
    if(!UserDefault::getInstance()->getBoolForKey("DailyObjectiveWallComplete",false))
    {
        if(dataManager->_colorCode == 0 && (dataManager->_WallPassForDO < dataManager->_totalNoOfWallPassForDO))
        {
            dataManager->_WallPassForDO++;
            UserDefault::getInstance()->setIntegerForKey("wallsPassedForDO",dataManager->_WallPassForDO);
        }
        else if(dataManager->_colorCode == 1 && currentObstacle->getTag() == 1 && (dataManager->_WallPassForDO < dataManager->_totalNoOfWallPassForDO))
        {
            dataManager->_WallPassForDO++;
            UserDefault::getInstance()->setIntegerForKey("wallsPassedForDO",dataManager->_WallPassForDO);
        }
        else if(dataManager->_colorCode == 2 && currentObstacle->getTag() == 2 && (dataManager->_WallPassForDO < dataManager->_totalNoOfWallPassForDO))
        {
            dataManager->_WallPassForDO++;
            UserDefault::getInstance()->setIntegerForKey("wallsPassedForDO",dataManager->_WallPassForDO);
        }
        else if(dataManager->_colorCode == 3 && currentObstacle->getTag() == 3 && (dataManager->_WallPassForDO < dataManager->_totalNoOfWallPassForDO))
        {
            dataManager->_WallPassForDO++;
            UserDefault::getInstance()->setIntegerForKey("wallsPassedForDO",dataManager->_WallPassForDO);
        }
        
        if(dataManager->_totalNoOfWallPassForDO ==  dataManager->_WallPassForDO)
        {
            dataManager->_doManager->onType2ObjectiveSuccessfull(true);
        }
    }
    
    //Acheivement
    
    switch (_totalWallPassed) {
        case 50:
            DNAcheivementManager::sharedManager()->AcheivementXp(20,"Crawler");
            
            break;
        case 2000:
            DNAcheivementManager::sharedManager()->AcheivementXp(40,"Star");
            if(!DNAcheivementManager::sharedManager()->isBGUnlocked(5))
            {
                UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount",0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackBg(5);
            break;
        case 5000:
            DNAcheivementManager::sharedManager()->AcheivementXp(80,"Super_Star");
            if(!DNAcheivementManager::sharedManager()->isCharacterUnlocked(4))
            {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount",0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(4);
            break;
        case 10000:
            DNAcheivementManager::sharedManager()->AcheivementXp(200,"Inhuman");
            break;
            
        default:
            break;
    }
    
    
    if(_wallCount % 10 == 0)
    {
        updateLevelBar();
    }
    
    currentObstacle->isScoreSet = false;
}

#pragma -mark updateMushroomPositionAndCheckCollision
void DNGameSceneLayer::updateMushroomPositionAndCheckCollision(float dtSpeed)
{
    
    if(_character->getBoundingBox().intersectsRect(_mashroom->getBoundingBox()))
    {
        
        _sheild->setVisible(true);
        
        UserDefault::getInstance()->setIntegerForKey("1Up",UserDefault::getInstance()->getIntegerForKey("1Up",0) + 1);
        
        DNDataManager::sharedManager()->play2dSound("Sound/Mashroom.mp3");
        DNDataManager::sharedManager()->_mashroomCount +=2;
        _mashroom->setVisible(false);
        isMashroomcreated = false;
        DNDataManager::sharedManager()->_HUDLayerRef->mashroomBG->setVisible(true);
        
        //Achievements : unlock achievement Collect 1ups
        
        int powerUp = UserDefault::getInstance()->getIntegerForKey("1Up",0);
        switch (powerUp) {
            case 1:
                DNAcheivementManager::sharedManager()->AcheivementXp(20,"Numero_uno");
                break;
            case 10:
                DNAcheivementManager::sharedManager()->AcheivementXp(20,"Novice");
                break;
            case 100:
                
                DNAcheivementManager::sharedManager()->AcheivementXp(30,"Seeker");
                break;
            case 500:
                DNAcheivementManager::sharedManager()->AcheivementXp(50,"Collector");
                break;
                
            default:
                break;
        }
        
    }
    else{
        _mashroom->setPosition(_mashroom->getPositionX() - DNDataManager::sharedManager()->_gameSpeed * dtSpeed,_mashroom->getPositionY());
    }
}

#pragma -mark isBothAreCollided
bool DNGameSceneLayer::isBothAreCollided(Node *objA, Node *objB)
{
    if(this->getDistanceBetweenFloat(POSX(objA), POSX(objB)) < (WIDTH(objA) + WIDTH(objB))/2 && objA->getBoundingBox().intersectsRect(objB->getBoundingBox()))
    {
        return true;
    }
    return false;
}

#pragma -mark updateTutorialsActions
void DNGameSceneLayer::updateTutorialsActions(float dtSpeed)
{
    if(dataManager->getTutorialPlayed() == false)
    {
        for(int i = 0; i<2; i++)
        {
            if((tutorialVector.at(0)->getPositionX()  <= _character->getPositionX() + 200) && !_tutorialCreated)
            {
                setIdleAnimation();
                
                dataManager->_HUDLayerRef->_pauseButton->setEnabled(false);
                unscheduleUpdate();
                
                
                _tutorialLayer = DNTutoriallayer::createTutorialLayer();
                this->addChild(_tutorialLayer,120);
                _tutorialLayer->fadeInBackground();
                _tutorialLayer->tutorialForCaharacterColor();
                _tutorialCreated = true;
            }
            
            tutorialVector.at(i)->setPosition(Vec2((tutorialVector.at(i)->getPositionX() - dataManager->_gameSpeed * dtSpeed),tutorialVector.at(i)->getPositionY()));
            if (_character->getBoundingBox().intersectsRect(tutorialVector.at(i)->getBoundingBox()))
            {
                
                if(_character->getTag() !=  tutorialVector.at(i)->getTag()  && !tutorialVector.at(i)->isScoreDeducted)
                {
                    if(dataManager->_mashroomCount > 0)
                    {
                        
                        tutorialVector.at(i)->isScoreDeducted = true;
                        dataManager->_mashroomCount--;
                        dataManager->_HUDLayerRef->mashroomBG->setVisible(false);
                        if(dataManager->_mashroomCount == 0)
                        {
                            
                            DNDataManager::sharedManager()->play2dSound("Sound/Shield_break_sound.mp3");
                            _sheild->setVisible(false);
                        }
                    }
                    else{
                        dataManager->_gameSceneRef->hideHUDLayer();
                        
                        DNDataManager::sharedManager()->play2dSound("Sound/death.mp3");
                        switch (dataManager->buttonNumber)
                        {
                            case 1:
                                characterRunAnimation->setAnimation(1, "Red_Death", false);
                                break;
                            case 2:
                                characterRunAnimation->setAnimation(1, "Green_Death", false);
                                break;
                            case 3:
                                characterRunAnimation->setAnimation(1, "Blue_Death", false);
                                break;
                                
                            default:
                                break;
                                
                        }
                        
                        dataManager->_HUDLayerRef->_isTapAllowed = false;
                        unscheduleUpdate();
                        
                        UserDefault::getInstance()->setIntegerForKey("XP",UserDefault::getInstance()->getIntegerForKey("XP",0) + dataManager->_score * 2);
                        int xp =UserDefault::getInstance()->getIntegerForKey("XP",0);
                        dataManager->_HUDLayerRef->XPLabel->setString("XP : " +StringUtils::toString(xp));
                        
                        CallFunc* calSelector = CallFunc::create(this, callfunc_selector(DNGameSceneLayer::createReviveMenu));
                        this->runAction(Sequence::create(DelayTime::create(2),calSelector, NULL));
                    }
                }
                else if(tutorialVector.at(i)->isScoreSet){
                    int score =   dataManager->_score;
                    score = score + UserDefault::getInstance()->getIntegerForKey("Points",1);
                    
                    if(_wallCount % 15 == 0)
                    {
                        dataManager->_gameSpeed += 0.5;
                    }
                    
                    if(dataManager->_gameSpeed < 10.5)
                    {
                        dataManager->_gameSpeed += 0.50;
                    }
                    
                    dataManager->_score = score;
                    dataManager->_HUDLayerRef->score->setString(StringUtils::toString(score));
                    tutorialVector.at(i)->isScoreSet = false;
                }
                
            }
            if(tutorialVector.at(i)->getPositionX() <= 0)
            {
                if(i == 1)
                {
                    dataManager->setTutorialPlayed(true);
                }
                this->removeChild(tutorialVector.at(i));
                tutorialVector.at(i)->isScoreDeducted = false;
                tutorialVector.at(i)->isScoreSet = true;
            }
        }
    }
}

#pragma -mark updateMashroomMovements
void DNGameSceneLayer::updateMashroomMovements(float dtSpeed)
{
    if(dataManager->getMashroomTutorialPlayed() == false)
    {
        if((_mashroom->getPositionX()  <= _character->getPositionX() + 150 && !_mashroomTutorialCreated))
        {
            setIdleAnimation();
            
            unscheduleUpdate();
            //            unschedule(schedule_selector(DNGameSceneLayer::fixedUpdate));
            _tutorialLayer = DNTutoriallayer::createTutorialLayer();
            this->addChild(_tutorialLayer,120);
            _tutorialLayer->fadeInBackground();
            _tutorialLayer->tutorialForMashroom();
            _mashroomTutorialCreated = true;
            DNDataManager::sharedManager()->_HUDLayerRef->mashroomBG->setVisible(false);
        }
    }
}

#pragma -mark moveObstecles
void DNGameSceneLayer::moveObstecles()
{
    int rand;
    float distance = 400;
    for(int j = 0; j<6 ; j++)
    {
        auto obstacle = obstecleVector.at(j);
        if(POSX(obstacle) + WIDTH(obstacle)  <= 0)
        {
            if(_index < _doType1CharArr.size())
            {
                rand = _doType1CharArr.at(_index).asInt();
                _index++;
                if(rand == 2)
                {
                    rand = 3;
                    distance = _wallDistance1;
                }
            }
            else
            {
                rand = random(1, 3);
                if(rand == 1)
                {
                    distance = _wallDistance2;
                }
                else if(rand == 2)
                {
                    distance = _wallDistance1;
                }
            }
            
            obstacle->isScoreDeducted = false;
            obstacle->isScoreSet = true;
            
            if(_wallCount < 301)
            {
                if(rand == 2 )
                {
                    rand = 3;
                }
            }
            
            
            if(rand == 1 && redWallCount == 2)
            {
                
                rand = 2;
                redWallCount = 0;
                blueWallCount = 0;
            }
            else if(rand == 2 && greenWallCount == 2)
            {
                rand = 3;
                greenWallCount = 0;
                redWallCount = 0;
            }
            else if(rand == 3 && blueWallCount == 2)
            {
                rand = 1;
                greenWallCount = 0;
                blueWallCount = 0;
            }
            
            if(_wallCount < 301)
            {
                if(rand == 2 )
                {
                    rand = 3;
                }
            }
            
            
            switch (rand) {
                case 1:
                    wallVector.at(j)->setAnimation(1, "Red", true);
                    obstacle->setTag(1);
                    redWallCount++;
                    greenWallCount = 0;
                    blueWallCount = 0;
                    break;
                case 2:
                    wallVector.at(j)->setAnimation(1, "Green", true);
                    obstacle->setTag(2);
                    redWallCount = 0;
                    greenWallCount++;
                    blueWallCount = 0;
                    break;
                case 3:
                    wallVector.at(j)->setAnimation(1, "Blue", true);
                    obstacle->setTag(3);
                    redWallCount = 0;
                    greenWallCount = 0;
                    blueWallCount++;
                    break;
                    
                default:
                    break;
            }
            
            
            
            int k = j - 1;
            if(j == 0 )
            {
                k = 5;
            }
            _previousColor = rand;
            obstacle->setPosition(Vec2(obstecleVector.at(k)->getPositionX() + distance,obstacle->getPositionY()));
            k--;
            if(k < 0)
            {
                k = 5;
            }
            DNDataManager::sharedManager()->alphaPosX = (obstecleVector.at(k)->getPositionX() + distance/2);
        }
    }
}

#pragma -mark updateLevelBar
void DNGameSceneLayer::updateLevelBar()
{
    
    //Just commanded for checking lack issue
    DNDataManager::sharedManager()->_HUDLayerRef->loadLevelDetails();
    
    int xpValue= UserDefault::getInstance()->getIntegerForKey("XP",0) +  UserDefault::getInstance()->getIntegerForKey("XPAwarded",1);
    UserDefault::getInstance()->setIntegerForKey("XP",xpValue);
    
    
    if(UserDefault::getInstance()->getIntegerForKey("Level", 0) == 9)
    {
        DNDataManager::sharedManager()->_HUDLayerRef->XPLabel->setString(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("XP", 0)));
    }
    else
    {
        DNDataManager::sharedManager()->_HUDLayerRef->XPLabel->setString(StringUtils::toString(xpValue) + " / " + StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("MaxXp", 0)));
    }
    DNDataManager::sharedManager()->_HUDLayerRef->levelLabel->setString(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("Level", 0) + 1));
    float previouslevelxp =  UserDefault::getInstance()->getIntegerForKey("PreviousLevelXp",0);
    float xp1 = xpValue;
    float maxxp = UserDefault::getInstance()->getIntegerForKey("MaxXp", 0);
    float percent = ((xp1 - previouslevelxp)/(maxxp - previouslevelxp)) * 100  ;
    
    DNDataManager::sharedManager()->_HUDLayerRef->_levelProgresSlider->setPercentage(percent);
}

#pragma mark - createMashroom
void DNGameSceneLayer::createMashroom()
{
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    _mashroom->setVisible(true);
    _mushroomcreated++;
    auto m_LayerGround = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    //    _mashroom->setPosition(cocos2d::Vec2(m_LayerGround->getContentSize().width + _character->getContentSize().width/2  ,m_LayerGround->getPositionY() + m_LayerGround->getContentSize().height * 0.85));
    _mashroom->setPosition(Vec2(DNDataManager::sharedManager()->alphaPosX ,POSY(m_LayerGround) + HEIGHT(m_LayerGround) * 0.80));
    
    
}

//handling Touch
#pragma mark - Touch_events
bool DNGameSceneLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}


void DNGameSceneLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(_mashroomTutorialCreated)
    {
        dataManager->setMashroomTutorialPlayed(true);
        _mashroomTutorialCreated = false;
        removeTutorialLayer();
        
        //calling run animation
        setRunAnimation();
        
        DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
        DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(true);
    }
}

#pragma mark - removeTutorialLayer
void  DNGameSceneLayer::removeTutorialLayer()
{
    _tutorialLayer->removeChild(_tutorialLayer->_fadeLayer);
    this->removeChild(_tutorialLayer);
    if(!DNDataManager::sharedManager()->_isGreenbuttonIntroduced)
    {
        scheduleUpdate();
        DNDataManager::sharedManager()->_backGroundLayerRef->scheduleUpdate();
    }
    
}

#pragma mark - createMainMenuLayer
void DNGameSceneLayer::createMainMenuLayer()
{
    _mainMenu  = DNMainMenuLayer::createMeinMenu();
    this->addChild(_mainMenu,999999910);
}

#pragma mark - createCharacterPack
void DNGameSceneLayer::createCharacterPack()
{
    if(!DNDataManager::sharedManager()->_isCharcterPackMenuCreated)
    {
        DNDataManager::sharedManager()->_isCharcterPackMenuCreated = true;
        _characterPack = DNCharacterPack::createCharacterPackLayer();
        this->addChild(_characterPack,999999950);
        _characterPack->showCharacterPackMenu();
    }
}

#pragma mark - removeCharacterPack
void DNGameSceneLayer::removeCharacterPack()
{
    this->removeChild(_characterPack);
    DNDataManager::sharedManager()->_isCharcterPackMenuCreated = false;
}

#pragma mark - removeMenuLayer
void DNGameSceneLayer::removeMenuLayer()
{
    _mainMenu->removeAllChildrenWithCleanup(true);
    this->removeChild(_mainMenu);
    _mainMenu = NULL;
}

#pragma mark - replayGame
void DNGameSceneLayer::replayGame()
{
    setRunAnimation();
    DNDataManager::sharedManager()->_bgPackRef->setBg();
    auto m_LayerGround = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    DNDataManager::sharedManager()->_gameSceneRef->showHUDLayer();
    
    CallFunc* calSelector = CallFunc::create(this, callfunc_selector(DNGameScene::enablePauseMenu));
    
    
    DNDataManager::sharedManager()->_gameLayerRef->_character->runAction(Sequence::create(MoveTo::create(0.6, Vec2(m_LayerGround->getContentSize().width * 0.25 + DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width/2 ,DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionY())),calSelector, NULL));
    DNDataManager::sharedManager()->_gameLayerRef->scheduleUpdate();
    
    
    DNDataManager::sharedManager()->_HUDLayerRef->_isStarting = false;
}

#pragma mark - createInternetPopUp
void DNGameSceneLayer::createInternetPopUp()
{
    if(DNDataManager::sharedManager()->_mainmenu)
    {
        DNDataManager::sharedManager()->_mainmenu->menu->setEnabled(false);
        DNDataManager::sharedManager()->_mainmenu->arrowMenu->setEnabled(false);
    }       _userDialogue =  UserDialogueLayer::createDialogueMenu();
    _userDialogue->internetIssuePopUp();
    this->addChild(_userDialogue,999999999);
}

#pragma mark - removeInternetPopUp
void DNGameSceneLayer::removeInternetPopUp()
{
    if(_userDialogue)
    {
        this->removeChild(_userDialogue);
        _userDialogue = NULL;
        
        if(DNDataManager::sharedManager()->_mainmenu)
        {
            DNDataManager::sharedManager()->_mainmenu->menu->setEnabled(true);
            DNDataManager::sharedManager()->_mainmenu->arrowMenu->setEnabled(true);
        }
    }
}

#pragma mark - createConectingPopUp
void DNGameSceneLayer::createConectingPopUp()
{
    if(_userDialogue == NULL)
    {
        if(DNDataManager::sharedManager()->_mainmenu)
        {
            DNDataManager::sharedManager()->_mainmenu->menu->setEnabled(false);
            DNDataManager::sharedManager()->_mainmenu->arrowMenu->setEnabled(false);
        }
        
        _userDialogue =  UserDialogueLayer::createDialogueMenu();
        _userDialogue->connectingPopUp();
        this->addChild(_userDialogue,999999999);
    }
}

#pragma mark - onNoAdShowVictoryScreen
void DNGameSceneLayer::onNoAdShowVictoryScreen()
{
    if(_userDialogue == NULL)
    {
        _userDialogue =  UserDialogueLayer::createDialogueMenu();
        _userDialogue->noAdPopUp();
        this->addChild(_userDialogue,999999999);
    }
}

#pragma mark - removeNoAdPopUp
void DNGameSceneLayer::removeNoAdPopUp()
{
    if(_userDialogue)
    {
        this->removeChild(_userDialogue);
        _userDialogue = NULL;
    }
    removeReviveMenu();
}

#pragma mark - onExitButtonPressed
void DNGameSceneLayer::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(DNDataManager::sharedManager()->isgreenButtonTutorial)
    {
        return;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK && DNDataManager::sharedManager()->_isinGameScene)
    {
        DNDataManager::sharedManager()->_HUDLayerRef->pauseButtonTapped();
    }
    
}

float DNGameSceneLayer::getDistanceBetweenFloat(float x, float y)
{
    return x>y ? x-y : y-x;
}


#pragma mark - startCountDown
void DNGameSceneLayer::startCountDown()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(false);
    DNDataManager::sharedManager()->_HUDLayerRef->_isTapAllowed = false;
    _countDown = Label::createWithTTF("3", "fonts/motschcc.ttf", 80);
    this->addChild(_countDown,130);
    _countDown->setPosition(Vec2( this->getContentSize().width/2,visibleSize.height / 2 ));
    _countDown->setColor(Color3B(250,167,14));
    _countDown->enableOutline(Color4B(0,0,0,200),2);
    
    this->schedule(schedule_selector(DNGameSceneLayer::timer),1);
}

#pragma -mark createPopUpLayer
void DNGameSceneLayer::createPopUpLayer(int b)
{
    
    
    if(charLabel == NULL)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
        
        
        charLabel = Label::createWithTTF("completed daily challenge", "fonts/motschcc.ttf", 20);
        
        DNDataManager::sharedManager()->_gameLayerRef->addChild(charLabel,110);
        charLabel->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height * 0.75));
        charLabel->setColor(Color3B(250,167,14));
        charLabel->enableOutline(Color4B(0,0,0,200),2);
        if(b == 1)
        {
            charLabel->setString("You unlocked a new BG");
        }
        else if(b == 2)
        {
            charLabel->setString("You unlocked a new Character");
        }
        
        MoveTo *move = MoveTo::create(5, Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height));
        
        Sequence *finSeq=Sequence::create(DelayTime::create(1),move,CallFunc::create(CC_CALLBACK_0(::DNGameSceneLayer::removePopUpLayer,this)),NULL);
        charLabel->runAction(finSeq);
    }
    
}

#pragma -mark removePopUpLayer
void DNGameSceneLayer::removePopUpLayer()
{
    charLabel->removeFromParent();
    charLabel = NULL;
}

#pragma -mark createQuitPopUp
void DNGameSceneLayer::createQuitPopUp()
{
    _userDialogue =  UserDialogueLayer::createDialogueMenu();
    _userDialogue->createQuitDialogue();
    this->addChild(_userDialogue,999999999);
    
}

#pragma -mark setIdleAnimation
void DNGameSceneLayer::setIdleAnimation()
{
    switch (dataManager->buttonNumber)
    {
        case 1:
            characterRunAnimation->setAnimation(1, "Red_Idle", true);
            break;
        case 2:
            characterRunAnimation->setAnimation(1, "Green_Idle", true);
            break;
        case 3:
            characterRunAnimation->setAnimation(1, "Blue_Idle", true);
            break;
            
        default:
            break;
    }
    
}


#pragma -mark setRunAnimation
void DNGameSceneLayer::setRunAnimation()
{
    switch (DNDataManager::sharedManager()->buttonNumber) {
        case 1:
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Red_Run", true);
            break;
        case 2:
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Green_Run", true);
            break;
        case 3:
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Blue_Run", true);
            break;
            
        default:
            break;
    }
}
