//
//  DNMainMenuLayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 21/08/17.
//
//

#include "DNMainMenuLayer.hpp"
#include "DNHUDLayer.hpp"
#include "DNDataManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "DNConstants.h"
#include "PluginIAP/PluginIAP.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include "UserDialogueLayer.hpp"
#include "DNAcheivementManager.hpp"
#include "PluginIAP/PluginIAP.h"
#include "PluginFacebook/PluginFacebook.h"
#include "PluginAdMob/PluginAdMob.h"
#include "DNAcheivementManager.hpp"
#include "SpriteFromURL.h"
#include  "DNGameUtility.hpp"
#include "DNCommonUtils.h"


#pragma -mark Constructor

template<typename T>
std::string tostr(const T &t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

DNMainMenuLayer::DNMainMenuLayer() {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(DNMainMenuLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(DNMainMenuLayer::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(DNMainMenuLayer::onTouchMoved, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    DNDataManager::sharedManager()->_mainmenu = this;
    DNDataManager::sharedManager()->_isCharcterPackMenuCreated = false;
    DNDataManager::sharedManager()->_isinGameScene = false;

    //Adding keyboard Listners
    auto keyBoardListener = EventListenerKeyboard::create();
    keyBoardListener->onKeyReleased = CC_CALLBACK_2(DNMainMenuLayer::onExitButtonPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

#pragma -mark Destructor

DNMainMenuLayer::~DNMainMenuLayer() {
    sdkbox::PluginFacebook::removeListener();
    sdkbox::IAP::removeListener();
    sdkbox::PluginSdkboxPlay::removeListener();

    DNDataManager::sharedManager()->_mainmenu = NULL;
}

#pragma -mark createBackground

DNMainMenuLayer *DNMainMenuLayer::createMeinMenu() {
    DNMainMenuLayer *scene = new DNMainMenuLayer();
    if (!scene->init()) {
        CC_SAFE_DELETE(scene);
        return NULL;
    }

    scene->autorelease();
    return scene;
}

#pragma -mark Initialization

bool DNMainMenuLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        if (UserDefault::getInstance()->getBoolForKey("morningmusicPlaying", true)) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/morning.mp3", true);

        } else if (UserDefault::getInstance()->getBoolForKey("nightmusicPlaying", true)) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/night.mp3", true);
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(UserDefault::getInstance()->getFloatForKey("volume", 0.5));
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("effect", 0.5));
    }

    sdkbox::IAP::setListener(this);
    sdkbox::PluginFacebook::setListener(this);
    sdkbox::PluginSdkboxPlay::setListener(this);


    if (DNGameUtility::checkIsNetworkAvailable()) {
        // sdkbox::PluginAdMob::cache("rewarded");
    }

    auto m_LayerGround1 = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    isFBPopupAdded = false;

    Sprite *highScore = Sprite::create("MainMenu/Rowtile1.png");

    highScore->setPosition(Vec2(origin.x + visibleSize.width * 0.50, origin.y + visibleSize.height * 0.90));
    this->addChild(highScore, 1000010);

    cocos2d::Label *highScoreLabel = cocos2d::Label::createWithTTF("BEST SCORE : " + StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("HighScore", 0)), "fonts/motschcc.ttf", 20);
    highScoreLabel->setPosition(cocos2d::Vec2(highScore->getContentSize().width / 2, highScore->getContentSize().height * 0.50));
    highScore->addChild(highScoreLabel, 100);

    _startLabel = cocos2d::Label::createWithTTF("TAP TO START", "fonts/motschcc.ttf", 50);
    _startLabel->setPosition(cocos2d::Vec2(this->getContentSize().width / 2, visibleSize.height * 0.10));
    this->addChild(_startLabel, 100);
    _startLabel->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.30, 0.50), ScaleTo::create(0.30, 0.60), NULL)));

    gameLogo = Sprite::create("GameLogo/LogoAlpha.png");
    gameLogo->setAnchorPoint(Vec2(0.5, 0.5));
    gameLogo->setScale(0.85);
    gameLogo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + m_LayerGround1->getContentSize().height * 0.725 + gameLogo->getContentSize().height * gameLogo->getScale() * 0.5f));
    this->addChild(gameLogo, 1000000);

    //Arrows
    Sprite *normalLeftArrowButton = Sprite::create("ScreenLocked/ArrowLeft.png");
    Sprite *selectedLeftArrowButton = Sprite::create("ScreenLocked/ArrowLeft.png");

    _leftArrow = cocos2d::MenuItemSprite::create(normalLeftArrowButton, selectedLeftArrowButton, CC_CALLBACK_1(DNMainMenuLayer::arrowsClicked, this));
    _leftArrow->setTag(1);
    _leftArrow->setPosition(cocos2d::Vec2(origin.x + _leftArrow->getContentSize().width / 2, origin.y + visibleSize.height / 2));


    Sprite *normalRightArrowButton = Sprite::create("ScreenLocked/ArrowRight.png");
    Sprite *selectedRightArrowButton = Sprite::create("ScreenLocked/ArrowRight.png");

    _rightArrow = cocos2d::MenuItemSprite::create(normalRightArrowButton, selectedRightArrowButton, CC_CALLBACK_1(DNMainMenuLayer::arrowsClicked, this));
    _rightArrow->setTag(2);
    _rightArrow->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - _rightArrow->getContentSize().width / 2, _leftArrow->getPositionY()));

    arrowMenu = Menu::create(_leftArrow, _rightArrow, NULL);
    arrowMenu->setPosition(Vec2::ZERO);
    arrowMenu->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(arrowMenu, 1000001);


    //LeaderBoard
    Sprite *normalachievementButton = Sprite::create("MainMenu/Achievement_button.png");
    Sprite *selectedachievementButton = Sprite::create("MainMenu/Achievement_button.png");

    cocos2d::MenuItemSprite *achievementButton = cocos2d::MenuItemSprite::create(normalachievementButton, selectedachievementButton, CC_CALLBACK_0(DNMainMenuLayer::showAchievement, this));
    achievementButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.10, origin.y + visibleSize.height * 0.90));

    //AchieveMent
    Sprite *normalleadreBoardButton = Sprite::create("MainMenu/Leaderboard_button.png");
    Sprite *selectedleadreBoardButton = Sprite::create("MainMenu/Leaderboard_button.png");


    cocos2d::MenuItemSprite *leadreBoardButton = cocos2d::MenuItemSprite::create(normalleadreBoardButton, selectedleadreBoardButton, CC_CALLBACK_0(DNMainMenuLayer::showLeaderBoard, this));
    leadreBoardButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.20, origin.y + visibleSize.height * 0.90));

    //Facebook
    Sprite *normalfacebookButton = Sprite::create("MainMenu/FB_Button.png");
    normalfacebookButton->setScale(0.90);
    Sprite *selectedfacebookButton = Sprite::create("MainMenu/FB_Button.png");

    MenuItemSprite *facebookButton = cocos2d::MenuItemSprite::create(normalfacebookButton, selectedfacebookButton, CC_CALLBACK_0(DNMainMenuLayer::showFriendsScoreAndName, this));
    facebookButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.90, leadreBoardButton->getPositionY()));

    //Rate App
    Sprite *normalrateButton = Sprite::create("MainMenu/Rate_Us_Btn.png");
    Sprite *selectedrateButton = Sprite::create("MainMenu/Rate_Us_Btn.png");

    _rateButton = cocos2d::MenuItemSprite::create(normalrateButton, selectedrateButton, CC_CALLBACK_0(DNMainMenuLayer::rateUsBtnClick, this));
    _rateButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.90, leadreBoardButton->getPositionY()));

    //BG Packs
    Sprite *normalbgPackButton = Sprite::create("MainMenu/BG_package_button.png");
    Sprite *selectedbgPackButton = Sprite::create("MainMenu/BG_package_button.png");

    bgPackButton = cocos2d::MenuItemSprite::create(normalbgPackButton, selectedbgPackButton, CC_CALLBACK_0(DNMainMenuLayer::unLockBackGround, this));
    bgPackButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.10, origin.y + m_LayerGround1->getContentSize().height * 0.40));

    //Character pack
    Sprite *normalcharacterPackButton = Sprite::create("MainMenu/character_package_button.png");
    Sprite *selectedcharacterPackButton = Sprite::create("MainMenu/character_package_button.png");

    characterPackButton = cocos2d::MenuItemSprite::create(normalcharacterPackButton, selectedcharacterPackButton, CC_CALLBACK_0(DNMainMenuLayer::unlockCharacter1, this));
    characterPackButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.20, bgPackButton->getPositionY()));

    //Ad
    Sprite *normalnoAdButton = Sprite::create("MainMenu/Remove_Ad_Btn2.png");
    Sprite *selectednoAdButton = Sprite::create("MainMenu/Remove_Ad_Btn2.png");

    noAdButton = cocos2d::MenuItemSprite::create(normalnoAdButton, selectednoAdButton, CC_CALLBACK_0(DNMainMenuLayer::IAPopen, this));
    noAdButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.90, characterPackButton->getPositionY()));

    //Restore Purchase
    Sprite *normalrestorePurchaseButton = Sprite::create("MainMenu/Restore_PurchaseBtn.png");
    Sprite *selectedrestorePurchaseButton = Sprite::create("MainMenu/Restore_PurchaseBtn.png");

    _restorePurchaseButton = cocos2d::MenuItemSprite::create(normalrestorePurchaseButton, selectedrestorePurchaseButton, CC_CALLBACK_0(DNMainMenuLayer::onIAPRestoreClick, this));
    _restorePurchaseButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.80, characterPackButton->getPositionY()));

    menu = Menu::create(achievementButton, leadreBoardButton,/*facebookButton,*/bgPackButton, _rateButton, _restorePurchaseButton, characterPackButton, noAdButton, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menu, 10000);

    if (UserDefault::getInstance()->getBoolForKey("isBGPackPurchased", false)) {
        if (bgPackButton) {
            bgPackButton->setVisible(false);
            bgPackButton->setEnabled(false);
            this->removeChild(bgPackButton);
        }

    } else {
        if (UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) > 4) {
            UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
        }
    }
    if (UserDefault::getInstance()->getBoolForKey("isCharacterPackPurchased", false)) {
        if (characterPackButton) {
            characterPackButton->setVisible(false);
            characterPackButton->setEnabled(false);
            this->removeChild(characterPackButton);
        }
    } else {
        if (UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) > 3) {
            UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
        }
    }
    if (UserDefault::getInstance()->getBoolForKey("isnoAdPackPurchased", false)) {
        if (noAdButton) {
            noAdButton->setVisible(false);
            noAdButton->setEnabled(false);
            this->removeChild(noAdButton);
        } else if (_restorePurchaseButton) {
            _restorePurchaseButton->setPositionX(this->getContentSize().width * 0.85);
        }
    }
    if (UserDefault::getInstance()->getBoolForKey("restorePurchased", false)) {
        _restorePurchaseButton->setEnabled(false);
        _restorePurchaseButton->setVisible(false);
        UserDefault::getInstance()->setBoolForKey("restorePurchased", true);
    }

    _background = Sprite::create("ScreenLocked/Caption_box.png");
    _background->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.50, origin.y + visibleSize.height * 0.75));
    this->addChild(_background, 1000001);

    unlockDescription = cocos2d::Label::createWithTTF("", "fonts/motschcc.ttf", 30);
    unlockDescription->setColor(Color3B(119, 74, 59));
    unlockDescription->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2, _background->getContentSize().height * 0.50));
    _background->addChild(unlockDescription, 100);

    _lock = Sprite::create("ScreenLocked/LockBtn.png");
    _lock->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.50, origin.y + visibleSize.height * 0.50));
    this->addChild(_lock, 1000001);

    _background2 = Sprite::create("ScreenLocked/Caption_box2.png");
    _background2->setScale(0.5, 0.75);
    _background2->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.50, origin.y + visibleSize.height * 0.275));
    this->addChild(_background2, 1000001);

    cocos2d::Label *unlockDescription1 = cocos2d::Label::createWithTTF("Buy", "fonts/motschcc.ttf", 30);
    unlockDescription1->setColor(Color3B(119, 74, 59));
    unlockDescription1->setPosition(cocos2d::Vec2(_background2->getContentSize().width / 2, _background2->getContentSize().height * 0.50));
    _background2->addChild(unlockDescription1, 100);

    _purchaseButton = cocos2d::MenuItemImage::create("ScreenLocked/PriceTag.png", "ScreenLocked/PriceTag.png", CC_CALLBACK_0(DNMainMenuLayer::purchaseOneByOneBg, this));
    _purchaseButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.50 + _background2->getContentSize().width * 0.23, origin.y + visibleSize.height * 0.275));

    _price = Label::createWithTTF("10", "fonts/motschcc.ttf", 20);
    _purchaseButton->addChild(_price, 6);
    _price->setPosition(Vec2(_purchaseButton->getContentSize().width * 0.575, _purchaseButton->getContentSize().height / 2));
    _price->setColor(Color3B(250, 167, 14));
    _price->enableOutline(Color4B(0, 0, 0, 200), 2);

    Menu *menu1 = Menu::create(_purchaseButton, NULL);
    menu1->setPosition(Vec2::ZERO);
    menu1->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menu1, 1000001);

    disableLockScreen();

    auto pos = bgPackButton->getPositionX() + bgPackButton->getContentSize().width * 0.30;

    DNDataManager::sharedManager()->_gameLayerRef->_character->setPosition(cocos2d::Vec2(pos + DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width / 2, origin.y + (DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1->getContentSize().height * 0.80)));

    //HandPointer
    _handPointer = Sprite::create("Tutorial/HandPointer.png");
    _handPointer->setScale(0.5);
    _handPointer->setPosition(Vec2(DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionX() + DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width * 0.6, DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionY() + 125));
    this->addChild(_handPointer, 1000000);
    _handPointer->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.30, Vec2(0, 10)), MoveBy::create(0.30, Vec2(0, -10)), NULL)));

    DNDataManager::sharedManager()->_bgPackRef->checkBg(DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex());
    if (DNGameUtility::checkIsNetworkAvailable()) {
        if (UserDefault::getInstance()->getBoolForKey("sdkboxsigin", true)) {
            sdkbox::PluginSdkboxPlay::signin();

            UserDefault::getInstance()->setBoolForKey("sdkboxsigin", false);
        }
    }

    _chracterPackMenu = cocos2d::MenuItemImage::create("GameScene/Characeter_Mask2.png", "GameScene/Characeter_Mask2.png", CC_CALLBACK_0(DNMainMenuLayer::loadCharacterPack, this));
    _chracterPackMenu->setAnchorPoint(Vec2(0, 0));
    _chracterPackMenu->setTag(0);
    _chracterPackMenu->setPosition(cocos2d::Vec2(DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionX() - DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width / 2,
                                                 DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionY() - DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().height / 2));

    _menu = Menu::create(_chracterPackMenu, NULL);
    _menu->setPosition(Vec2::ZERO);
    _menu->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_menu, 1000010);


    // UserDefault::getInstance()->setBoolForKey("isFirstLogin", true);
    if (UserDefault::getInstance()->getBoolForKey("isFirstLogin", true)) {


        ActivtyIndicator::activityIndicatorOnScene("Loading...  Please wait", this);


        CallFunc *calSelector = CallFunc::create(this, callfunc_selector(DNMainMenuLayer::pleaseWaitPopUp));
        this->runAction(Sequence::create(DelayTime::create(10), calSelector, NULL));
    }

    return true;
}

