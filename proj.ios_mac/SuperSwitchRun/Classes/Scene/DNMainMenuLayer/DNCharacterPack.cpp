//
//  DNCharacterPack.cpp
//  DesiNinja
//
//  Created by Yathiraj on 23/08/17.
//
//

#include "DNCharacterPack.hpp"
#include "ui/UIPageView.h"
#include "DNDataManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "DNAcheivementManager.hpp"
#include "PluginIAP/PluginIAP.h"
#include "DNGameUtility.hpp"

//#include "PluginFirebase/PluginFirebase.h"

#pragma -mark Constructor

template <typename T> std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }

DNCharacterPack::DNCharacterPack() {
    DNDataManager::sharedManager()->_characterPack = this;
    DNDataManager::sharedManager()->_isinGameScene = false;
    DNDataManager::sharedManager()->_mainmenu->_purchaseButton->setEnabled(false);
    
}

#pragma -mark Destructor
DNCharacterPack::~DNCharacterPack() {
     DNDataManager::sharedManager()->_mainmenu->_purchaseButton->setEnabled(true);
    DNDataManager::sharedManager()->_mainmenu->_handPointer->setVisible(true);
    if(DNDataManager::sharedManager()->_isBgAvailable)
    {
        DNDataManager::sharedManager()->_mainmenu->gameLogo->setVisible(true);
    }
}

#pragma -mark createCharacterPackLayer
DNCharacterPack* DNCharacterPack::createCharacterPackLayer()
{
    DNCharacterPack* scene = new DNCharacterPack();
    if(!scene->init())
    {
        CC_SAFE_DELETE(scene);
        return NULL;
    }
    
    
    scene->autorelease();
    return scene;
}

