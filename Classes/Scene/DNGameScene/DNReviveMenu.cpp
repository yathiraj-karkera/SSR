//
//  DNReviveMenu.cpp
//  DesiNinja
//
//  Created by Yathiraj on 08/08/17.
//
//

#include "DNReviveMenu.hpp"
#include "DNHUDLayer.hpp"
#include "DNDataManager.h"
#include "cocos2d.h"
#include "DNGameSceneLayer.hpp"
#include "DNConstants.h"
#include "SimpleAudioEngine.h"
#include "PluginAdMob/PluginAdMob.h"
#include "DNAcheivementManager.hpp"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#include   "DNGameUtility.hpp"

static std::function<void(const std::string &)> showText = nullptr;

#ifdef SDKBOX_ENABLED

class IMListener : public sdkbox::AdMobListener {
public:
    virtual void adViewDidReceiveAd(const std::string &name) {
        //////CCLOG("adViewDidReceiveAd");

        if (showText)
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
    }

    virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
        ////CCLOG("adViewDidFailToReceiveAdWithError");

        if (DNGameUtility::checkIsNetworkAvailable()) {
            //  sdkbox::PluginAdMob::cache("rewarded");

        }
        if (showText)
            showText(StringUtils::format("%s name=%s, msg=%s", __FUNCTION__, name.c_str(), msg.c_str()));
    }

    virtual void adViewWillPresentScreen(const std::string &name) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->pauseAllEffects();
        audio->pauseBackgroundMusic();


        if (showText)
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
    }

    virtual void adViewDidDismissScreen(const std::string &name) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->resumeAllEffects();
        audio->resumeBackgroundMusic();
        DNDataManager::sharedManager()->_isReviveAdshouldPlay = false;
        if (DNDataManager::sharedManager()->_isFromGameOver) {
#if((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID))

            if (DNDataManager::sharedManager()->_isRewardAwarded) {
                DNDataManager::sharedManager()->_reviveMenu->watchAdSuccess();
            }
            DNDataManager::sharedManager()->_reviveMenu->schedule(schedule_selector(DNReviveMenu::loadingBarProgress), 0.025f);
#endif
        }

        if (showText)
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));

        if (name == "gameover") {
            //  sdkbox::PluginAdMob::cache("gameover");
        }
    }

    virtual void adViewWillDismissScreen(const std::string &name) {
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->resumeAllEffects();
        audio->resumeBackgroundMusic();


        if (DNDataManager::sharedManager()->_isFromGameOver) {
            UserDefault::getInstance()->setBoolForKey("GameMusicMuted", false);
            if (DNDataManager::sharedManager()->_isRewardAwarded) {
                DNDataManager::sharedManager()->_reviveMenu->watchAdSuccess();
            }
            DNDataManager::sharedManager()->_isReviveAdshouldPlay = false;
        }
    }

    virtual void adViewWillLeaveApplication(const std::string &name) {

        if (showText)
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
    }

    virtual void reward(const std::string &name, const std::string &currency, double amount) {
        UserDefault::getInstance()->setBoolForKey("GameMusicMuted", false);
        DNDataManager::sharedManager()->_isRewardAwarded = true;
    }
};

#endif


#pragma -mark Constructor

DNReviveMenu::DNReviveMenu() {

    _fadeLayer = NULL;
    DNDataManager::sharedManager()->_isRevivemenuButtonTapped = false;
    DNDataManager::sharedManager()->_reviveMenu = this;
    DNDataManager::sharedManager()->_isinGameScene = false;
    DNDataManager::sharedManager()->_isFromGameOver = true;
    DNDataManager::sharedManager()->_isRewardAwarded = false;
}

#pragma -mark Destructor

DNReviveMenu::~DNReviveMenu() {


    sdkbox::PluginAdMob::removeListener();
}

#pragma -mark createBackground

DNReviveMenu *DNReviveMenu::createReviveMenu() {
    DNReviveMenu *scene = new DNReviveMenu();
    if (!scene->init()) {
        CC_SAFE_DELETE(scene);
        return NULL;
    }

    scene->autorelease();
    return scene;
}

#pragma -mark Initialization