#pragma -mark loadCharacterPack

void DNMainMenuLayer::loadCharacterPack() {
    DNDataManager::sharedManager()->_gameLayerRef->createCharacterPack();

}

#pragma -mark loadCharacterPack

void DNMainMenuLayer::pleaseWaitPopUp() {

    UserDefault::getInstance()->setBoolForKey("isFirstLogin", false);

    ActivtyIndicator::PopIfActiveFromScene(this);


}

#pragma -mark rateUsBtnClick

void DNMainMenuLayer::rateUsBtnClick() {
    // CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;

    //Analytics
    DNDataManager::sharedManager()->callAnalytics("Rate_App_button", "Clicked", "Main_Menu", 1);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Application::getInstance()->openURL("https://play.google.com/store/apps/details?id=com.ghostchillies.superswitchrun");
#else
    Application::getInstance()->openURL("https://itunes.apple.com/us/app/super-switch-run/id1276997017?ls=1&mt=8");
#endif
}

#pragma -mark onIAPRestoreClick

void DNMainMenuLayer::onIAPRestoreClick() {
    // CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/UI_Button.mp3");
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;
    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {
        sdkbox::IAP::restore();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Restore_button", "Clicked", "Main_Menu", 1);

    }
}

#pragma -mark onConnectionStatusChanged