#pragma -mark Initialization
bool DNCharacterPack::init() {
    if (!Layer::init()) {
        return false;
    }
    
    
   
    
    DNDataManager::sharedManager()->_mainmenu->_handPointer->setVisible(false);
    DNDataManager::sharedManager()->_mainmenu->gameLogo->setVisible(false);
    
    _background = cocos2d::Sprite::create("Character_Locked/PopupScreen.png");
    _background->setPosition(cocos2d::Vec2(DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionX(),DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionY()  + DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().height/2));
    _background->setScale(0);
    addChild(_background,1000);
    
    Sprite* tile = Sprite::create("Character_Locked/Tile.png");
    tile->setPosition(Vec2(_background->getContentSize().width/2,_background->getContentSize().height * 0.60));
    _background->addChild(tile,1);
    
    
    
    _purchaseButton =cocos2d::MenuItemImage::create("ScreenLocked/PriceTag.png","ScreenLocked/PriceTag.png",CC_CALLBACK_0(DNCharacterPack::purchaseCharacterIap, this));
    _purchaseButton->setPosition(cocos2d::Vec2(_background->getContentSize().width * 0.75 , _background->getContentSize().height * 0.325));
    
    _price = Label::createWithTTF("10", "fonts/motschcc.ttf", 20);
    _purchaseButton->addChild(_price,6);
    _price->setPosition(Vec2(_purchaseButton->getContentSize().width * 0.575,_purchaseButton->getContentSize().height/2));
    _price->setColor(Color3B(250,167,14));
    _price->enableOutline(Color4B(0,0,0,200),2);
    
    Sprite* normalLeftArrowButton = Sprite::create("ScreenLocked/ArrowLeft.png");
    Sprite* selectedLeftArrowButton = Sprite::create("ScreenLocked/ArrowLeft.png");
    
    _leftArrow= cocos2d::MenuItemSprite::create(normalLeftArrowButton, selectedLeftArrowButton,CC_CALLBACK_1(DNCharacterPack::arrowsClicked, this));
    _leftArrow->setColor(Color3B::BLACK);
    _leftArrow->setTag(1);
    _leftArrow->setPosition(cocos2d::Vec2(_leftArrow->getContentSize().width * 1.50,_background->getContentSize().height * 0.62 ));
    
    
    Sprite* normalRightArrowButton = Sprite::create("ScreenLocked/ArrowRight.png");
    Sprite* selectedRightArrowButton = Sprite::create("ScreenLocked/ArrowRight.png");
    
    _rightArrow= cocos2d::MenuItemSprite::create(normalRightArrowButton, selectedRightArrowButton,CC_CALLBACK_1(DNCharacterPack::arrowsClicked, this));
    _rightArrow->setColor(Color3B::BLACK);
    _rightArrow->setTag(2);
    _rightArrow->setPosition(cocos2d::Vec2(_background->getContentSize().width - _rightArrow->getContentSize().width * 1.2,_background->getContentSize().height * 0.62 ));
    
    
    _lock = Sprite::create("Character_Locked/LockBtn.png");
    _lock->setPosition(Vec2(_background->getContentSize().width/2,_background->getContentSize().height * 0.6));
    _lock->setVisible(false);
    _background->addChild(_lock,100);
    
    Menu* menu1 = Menu::create(_purchaseButton,_leftArrow,_rightArrow,NULL);
    menu1->setPosition(Vec2::ZERO);
    menu1->setAnchorPoint(Vec2(0.5,0.5));
    _background->addChild(menu1, 1000001);
    
    _purchaseButton->setVisible(false);
    _purchaseButton->setEnabled(false);
    
    //PageView
    _pageview = cocos2d::ui::PageView::create();
    _pageview->setTouchEnabled(true);
    _pageview->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
    _pageview->setContentSize(cocos2d::Size(tile->getContentSize().width * 0.80,tile->getContentSize().height));
    _pageview->setPosition(cocos2d::Vec2(tile->getContentSize().width/2,tile->getContentSize().height/2));
    
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/ChacterDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    
    
    _typeOneDetailsArr.clear();
    _typeOneDetailsArr = rootDict["Character"].asValueVector();
    
    for(int i = 0;i< _typeOneDetailsArr.size() ; i++)
    {
        ValueMap typeDetailDict = _typeOneDetailsArr.at(i).asValueMap();
        UserDefault::getInstance()->setStringForKey("CharacterFileName",typeDetailDict["FileName"].asString());
        auto name =  UserDefault::getInstance()->getStringForKey("CharacterFileName","Character1");
        
        cocos2d::Sprite* character1 = cocos2d::Sprite::create("Character_Locked/"+name+".png");
        character1->setPosition(cocos2d::Vec2(tile->getContentSize().width * 0.4,tile->getContentSize().height/2));
        
        cocos2d::ui::Layout *layout1 = cocos2d::ui::Layout::create();
        layout1->setContentSize(cocos2d::Size(_pageview->getContentSize().width,_pageview->getContentSize().height));
        layout1->addChild(character1);
        
        
        _pageview->insertPage(layout1,i);
        
        auto charcterNameForLockedOrUnlocked = typeDetailDict["FileName"].asString();
        if(i == 0)
        {
            UserDefault::getInstance()->setBoolForKey(charcterNameForLockedOrUnlocked.c_str(), true);
            
        }
        
        _characterName  = cocos2d::Label::createWithTTF("","fonts/motschcc.ttf", 30);
        _characterName->setColor(Color3B(119,74,59));
        _characterName->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height * 0.175));
        _background->addChild(_characterName,100);
        
        _unlockLabel  = cocos2d::Label::createWithTTF("","fonts/motschcc.ttf", 20);
        _unlockLabel->setColor(Color3B(227,80,72));
        _unlockLabel->setPosition(cocos2d::Vec2( _background->getContentSize().width/2,_background->getContentSize().height * 0.10));
        _background->addChild(_unlockLabel,100);
        
        
        
        
        auto isUnLocked = UserDefault::getInstance()->getBoolForKey(charcterNameForLockedOrUnlocked.c_str(), false);
        
        /*TEsting  for all character ,comment after testing */
        
        // isUnLocked = true;
        
        
        if(!isUnLocked)
        {
            _purchaseButton->setVisible(true);
            _purchaseButton->setEnabled(true);
            _lock->setVisible(true);
            
        }
    }
    
    tile->addChild(_pageview,2);
    _pageview->setCurrentPageIndex( UserDefault::getInstance()->getIntegerForKey("characterIndex", 0));
    UserDefault::getInstance()->setIntegerForKey("characterIndex1", UserDefault::getInstance()->getIntegerForKey("characterIndex", 0));
    _pageview->addEventListener(CC_CALLBACK_2(DNCharacterPack::pageViewEvent, this));
    creatingCharacterAnimation(_pageview->getCurrentPageIndex());
    
    return true;
    
}