bool DNReviveMenu::init() {
    if (!Layer::init()) {
        return false;
    }
    sdkbox::PluginAdMob::setListener(new IMListener());
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    _background = cocos2d::Sprite::create("WatchAddScreen/Popup1.png");
    _background->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 1.5 + origin.y));
    addChild(_background, 1000);

    cocos2d::Label *description = cocos2d::Label::createWithTTF("Watch ad to", "fonts/motschcc.ttf", 40);
    description->setColor(cocos2d::Color3B(119, 74, 59));
    description->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2, _background->getContentSize().height * 0.80));
    _background->addChild(description, 1000);

    cocos2d::Label *description1 = cocos2d::Label::createWithTTF("Revive and play", "fonts/motschcc.ttf", 40);
    description1->setColor(cocos2d::Color3B(119, 74, 59));
    description1->setPosition(cocos2d::Vec2(_background->getContentSize().width / 2, _background->getContentSize().height * 0.70));
    _background->addChild(description1, 1000);

    //Progress Bar
    Sprite *progressBgSprite = Sprite::create("SettingScene/volume_bar_empty.png");
    _background->addChild(progressBgSprite, 50);
    progressBgSprite->setPosition(Vec2(_background->getContentSize().width * 0.50, _background->getContentSize().height * 0.50));

    //loading bar
    _adTimeOutBar = cocos2d::ui::LoadingBar::create("SettingScene/volume_bar_empty.png");
    _adTimeOutBar->setName("Loading Bar");
    _adTimeOutBar->loadTexture("SettingScene/volume_bar_fill.png");
    _adTimeOutBar->setPercent(100);
    _adTimeOutBar->setPosition(Vec2(_background->getContentSize().width * 0.50, _background->getContentSize().height * 0.50));
    _background->addChild(_adTimeOutBar, 100);
    this->schedule(schedule_selector(DNReviveMenu::loadingBarProgress), 0.025f);


    cocos2d::MenuItemImage *yesButton = cocos2d::MenuItemImage::create("WatchAddScreen/Yes.png", "WatchAddScreen/Yes.png", CC_CALLBACK_0(DNReviveMenu::showAds, this));
    yesButton->setTag(3);
    yesButton->setPosition(cocos2d::Vec2(_background->getContentSize().width * 0.30, _background->getContentSize().height * 0.25));


    cocos2d::Label *yesButtonlabel = cocos2d::Label::createWithTTF("Yes", "fonts/motschcc.ttf", 30);
    yesButtonlabel->setColor(cocos2d::Color3B(119, 74, 59));
    yesButtonlabel->setPosition(cocos2d::Vec2(yesButton->getContentSize().width / 2, yesButton->getContentSize().height / 2));
    yesButton->addChild(yesButtonlabel, 1000);


    cocos2d::MenuItemImage *quit = cocos2d::MenuItemImage::create("WatchAddScreen/No.png", "WatchAddScreen/No.png", CC_CALLBACK_0(DNReviveMenu::removeReviveMenu, this));
    quit->setTag(4);
    quit->setPosition(cocos2d::Vec2(_background->getContentSize().width * 0.70, _background->getContentSize().height * 0.25));

    cocos2d::Label *quitlabel = cocos2d::Label::createWithTTF("No", "fonts/motschcc.ttf", 30);
    quitlabel->setColor(cocos2d::Color3B(119, 74, 59));
    quitlabel->setPosition(cocos2d::Vec2(quit->getContentSize().width / 2, quit->getContentSize().height / 2));
    quit->addChild(quitlabel, 1000);

    cocos2d::Menu *menu = cocos2d::Menu::create(yesButton, quit, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    menu->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    _background->addChild(menu, 1);
    showReviveMenu();


    return true;
}

void DNReviveMenu::showReviveMenu() {
    //CCLOG("Crash 1 in android============================");
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    _background->runAction(cocos2d::Sequence::create((cocos2d::MoveTo::create(0.1, cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y))), NULL));
    //CCLOG("Crash 4 in android============================");
}

void DNReviveMenu::removeReviveMenu() {
    //CCLOG("Crash 2 in android============================");
    DNDataManager::sharedManager()->_isRevivemenuButtonTapped = true;

    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    this->unschedule(schedule_selector(DNReviveMenu::loadingBarProgress));

    DNDataManager::sharedManager()->_gameLayerRef->removeReviveMenu();
    //CCLOG("Crash 5 in android============================");

    //Analytics
    DNDataManager::sharedManager()->callAnalytics("No_button", "Clicked", "Revive", 1);

}