void DNMainMenuLayer::onConnectionStatusChanged(int status) {

}

#pragma -mark onScoreSubmitted

void DNMainMenuLayer::onScoreSubmitted(const std::string &leaderboard_name, long score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday) {

}

#pragma -mark onIncrementalAchievementUnlocked

void DNMainMenuLayer::onIncrementalAchievementUnlocked(const std::string &achievement_name) {

}

#pragma -mark onIncrementalAchievementStep

void DNMainMenuLayer::onIncrementalAchievementStep(const std::string &achievement_name, double step) {

}

#pragma -mark onAchievementUnlocked

void DNMainMenuLayer::onAchievementUnlocked(const std::string &achievement_name, bool newlyUnlocked) {

}

#pragma -mark unLockBackGround

void DNMainMenuLayer::unLockBackGround() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;


    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {
        DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();
        sdkbox::IAP::purchase("Unlock_all_the_levels");

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Background_Pack_button", "Clicked", "Main_Menu", 1);


    }

}

#pragma -mark showFriendsScoreAndName

void DNMainMenuLayer::showFriendsScoreAndName() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    if (isFBPopupAdded)
        return;


    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {
        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Facebook_leaderboard_button", "Clicked", "Main_Menu", 1);


        if (!sdkbox::PluginFacebook::isLoggedIn()) {
            sdkbox::PluginFacebook::requestReadPermissions({sdkbox::FB_PERM_READ_PUBLIC_PROFILE,
                                                            sdkbox::FB_PERM_READ_USER_FRIENDS});
        } else {
            sdkbox::FBAPIParam params;
            params["fields"] = "name,scores";
            sdkbox::PluginFacebook::api("me/friends", "GET", params, "FriendsList");
        }


    }
    //DNMainMenuLayer::showPopUpForFacebookLeaderboard();
}

#pragma -mark showPopUpForFacebookLeaderboard