#pragma -mark purchaseCharacterIap
void DNCharacterPack::purchaseCharacterIap()
{
    if(!DNGameUtility::checkIsNetworkAvailable())
    {
            //CCLOG("no internet");
            DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();
                return;
    }
    else
    {
       // DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();
        if(_pageview->getCurrentPageIndex() == 1)
        {
            sdkbox::IAP::purchase("Unlock_Retard_Potato");
        }
        else if(_pageview->getCurrentPageIndex() == 2)
        {
            sdkbox::IAP::purchase("Unlock_Spooky_Pooky");
        }
        else if(_pageview->getCurrentPageIndex() == 3)
        {
            sdkbox::IAP::purchase("Unlock_Super_Sausage");
            
        }
        else if(_pageview->getCurrentPageIndex() == 4)
        {
            sdkbox::IAP::purchase("Unlock_Crazy_Cub");
        }
        
        //sdkbox::IAP::purchase("Unlock_Retard_Potato");
    }
}

#pragma -mark onRequestIAP
void DNCharacterPack::onRequestIAP(cocos2d::Ref* sender)
{
    if(!DNGameUtility::checkIsNetworkAvailable())
    {
        //CCLOG("no internet");
        return;
    }
    else
    {
        sdkbox::IAP::refresh();
    }
}

#pragma -mark onRestoreIAP
void DNCharacterPack::onRestoreIAP(cocos2d::Ref* sender)
{
    if(!DNGameUtility::checkIsNetworkAvailable())
    {
        //CCLOG("no internet");
        return;
    }
    else
    {
        sdkbox::IAP::restore();
    }
}









#pragma -mark showCharacterPackMenu
void DNCharacterPack::showCharacterPackMenu()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    _background->runAction(MoveTo::create(0.1,Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2)));
    _background->runAction(ScaleTo::create(0.1, 1));
}


#pragma -mark pageViewEvent
void DNCharacterPack::pageViewEvent(cocos2d::Ref* sender,cocos2d::ui::PageView::EventType type)
{
    switch (type) {
        case cocos2d::ui::PageView::EventType::TURNING:
            cocos2d::ui::PageView *pageView = dynamic_cast<cocos2d::ui::PageView*>(sender);
            
            auto index = pageView->getCurrentPageIndex();
            UserDefault::getInstance()->setIntegerForKey("characterIndex1",index);
            creatingCharacterAnimation(index);
            
            
    }
}