void DNReviveMenu::hideReviveMenu() {
    //CCLOG("Crash 3 in android============================");
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    _background->runAction(cocos2d::Sequence::create((cocos2d::MoveTo::create(0.1, cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 1.3 + origin.y))), NULL));
    //CCLOG("Crash 6 in android============================");
}


void DNReviveMenu::showAds() {
    hideReviveMenu();
    this->unschedule(schedule_selector(DNReviveMenu::loadingBarProgress));
    if (!DNGameUtility::checkIsNetworkAvailable()) {
        //CCLOG("no internet");
        DNDataManager::sharedManager()->_gameLayerRef->createInternetPopUp();
        DNDataManager::sharedManager()->_isFromReviveAd = true;
        return;
    } else {
        //Analytics
        DNDataManager::sharedManager()->callAnalytics("Yes_button", "Clicked", "Revive", 1);

        if (sdkbox::PluginAdMob::isAvailable("rewarded") && DNDataManager::sharedManager()->_isReviveAdshouldPlay) {
            sdkbox::PluginAdMob::show("rewarded");

        } else {
            DNDataManager::sharedManager()->_gameLayerRef->_noAdAvailable = true;
            DNDataManager::sharedManager()->_gameLayerRef->onNoAdShowVictoryScreen();
            //CCLOG("No Ad is present");
            DNDataManager::sharedManager()->_isFromReviveAd = true;
        }
    }
}

void DNReviveMenu::watchAdSuccess() {

    //============================sdkbox::PluginAdMob::show("gameover");
    DNDataManager::sharedManager()->_isReviveAdshouldPlay = false;
    UserDefault::getInstance()->setIntegerForKey("TotalRevive", UserDefault::getInstance()->getIntegerForKey("TotalRevive", 0) + 1);

    //Acheivement
    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(false);
    DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(false);


    int totalRevive = UserDefault::getInstance()->getIntegerForKey("TotalRevive", 0);
    switch (totalRevive) {
        case 5:
            DNAcheivementManager::sharedManager()->AcheivementXp(20, "First_Aid");

            break;
        case 100:
            DNAcheivementManager::sharedManager()->AcheivementXp(50, "Reviver");
            if (!DNAcheivementManager::sharedManager()->isBGUnlocked(2)) {
                UserDefault::getInstance()->setIntegerForKey("BgPurchaseCount", UserDefault::getInstance()->getIntegerForKey("BgPurchaseCount", 0) + 1);

            }
            DNAcheivementManager::sharedManager()->unloackBg(2);
            DNDataManager::sharedManager()->_gameLayerRef->createPopUpLayer(1);
            break;
        case 500:
            DNAcheivementManager::sharedManager()->AcheivementXp(100, "Walking_Dead");

            break;
        default:
            break;
    }


    DNDataManager::sharedManager()->_isAdWatched = true;
    DNDataManager::sharedManager()->_isRevivemenuButtonTapped = true;

    DNDataManager::sharedManager()->_gameLayerRef->setIdleAnimation();

    DNDataManager::sharedManager()->_gameLayerRef->removeReviveMenu();
}

#pragma -mark fadeInBackground

void DNReviveMenu::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
    }
    _fadeLayer = LayerColor::create(Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    _fadeLayer->retain();
    _fadeLayer->runAction(FadeTo::create(0.5f, FADE_LAYER_OPACITY));
    addChild(_fadeLayer, 1000 - 1);
}

#pragma -mark fadeOutbackground

void DNReviveMenu::fadeOutbackground() {
    if (_fadeLayer) {
        CallFunc *calSelector = CallFunc::create(this, callfunc_selector(DNHUDLayer::removeFadeLayerFromParent));
        _fadeLayer->runAction(Sequence::create(FadeTo::create(0.5f, 0), calSelector, NULL));
    }
}

#pragma -mark removeFadeLayerFromParent

void DNReviveMenu::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        CC_SAFE_RELEASE_NULL(_fadeLayer);
    }
}

#pragma mark -  loadingBarProgress

void DNReviveMenu::loadingBarProgress(float f) {
    if (_adTimeOutBar->getPercent() != 0) {
        _adTimeOutBar->setPercent(_adTimeOutBar->getPercent() - 1);
    } else if (!DNDataManager::sharedManager()->_isRevivemenuButtonTapped) {
        DNDataManager::sharedManager()->_gameLayerRef->removeInternetPopUp();
        removeReviveMenu();
    }
}