void DNMainMenuLayer::showPopUpForFacebookLeaderboard() {

    if (isFBPopupAdded)
        return;

    isFBPopupAdded = true;

    menu->setEnabled(false);
    arrowMenu->setEnabled(false);
    DNDataManager::sharedManager()->_mainmenu->_menu->setEnabled(false);

    //CCLOG("Show facebook leaderBoard");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float incremntInnerContentSize = 0;

    Show_friends_score_popup = cocos2d::Sprite::create("MainMenu/Box.png");
    Show_friends_score_popup->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(Show_friends_score_popup, 1000000000);


    auto Invite_friend = cocos2d::MenuItemImage::create("MainMenu/Invite-friend-button.png", "MainMenu/Invite-friend-button.png", CC_CALLBACK_0(DNMainMenuLayer::inviteFriends, this));
    Invite_friend->setPosition(cocos2d::Vec2(Show_friends_score_popup->getContentSize().width / 2, Show_friends_score_popup->getContentSize().height / 2 - 160));


    Label *price = Label::createWithTTF("INVITE FRIENDS", "fonts/motschcc.ttf", 35);
    Invite_friend->addChild(price, 6);
    price->setPosition(Vec2(Invite_friend->getContentSize().width / 2, Invite_friend->getContentSize().height / 2));
    //    price->setColor(Color3B(250,167,14));

    Menu *menu1 = Menu::create(Invite_friend, NULL);
    menu1->setPosition(Vec2::ZERO);
    menu1->setAnchorPoint(Vec2(0.5, 0.5));
    Show_friends_score_popup->addChild(menu1, 1000001);

    //Scroll
    ui::ScrollView *scrollView = ui::ScrollView::create();
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setContentSize(Size(600, 320));
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    scrollView->setBounceEnabled(false);

    Vec2 value;
    scrollView->setPosition(Vec2(Show_friends_score_popup->getContentSize().width / 2, Show_friends_score_popup->getContentSize().height / 2 + 50));
    float innerWidth = scrollView->getContentSize().width;
    float innerHeight = scrollView->getContentSize().height * 0.9;
    incremntInnerContentSize = 3.3;
    innerHeight = innerHeight * incremntInnerContentSize;
    //    scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
    auto Show_friends_Score_name1 = cocos2d::Sprite::create("MainMenu/Rowtile1.png");
    float innerHeightVal = Show_friends_Score_name1->getContentSize().height / 2;

    Show_friends_score_popup->addChild(scrollView);
    innerHeight = Show_friends_Score_name1->getContentSize().height;
    incremntInnerContentSize = 4;
    innerHeight = (innerHeight + 4) * document["data"].Size();

    if (innerHeight < 320)
        innerHeight = 320;
    scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

    if (document["data"].Size() == 0) {
        auto ScoreLable = Label::createWithTTF("No friends playing", "fonts/motschcc.ttf", 32);
        ScoreLable->setPosition(Vec2(innerWidth / 2, innerHeight * 0.55));
        scrollView->addChild(ScoreLable);
        ScoreLable->setColor(Color3B(165, 42, 42));


        auto ScoreLable2 = Label::createWithTTF("around you!!!", "fonts/motschcc.ttf", 32);
        ScoreLable2->setPosition(Vec2(innerWidth / 2, innerHeight * 0.45));
        scrollView->addChild(ScoreLable2);
        ScoreLable2->setColor(Color3B(165, 42, 42));
        // around you!!!

        auto ScoreLable1 = Label::createWithTTF("Please invite!!", "fonts/motschcc.ttf", 32);
        ScoreLable1->setPosition(Vec2(innerWidth / 2, innerHeight * 0.22));
        scrollView->addChild(ScoreLable1);
        ScoreLable1->setColor(Color3B(165, 42, 42));

    }

    for (rapidjson::SizeType i = document["data"].Size(); i > 0; i--) {

        const rapidjson::Value &game = document["data"][i - 1];

        std::string friendName = game["name"].GetString();

        std::string idStr = game["id"].GetString();

        int gameScore = 0;

        if (game.HasMember("scores")) {
            const rapidjson::Value &scoreDict = game["scores"]["data"][0];

            gameScore = scoreDict["score"].GetInt();
        }

        auto Show_friends_Score_name2 = cocos2d::Sprite::create("MainMenu/Rowtile1.png");
        Show_friends_Score_name2->setPosition(Vec2(innerWidth * 0.65, innerHeight - innerHeightVal));
        innerHeightVal += Show_friends_Score_name2->getContentSize().height + 4;
        scrollView->addChild(Show_friends_Score_name2);

        //name
        auto NameLable = Label::createWithTTF(friendName.c_str(), "fonts/motschcc.ttf", 32, Size(275, 32));
        NameLable->setPosition(Vec2(NameLable->getContentSize().width / 2 + 20, Show_friends_Score_name2->getContentSize().height / 2));
        Show_friends_Score_name2->addChild(NameLable);
        NameLable->setColor(Color3B(165, 42, 42));

        //        NameLable-(10);

        __String *scoreStr = __String::createWithFormat("%d", gameScore);
        //score
        auto ScoreLable = Label::createWithTTF(scoreStr->getCString(), "fonts/motschcc.ttf", 32);
        ScoreLable->setPosition(Vec2(Show_friends_Score_name2->getContentSize().width - ScoreLable->getContentSize().width / 2 - 50, Show_friends_Score_name2->getContentSize().height / 2));
        Show_friends_Score_name2->addChild(ScoreLable);
        ScoreLable->setColor(Color3B(165, 42, 42));

        //image for photo
        std::string url;
        url = "https://graph.facebook.com/" + idStr + "/picture?type=large&return_ssl_resources=1";
        Sprite *profilePic;
        profilePic = SpriteFromUrl::create(url.c_str(), "MainMenu/Profileicon1.png", true, false);

        profilePic->setPosition(Vec2(-profilePic->getContentSize().width / 2 - 10, Show_friends_Score_name2->getContentSize().height / 2));
        Show_friends_Score_name2->addChild(profilePic);


    }


}

#pragma -mark inviteFriends

void DNMainMenuLayer::inviteFriends() {
    //CCLOG("Inside inviteFriends");
    if (DNGameUtility::checkIsNetworkAvailable()) {
        sdkbox::PluginFacebook::inviteFriends(
                "https://fb.me/134978107109904",
                "http://juego-build-server.s3.amazonaws.com/SuperSwitchRun/Game-Icon0052.png");
    }

}

#pragma -mark addScrollView

void DNMainMenuLayer::addScrollView() {

}


//MARK: FaceBook
#pragma -mark onLogin

void DNMainMenuLayer::onLogin(bool isLogin, const std::string &error) {
}

#pragma -mark onAPI

void DNMainMenuLayer::onAPI(const std::string &tag, const std::string &jsonData) {
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();


    if (tag == "FriendsList") {


        document = NULL;
        document.Parse<0>(jsonData.c_str());


        if (document["data"].Size() > 1) {
            for (rapidjson::SizeType i = 0; i < document["data"].Size(); i++) {
                for (rapidjson::SizeType j = 0; j < document["data"].Size() - 1 - i; j++) {

                    const rapidjson::Value &val1 = document["data"][j];
                    const rapidjson::Value &val2 = document["data"][j + 1];

                    int gameScore1 = 0, gameScore2 = 0;


                    if (val1.HasMember("scores")) {
                        const rapidjson::Value &scoreDict = val1["scores"]["data"][0];

                        gameScore1 = scoreDict["score"].GetInt();
                    }

                    if (val2.HasMember("scores")) {
                        const rapidjson::Value &scoreDict = val2["scores"]["data"][0];

                        gameScore2 = scoreDict["score"].GetInt();
                    }

                    if (gameScore1 > gameScore2) {
                        document["data"][j].Swap(document["data"][j + 1]);
                    }
                }
            }
        }


        if (document.HasParseError()) {
            //CCLOG("Json has errors!!!");
            return;
        }

        for (rapidjson::SizeType i = 0; i < document["data"].Size(); i++) {
            const rapidjson::Value &game = document["data"][i];

            std::string friendName = game["name"].GetString();

            std::string idStr = game["id"].GetString();

            int gameScore = 0;

            if (game.HasMember("scores")) {
                const rapidjson::Value &scoreDict = game["scores"]["data"][0];

                gameScore = scoreDict["score"].GetInt();
            }
            //CCLOG("Friend Name %s and %s and score %d",friendName.c_str(),idStr.c_str(),gameScore);
        }
        showPopUpForFacebookLeaderboard();
    }
}

#pragma -mark onSharedSuccess

void DNMainMenuLayer::onSharedSuccess(const std::string &message) {
    //CCLOG("##FB onSharedSuccess:%s", message.c_str());

    //    MessageBox(message.c_str(), "share success");
}

#pragma -mark onSharedFailed