#pragma -mark creatingCharacterAnimation
void DNCharacterPack::creatingCharacterAnimation(int index)
{
    ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    UserDefault::getInstance()->setStringForKey("Character",typeDetailDict["Name"].asString());
    auto character = UserDefault::getInstance()->getStringForKey("Character", "char2/Character2");
    auto charcterNameForLockedOrUnlocked = typeDetailDict["Name"].asString();
    auto isUnLocked = UserDefault::getInstance()->getBoolForKey(charcterNameForLockedOrUnlocked.c_str(), false);
    
    /*TEsting  for all character ,comment after testing */
    //  isUnLocked = true;
    
    if(index == 0)
    {
        isUnLocked = true;
        _leftArrow->setEnabled(false);
        _leftArrow->setVisible(false);
        
    }
    else if(index == 4)
    {
        _rightArrow->setEnabled(false);
        _rightArrow->setVisible(false);
    }
    else{
        _leftArrow->setEnabled(true);
        _leftArrow->setVisible(true);
        _rightArrow->setEnabled(true);
        _rightArrow->setVisible(true);
    }
    
    _characterName->setString(typeDetailDict["DisplayName"].asString());
    if(!isUnLocked)
    {
        
        _purchaseButton->setVisible(true);
        _purchaseButton->setEnabled(true);
        // _characterName->setPositionY(_background->getContentSize().height * 0.25);
        _lock->setVisible(true);
        _unlockLabel->setString(typeDetailDict["UnlockTask"].asString());
        if(DNDataManager::sharedManager()->_mainmenu->_productPrice.size() > 0)
        {
            _price->setString(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("BgAmount",10)));
        }
    }
    else
    {
        DNDataManager::sharedManager()->_gameLayerRef->_character->removeChild(DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation,true);
        _purchaseButton->setVisible(false);
        _purchaseButton->setEnabled(false);
        _lock->setVisible(false);
        _unlockLabel->setString("");
        
        //Creating Charcter Animation
        DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation = spine::SkeletonAnimation::createWithBinaryFile("GameScene/Animation/Character/"+character+".skel", "GameScene/Animation/Character/"+character+".atlas",0.5f);
        
        UserDefault::getInstance()->setStringForKey("character", "GameScene/Animation/Character/" + character);
        UserDefault::getInstance()->setIntegerForKey("characterIndex", index);
        DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setScale(0.60);
        DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnchorPoint(Vec2(0,0));
        DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setPosition(Vec2(DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width/2  ,15));
        DNDataManager::sharedManager()->_gameLayerRef->_character->addChild( DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation,100);
        
        //Setting Random Charcter Animation
        int rand = random(1, 3);
        if(rand == 1)
        {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Red_Idle", true);
            DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(1);
        }
        else if(rand == 2)
        {
            rand = 3;
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Blue_Idle", true);
            DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(3);
        }
        else if(rand == 3)
        {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Blue_Idle", true);
            DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(3);
        }
        DNDataManager::sharedManager()->buttonNumber = rand;
        
    }
    
}

#pragma -mark arrowsClicked
void DNCharacterPack::arrowsClicked(Ref* sender)
{
    cocos2d::MenuItemSprite* arrows = dynamic_cast<cocos2d::MenuItemSprite*>(sender);
    
DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");    
    if(arrows->getTag() == 1)
    {
        if(UserDefault::getInstance()->getIntegerForKey("characterIndex1",0) > 0)
        {
            int ind = UserDefault::getInstance()->getIntegerForKey("characterIndex1",0);
            ind = ind - 1 ;
            _pageview->setCurSelectedIndex(ind);
            _pageview->scrollToPage(ind);
            UserDefault::getInstance()->setIntegerForKey("characterIndex1",ind);
        }
        else{
            arrows->setEnabled(false);
            arrows->setVisible(false);
            _rightArrow->setEnabled(true);
            _rightArrow->setVisible(true);
        }
        
    }
    else if(arrows->getTag() == 2)
    {
        if(UserDefault::getInstance()->getIntegerForKey("characterIndex1",0) < 4)
        {
            int ind = UserDefault::getInstance()->getIntegerForKey("characterIndex1",0);
            ind = ind + 1 ;
            _pageview->setCurSelectedIndex(ind);
            _pageview->scrollToPage(ind);
            _pageview->setIndicatorEnabled(true);
            UserDefault::getInstance()->setIntegerForKey("characterIndex1",ind);
        }
        else{
            arrows->setEnabled(false);
            arrows->setVisible(false);
            _leftArrow->setEnabled(true);
            _leftArrow->setVisible(true);
        }
        
    }
    
    creatingCharacterAnimation(_pageview->getCurSelectedIndex());
    
    
}

#pragma -mark arrowsClicked
void DNCharacterPack::purchasecharacter()
{
    DNAcheivementManager::sharedManager()->unloackCharacter(_pageview->getCurrentPageIndex());
    creatingCharacterAnimation(_pageview->getCurrentPageIndex());
}