void DNMainMenuLayer::onSharedFailed(const std::string &message) {
    //CCLOG("##FB onSharedFailed:%s", message.c_str());

    //    MessageBox(message.c_str(), "share failed");
}

#pragma -mark onSharedCancel

void DNMainMenuLayer::onSharedCancel() {
    //CCLOG("##FB onSharedCancel");

    //    MessageBox("", "share cancel");
}

#pragma -mark onPermission

void DNMainMenuLayer::onPermission(bool isLogin, const std::string &error) {
    //CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    DNDataManager::sharedManager()->_dialogueLayer->removeInternetPopup();

    /*if(isLogin)
     {
     bool isAsked=UserDefault::getInstance()->getBoolForKey("isPublishPermision", false);
     if(!isAsked)
     {
     DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();
     UserDefault::getInstance()->setBoolForKey("isPublishPermision", true);
     sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
     }
     }*/

    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    //    MessageBox(error.c_str(), title.c_str());
}

#pragma -mark onInviteFriendsResult

void DNMainMenuLayer::onInviteFriendsResult(bool result, const std::string &msg) {
    //CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
}

#pragma -mark purchaseOneByOneBg

void DNMainMenuLayer::purchaseOneByOneBg() {
    if (isFBPopupAdded)
        return;

    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {


        DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();

        //   DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();


        if (DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex() == 1) {
            sdkbox::IAP::purchase("Unlock_megalopolis");
        } else if (DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex() == 2) {
            sdkbox::IAP::purchase("Unlock_in_to_the_wood");
        } else if (DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex() == 3) {
            sdkbox::IAP::purchase("Unlock_Wilderness");
        } else if (DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex() == 4) {
            sdkbox::IAP::purchase("Unlock_Barren_Land");
        } else if (DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex() == 5) {
            sdkbox::IAP::purchase("Unlock_Desert_Night");
        }



        //  sdkbox::IAP::purchase("Unlock_Desert_Night");
    }
}

#pragma -mark unlockCharacter1

void DNMainMenuLayer::unlockCharacter1() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {

        DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();


        sdkbox::IAP::purchase("Unlock_all_the_characters");

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_Pack_button", "Clicked", "Main_Menu", 1);
    }
}

#pragma -mark showLeaderBoard

void DNMainMenuLayer::showLeaderBoard() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    if (isFBPopupAdded)
        return;

    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {
        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Leaderboard_button", "Clicked", "Main_Menu", 1);

        if (sdkbox::PluginSdkboxPlay::isSignedIn()) {
            sdkbox::PluginSdkboxPlay::showLeaderboard("ldb1");
        } else {
            sdkbox::PluginSdkboxPlay::signin();
        }


    }
}

#pragma -mark showAchievement

void DNMainMenuLayer::showAchievement() {


    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;

    if (!DNGameUtility::checkIsNetworkAvailable()) {


        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {
        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Achievements_button", "Clicked", "Main_Menu", 1);

        if (sdkbox::PluginSdkboxPlay::isSignedIn()) {
            sdkbox::PluginSdkboxPlay::showAchievements();
        } else {
            sdkbox::PluginSdkboxPlay::signin();
        }

    }
}

#pragma -mark IAPopen

void DNMainMenuLayer::IAPopen() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;

    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();

        return;
    } else {

        //CCLOG("open IAP=======================");
        DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();
        //   DNDataManager::sharedManager()->_gameLayerRef->createConectingPopUp();

        sdkbox::IAP::purchase("remove_ads");

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Remove_Ads_button", "Clicked", "Main_Menu", 1);

    }
}

#pragma -mark onSuccess

void DNMainMenuLayer::onSuccess(const sdkbox::Product &p) {
    int bgCount = 0;

    if (p.id == "com.ghostchillies.superswitchrun.bg2") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(1)) {
            bgCount++;
        }

        DNAcheivementManager::sharedManager()->unloackBg(1);
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        disableLockScreen();
        DNDataManager::sharedManager()->_bgPackRef->checkBg(1);
        DNDataManager::sharedManager()->_bgPackRef->setBg();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("BG_2", "Purchased", "Unlocks", 1);


    } else if (p.id == "com.ghostchillies.superswitchrun.bg3") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(2)) {
            bgCount++;
        }
        DNAcheivementManager::sharedManager()->unloackBg(2);
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        disableLockScreen();
        DNDataManager::sharedManager()->_bgPackRef->checkBg(2);
        DNDataManager::sharedManager()->_bgPackRef->setBg();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("BG_3", "Purchased", "Unlocks", 1);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg4") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(3)) {
            bgCount++;
        }
        DNAcheivementManager::sharedManager()->unloackBg(3);
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        disableLockScreen();

        DNDataManager::sharedManager()->_bgPackRef->checkBg(3);
        DNDataManager::sharedManager()->_bgPackRef->setBg();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("BG_4", "Purchased", "Unlocks", 1);

    } else if (p.id == "com.ghostchillies.superswitchrun.bg5") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(4)) {
            bgCount++;
        }
        DNAcheivementManager::sharedManager()->unloackBg(4);
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        disableLockScreen();
        DNDataManager::sharedManager()->_bgPackRef->checkBg(4);
        DNDataManager::sharedManager()->_bgPackRef->setBg();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("BG_5", "Purchased", "Unlocks", 1);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg6") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(5)) {
            bgCount++;
        }
        DNAcheivementManager::sharedManager()->unloackBg(5);
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        disableLockScreen();
        DNDataManager::sharedManager()->_bgPackRef->checkBg(1);
        DNDataManager::sharedManager()->_bgPackRef->setBg();

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("BG_6", "Purchased", "Unlocks", 1);
    } else if (p.id == "com.ghostchillies.superswitchrun.removeads") {
        //CCLOG("purchase finsished: remove ads");
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        UserDefault::getInstance()->setBoolForKey("isnoAdPackPurchased", true);
        noAdButton->setVisible(false);
        noAdButton->setEnabled(false);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Remove_Ads", "Purchased", "Store", 1);

    } else if (p.id == "com.ghostchillies.superswitchrun.characterpack") {
        //CCLOG("unlock all locked character");
        DNAcheivementManager::sharedManager()->unlockAllCharacter();
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
        characterPackButton->setVisible(false);
        characterPackButton->setEnabled(false);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_pack", "Purchased", "Store", 1);

    } else if (p.id == "com.ghostchillies.superswitchrun.levelspack") {
        DNAcheivementManager::sharedManager()->unlockAllBg();
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
        bgPackButton->setVisible(false);
        bgPackButton->setEnabled(false);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Background_pack", "Purchased", "Store", 1);
    }


    UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + bgCount);

    if (UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) > 4) {
        bgPackButton->setVisible(false);
        bgPackButton->setEnabled(false);
        this->removeChild(bgPackButton);
        UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);

    }


    if (p.id == "com.ghostchillies.superswitchrun.character2") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(1);
        DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(1);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_2", "Purchased", "Unlocks", 1);


    } else if (p.id == "com.ghostchillies.superswitchrun.character3") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(2)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(2);
        DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(2);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_3", "Purchased", "Unlocks", 1);
    } else if (p.id == "com.ghostchillies.superswitchrun.character4") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(3)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(3);
        DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(3);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_4", "Purchased", "Unlocks", 1);

    } else if (p.id == "com.ghostchillies.superswitchrun.character5") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(4)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(4);
        DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(4);

        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Character_5", "Purchased", "Unlocks", 1);
    }


    if (UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) > 3) {
        DNDataManager::sharedManager()->_mainmenu->characterPackButton->setVisible(false);
        UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
        DNDataManager::sharedManager()->_mainmenu->characterPackButton->setEnabled(false);

    }
    if (DNDataManager::sharedManager()->_isCharcterPackMenuCreated) {

        //  DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        DNDataManager::sharedManager()->_characterPack->_purchaseButton->setVisible(false);
        DNDataManager::sharedManager()->_characterPack->_purchaseButton->setEnabled(false);
        DNDataManager::sharedManager()->_characterPack->_unlockLabel->setString("");


    }


}

#pragma -mark onIAP

void DNMainMenuLayer::onIAP(cocos2d::Ref *sender) {

}

#pragma -mark onRequestIAP

void DNMainMenuLayer::onRequestIAP(cocos2d::Ref *sender) {
    sdkbox::IAP::refresh();
}

#pragma -mark onRestoreIAP

void DNMainMenuLayer::onRestoreIAP(cocos2d::Ref *sender) {
    sdkbox::IAP::restore();
}

#pragma -mark onInitialized

void DNMainMenuLayer::onInitialized(bool ok) {
    // CCLOG("%s : %d", __func__, ok);
}


#pragma -mark updateIAP

void DNMainMenuLayer::updateIAP(const std::vector<sdkbox::Product> &products) {

}

#pragma -mark onFailure

void DNMainMenuLayer::onFailure(sdkbox::Product const &p, const std::string &msg) {

    if (strcmp(msg.c_str(), "Unable to buy item (response: 7:Item Already Owned)") == 0) {

        int bgCount = 0;


        if (p.id == "com.ghostchillies.superswitchrun.bg2") {
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(1)) {
                bgCount++;
            }

            DNAcheivementManager::sharedManager()->unloackBg(1);
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            disableLockScreen();
            DNDataManager::sharedManager()->_bgPackRef->checkBg(1);
            DNDataManager::sharedManager()->_bgPackRef->setBg();

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("BG_2", "Purchased", "Unlocks", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.bg3") {
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(2)) {
                bgCount++;
            }
            DNAcheivementManager::sharedManager()->unloackBg(2);
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            disableLockScreen();
            DNDataManager::sharedManager()->_bgPackRef->checkBg(2);
            DNDataManager::sharedManager()->_bgPackRef->setBg();

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("BG_3", "Purchased", "Unlocks", 1);
        } else if (p.id == "com.ghostchillies.superswitchrun.bg4") {
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(3)) {
                bgCount++;
            }
            DNAcheivementManager::sharedManager()->unloackBg(3);
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            disableLockScreen();

            DNDataManager::sharedManager()->_bgPackRef->checkBg(3);
            DNDataManager::sharedManager()->_bgPackRef->setBg();

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("BG_4", "Purchased", "Unlocks", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.bg5") {
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(4)) {
                bgCount++;
            }
            DNAcheivementManager::sharedManager()->unloackBg(4);
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            disableLockScreen();
            DNDataManager::sharedManager()->_bgPackRef->checkBg(4);
            DNDataManager::sharedManager()->_bgPackRef->setBg();

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("BG_5", "Purchased", "Unlocks", 1);
        } else if (p.id == "com.ghostchillies.superswitchrun.bg6") {
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(5)) {
                bgCount++;
            }
            DNAcheivementManager::sharedManager()->unloackBg(5);
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            disableLockScreen();
            DNDataManager::sharedManager()->_bgPackRef->checkBg(1);
            DNDataManager::sharedManager()->_bgPackRef->setBg();

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("BG_6", "Purchased", "Unlocks", 1);
        } else if (p.id == "com.ghostchillies.superswitchrun.removeads") {
            //CCLOG("purchase finsished: remove ads");
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            UserDefault::getInstance()->setBoolForKey("isnoAdPackPurchased", true);
            noAdButton->setVisible(false);
            noAdButton->setEnabled(false);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Remove_Ads", "Purchased", "Store", 1);


        } else if (p.id == "com.ghostchillies.superswitchrun.characterpack") {
            //CCLOG("unlock all locked character");
            DNAcheivementManager::sharedManager()->unlockAllCharacter();
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
            characterPackButton->setVisible(false);
            characterPackButton->setEnabled(false);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Character_pack", "Purchased", "Store", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.levelspack") {
            DNAcheivementManager::sharedManager()->unlockAllBg();
            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
            bgPackButton->setVisible(false);
            bgPackButton->setEnabled(false);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Background_pack", "Purchased", "Store", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.character2") {
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(1);
            DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(1);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Character_2", "Purchased", "Unlocks", 1);


        } else if (p.id == "com.ghostchillies.superswitchrun.character3") {
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(2)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(2);
            DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(2);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Character_3", "Purchased", "Unlocks", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.character4") {
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(3)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(3);
            DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(3);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Character_4", "Purchased", "Unlocks", 1);

        } else if (p.id == "com.ghostchillies.superswitchrun.character5") {
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(4)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(4);
            DNDataManager::sharedManager()->_characterPack->creatingCharacterAnimation(4);

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Character_5", "Purchased", "Unlocks", 1);
        }


        if (UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) > 3) {
            DNDataManager::sharedManager()->_mainmenu->characterPackButton->setVisible(false);
            UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
            DNDataManager::sharedManager()->_mainmenu->characterPackButton->setEnabled(false);

        }

        if (DNDataManager::sharedManager()->_isCharcterPackMenuCreated) {

            DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
            DNDataManager::sharedManager()->_characterPack->_purchaseButton->setVisible(false);
            DNDataManager::sharedManager()->_characterPack->_purchaseButton->setEnabled(false);
            DNDataManager::sharedManager()->_characterPack->_unlockLabel->setString("");


        }


        UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + bgCount);

        if (UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) > 4) {
            bgPackButton->setVisible(false);
            bgPackButton->setEnabled(false);
            this->removeChild(bgPackButton);
            UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);

        }


    }

};

#pragma -mark onCanceled

void DNMainMenuLayer::onCanceled(sdkbox::Product const &p) {

};

#pragma -mark onRestored

void DNMainMenuLayer::onRestored(sdkbox::Product const &p) {
    int bgCount = 0;
    int characterCount = 0;

    //    for (int i=0; i < _products.size(); i++)
    //    {
    //
    //
    //        _productPrice.push_back(_products[i].price.c_str());
    //


    if (p.id == "com.ghostchillies.superswitchrun.bg2") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(1)) {
            UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackBg(1);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg3") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(2)) {
            UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackBg(2);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg4") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(3)) {
            UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackBg(3);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg5") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(4)) {
            UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackBg(4);
    } else if (p.id == "com.ghostchillies.superswitchrun.bg6") {
        if (!DNAcheivementManager::sharedManager()->isBGUnlocked(5)) {
            UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackBg(5);
    } else if (p.id == "com.ghostchillies.superswitchrun.character2") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(1)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(1);

    } else if (p.id == "com.ghostchillies.superswitchrun.character3") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(2)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(2);

    } else if (p.id == "com.ghostchillies.superswitchrun.character4") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(3)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(3);

    } else if (p.id == "com.ghostchillies.superswitchrun.character5") {
        if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(4)) {
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
        }
        DNAcheivementManager::sharedManager()->unloackCharacter(4);

    }
        //        else if(p.id == "com.ghostchillies.superswitchrun.characterpack")
        //        {
        //            DNAcheivementManager::sharedManager()->unlockAllCharacter();
        //        }
    else if (p.id == "com.ghostchillies.superswitchrun.characterpack") {
        // characterPackButton->setVisible(false);
        DNAcheivementManager::sharedManager()->unlockAllCharacter();

        if (characterPackButton) {
            characterPackButton->setVisible(false);
        }
        characterPackButton->setVisible(false);
        UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);

    } else if (p.id == "com.ghostchillies.superswitchrun.levelspack") {
        DNAcheivementManager::sharedManager()->unlockAllBg();
        UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
        if (bgPackButton) {
            bgPackButton->setVisible(false);

        }
    } else if (p.id == "com.ghostchillies.superswitchrun.removeads") {
        UserDefault::getInstance()->setBoolForKey("isnoAdPackPurchased", true);
        if (noAdButton) {
            noAdButton->setVisible(false);
        }



        // }


        if (UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", bgCount) > 4) {
            UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
            bgPackButton->setVisible(false);
        }
        if (UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", characterCount) > 3) {
            characterPackButton->setVisible(false);
            UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
        }

    }
    _restorePurchaseButton->setEnabled(false);
    _restorePurchaseButton->setVisible(false);
    UserDefault::getInstance()->setBoolForKey("restorePurchased", true);
    this->removeChild(_restorePurchaseButton);

};

#pragma -mark onProductRequestSuccess

void DNMainMenuLayer::onProductRequestSuccess(std::vector<sdkbox::Product> const &products) {
    _products = products;
    _productPrice.clear();
    for (int i = 0; i < _products.size(); i++) {
        _productPrice.push_back(products[i].price.c_str());
    }
    UserDefault::getInstance()->setIntegerForKey("BgAmount", _products[0].priceValue);

    updateIAP(products);
};

#pragma -mark onProductRequestFailure

void DNMainMenuLayer::onProductRequestFailure(const std::string &msg) {

};

#pragma -mark onRestoreComplete

void DNMainMenuLayer::onRestoreComplete(bool ok, const std::string &msg) {

};


//handling Touch
#pragma mark - Touch_events

bool DNMainMenuLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {

    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    clock_t t;
    t = clock();
    _timer1 = t;

    _pos1 = touch->getLocation();


    //CCLOG("int time=%f",_timer1);


    _isMoved = false;

    return true;
}

//handling Touch
#pragma mark - Touch_events

void DNMainMenuLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (DNDataManager::sharedManager()->_gameLayerRef->_character->getBoundingBox().containsPoint(touch->getLocation())) {
        return;
    }


    _isMoved = true;
    return;

}

#pragma mark - onTouchEnded

void DNMainMenuLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {

    if (isFBPopupAdded) {
        menu->setEnabled(true);
        arrowMenu->setEnabled(true);
        DNDataManager::sharedManager()->_mainmenu->_menu->setEnabled(true);
        isFBPopupAdded = false;
        Show_friends_score_popup->removeFromParentAndCleanup(true);
        return;
    }

    int time = 10000;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    time = 130000;

#endif

    _pos2 = touch->getLocation();

    clock_t t;
    t = clock();
    _timer2 = t;
    //CCLOG("int time=%f",_timer2);
    //CCLOG("int time=%f",_timer2 - _timer1);
    if (std::abs(_pos1.x - _pos2.x) < 50) {
        if (_timer2 - _timer1 < time) {
            _isMoved = false;
        }
    }

    onTapToStartbuttonClicked();


}

#pragma mark - onTapToStartbuttonClicked

void DNMainMenuLayer::onTapToStartbuttonClicked() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    if (isFBPopupAdded)
        return;

    if (UserDefault::getInstance()->getBoolForKey("userDialogue", false)) {
        UserDefault::getInstance()->setBoolForKey("userDialogue", false);
        return;
    }


    //CCLOG("inside ontaptostart");
    if (DNDataManager::sharedManager()->_isCharcterPackMenuCreated) {
        DNDataManager::sharedManager()->_gameLayerRef->removeCharacterPack();
    } else if (DNDataManager::sharedManager()->_HUDLayerRef->_isStarting && !_isMoved && DNDataManager::sharedManager()->_isBgAvailable) {


        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Play_button", "Clicked", "Main_Menu", 1);

        menu->setEnabled(false);
        DNDataManager::sharedManager()->_mainmenu->_menu->setEnabled(false);
        DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(false);

        gameLogo->runAction(RotateBy::create(0.30, 360));
        gameLogo->runAction(ScaleTo::create(0.30, 0));

        DNDataManager::sharedManager()->_bgPackRef->setBg();
        DNDataManager::sharedManager()->_isinGameScene = true;

        CallFunc *calSelector = CallFunc::create(this, callfunc_selector(DNMainMenuLayer::unloadBgPack));
        this->runAction(Sequence::create(DelayTime::create(0.35), calSelector, NULL));
    }

}


#pragma mark - unloadBgPack

void DNMainMenuLayer::unloadBgPack() {
    UserDefault::getInstance()->setIntegerForKey("PlayCount", UserDefault::getInstance()->getIntegerForKey("PlayCount", 0) + 1);

    //Acheivement

    int playCount = UserDefault::getInstance()->getIntegerForKey("PlayCount", 0);
    switch (playCount) {
        case 1000:
            //sdkbox::PluginSdkboxPlay::showAchievements();
            //unlock achievement Play 1000 times
            // sdkbox::PluginSdkboxPlay::unlockAchievement("Regular");
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(4)) {
                UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->AcheivementXp(40, "Regular");
            DNAcheivementManager::sharedManager()->unloackBg(4);
            DNDataManager::sharedManager()->_gameLayerRef->createPopUpLayer(1);

            break;
        case 5000:
            DNAcheivementManager::sharedManager()->AcheivementXp(100, "Loyalist");
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(2)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }
            DNAcheivementManager::sharedManager()->unloackCharacter(2);
            DNDataManager::sharedManager()->_gameLayerRef->createPopUpLayer(2);


            //unlock achievement Play 5000 times
            // sdkbox::PluginSdkboxPlay::unlockAchievement("Loyalist");
            break;
        case 10000:
            DNAcheivementManager::sharedManager()->AcheivementXp(200, "God-Like");
            if (!DNAcheivementManager::sharedManager()->isCharacterUnlocked(3)) {
                UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);
            }

            DNAcheivementManager::sharedManager()->unloackCharacter(3);
            DNDataManager::sharedManager()->_gameLayerRef->createPopUpLayer(2);
            UserDefault::getInstance()->setIntegerForKey("characterPurchaseCount", UserDefault::getInstance()->getIntegerForKey("characterPurchaseCount", 0) + 1);

            //unlock achievement Play 10000 times
            // sdkbox::PluginSdkboxPlay::unlockAchievement("God-Like");
            break;


        default:
            break;
    }


    auto m_LayerGround = DNDataManager::sharedManager()->_backGroundLayerRef->m_LayerGround1;
    removeChild(_startLabelMaskBg);
    DNDataManager::sharedManager()->_gameLayerRef->removeBGPack();
    DNDataManager::sharedManager()->_gameLayerRef->removeMenuLayer();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    DNDataManager::sharedManager()->_gameSceneRef->showHUDLayer();

    //setting running animation
    DNDataManager::sharedManager()->_gameLayerRef->setRunAnimation();

    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(false);
    CallFunc *calSelector = CallFunc::create(this, callfunc_selector(DNGameScene::enablePauseMenu));

    DNDataManager::sharedManager()->_gameLayerRef->_character->runAction(
            Sequence::create(DelayTime::create(0.3), MoveTo::create(1, Vec2(m_LayerGround->getContentSize().width * 0.25 + DNDataManager::sharedManager()->_gameLayerRef->_character->getContentSize().width / 2, DNDataManager::sharedManager()->_gameLayerRef->_character->getPositionY())), calSelector,
                             NULL));
    DNDataManager::sharedManager()->_gameLayerRef->scheduleUpdate();

    DNDataManager::sharedManager()->_isCharcterPackMenuCreated = true;
    DNDataManager::sharedManager()->_HUDLayerRef->_isStarting = false;

    this->removeChild(_handPointer);


}

#pragma mark - enabledLockScreen

void DNMainMenuLayer::enabledLockScreen() {
    gameLogo->setVisible(false);
    _startLabel->setVisible(false);
    _background->setVisible(true);
    _background->setStretchEnabled(true);
    _lock->setVisible(true);
    _lock->setStretchEnabled(true);
    _purchaseButton->setVisible(true);
    _purchaseButton->setEnabled(true);
    _background2->setVisible(true);
}

#pragma -mark arrowsClicked

void DNMainMenuLayer::arrowsClicked(Ref *sender) {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    cocos2d::MenuItemSprite *arrows = dynamic_cast<cocos2d::MenuItemSprite *>(sender);
    if (arrows->getTag() == 1) {
        if (UserDefault::getInstance()->getIntegerForKey("BGIndex1", 0) > 0) {
            int ind = UserDefault::getInstance()->getIntegerForKey("BGIndex1", 0);
            ind = ind - 1;
            DNDataManager::sharedManager()->_bgPackRef->pageview->setCurSelectedIndex(ind);
            DNDataManager::sharedManager()->_bgPackRef->pageview->scrollToPage(ind);
            UserDefault::getInstance()->setIntegerForKey("BGIndex1", ind);
            _rightArrow->setEnabled(true);
            _rightArrow->setVisible(true);
        } else {
            arrows->setEnabled(false);
            arrows->setVisible(false);
            _rightArrow->setEnabled(true);
            _rightArrow->setVisible(true);
        }

    } else if (arrows->getTag() == 2) {
        if (UserDefault::getInstance()->getIntegerForKey("BGIndex1", 0) < 5) {
            int ind = UserDefault::getInstance()->getIntegerForKey("BGIndex1", 0);
            ind = ind + 1;
            DNDataManager::sharedManager()->_bgPackRef->pageview->setCurSelectedIndex(ind);
            DNDataManager::sharedManager()->_bgPackRef->pageview->scrollToPage(ind);
            DNDataManager::sharedManager()->_bgPackRef->pageview->setIndicatorEnabled(true);
            UserDefault::getInstance()->setIntegerForKey("BGIndex1", ind);
            _leftArrow->setEnabled(true);
            _leftArrow->setVisible(true);
        } else {
            arrows->setEnabled(false);
            arrows->setVisible(false);
            _leftArrow->setEnabled(true);
            _leftArrow->setVisible(true);
        }

    }
    DNDataManager::sharedManager()->_bgPackRef->checkBg(UserDefault::getInstance()->getIntegerForKey("BGIndex1", 0));
}

#pragma mark - disableLockScreen

void DNMainMenuLayer::disableLockScreen() {
    gameLogo->setVisible(true);
    _startLabel->setVisible(true);
    _background->setVisible(false);
    _lock->setVisible(false);
    _purchaseButton->setVisible(false);
    _purchaseButton->setEnabled(false);
    _background2->setVisible(false);
}

#pragma mark - onExitButtonPressed

void DNMainMenuLayer::onExitButtonPressed(EventKeyboard::KeyCode keyCode, Event *event) {

    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        if (UserDefault::getInstance()->getBoolForKey("userDialogue", true)) {
            if (DNDataManager::sharedManager()->_isUserDialogueCreated) {
                exit(0);
            }
            return;
        }


        if (DNDataManager::sharedManager()->_isCharcterPackMenuCreated) {
            DNDataManager::sharedManager()->_gameLayerRef->removeCharacterPack();
            DNDataManager::sharedManager()->_mainmenu->_handPointer->setVisible(true);
            DNDataManager::sharedManager()->_mainmenu->gameLogo->setVisible(true);

        } else if (DNDataManager::sharedManager()->_isDefaultBg) {
            DNDataManager::sharedManager()->_gameLayerRef->createQuitPopUp();
        } else {
            DNDataManager::sharedManager()->_isDefaultBg = true;
            DNDataManager::sharedManager()->_bgPackRef->pageview->setCurrentPageIndex(0);
            UserDefault::getInstance()->setStringForKey("Path", "BG_Assets/Daylight_BG/");
            DNDataManager::sharedManager()->_isBgAvailable = true;
            DNDataManager::sharedManager()->_mainmenu->disableLockScreen();
            DNDataManager::sharedManager()->_bgPackRef->setBg();
        }
    }
}

#pragma mark - onExitNoButtonPressed

void DNMainMenuLayer::onExitNoButtonPressed() {
    this->removeChild(_userDialogue);
    UserDefault::getInstance()->setBoolForKey("userDialogue", false);
}


#pragma -mark purchaseBg

void DNMainMenuLayer::purchaseBg() {
    DNAcheivementManager::sharedManager()->unloackBg(DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex());
    DNDataManager::sharedManager()->_bgPackRef->checkBg(DNDataManager::sharedManager()->_bgPackRef->pageview->getCurrentPageIndex());
}
