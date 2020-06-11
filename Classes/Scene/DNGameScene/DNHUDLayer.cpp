//
//  DNHUDLayer.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNHUDLayer.hpp"
#include "cocos2d.h"
#include "DNGameScene.hpp"
#include "DNDataManager.h"
#include "DNConstants.h"
#include "DNPauseMenu.hpp"
#include "DNSettingScene.hpp"
#include "DNReviveMenu.hpp"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include  "DNGameUtility.hpp"
//#include "PluginFirebase/PluginFirebase.h"
#include "PluginAdMob/PluginAdMob.h"
#include "audio/include/AudioEngine.h"

#pragma -mark Constructor

DNHUDLayer::DNHUDLayer() {

    _isStarting = true;
    DNDataManager::sharedManager()->_HUDLayerRef = this;
    _fadeLayer = NULL;

}

#pragma -mark Destructor

DNHUDLayer::~DNHUDLayer() {

}

#pragma -mark createHUD

DNHUDLayer *DNHUDLayer::createHUD() {
    DNHUDLayer *scene = new DNHUDLayer();
    if (!scene->init()) {
        CC_SAFE_DELETE(scene);
        return NULL;
    }

    scene->autorelease();
    return scene;
}

#pragma -mark Initialization

bool DNHUDLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    loadLevelDetails();

    //death Ad
    if (DNGameUtility::checkIsNetworkAvailable()) {
        if (UserDefault::getInstance()->getIntegerForKey("deathCount") % 3 == 0 && UserDefault::getInstance()->getIntegerForKey("deathCount") > 0) {
            //  sdkbox::PluginAdMob::cache("gameover");
        }

    }

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    if (!UserDefault::getInstance()->getBoolForKey("DailyObjectiveAlphaComplete", false)) {
        //label
        dailyObjectiveLabelForWord = Label::createWithTTF("You completed the word collecting challenge", "fonts/motschcc.ttf", 20);
        this->addChild(dailyObjectiveLabelForWord, 110);
        dailyObjectiveLabelForWord->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.75));
        dailyObjectiveLabelForWord->setColor(Color3B(250, 167, 14));
        dailyObjectiveLabelForWord->enableOutline(Color4B(0, 0, 0, 200), 2);
        dailyObjectiveLabelForWord->setVisible(false);
    }

    if (!UserDefault::getInstance()->getBoolForKey("DailyObjectiveWallComplete", false)) {
        //label
        dailyObjectiveLabelForWall = Label::createWithTTF("You completed the wall passing challenge", "fonts/motschcc.ttf", 20);
        this->addChild(dailyObjectiveLabelForWall, 110);
        dailyObjectiveLabelForWall->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.75));
        dailyObjectiveLabelForWall->setColor(Color3B(250, 167, 14));
        dailyObjectiveLabelForWall->enableOutline(Color4B(0, 0, 0, 200), 2);
        dailyObjectiveLabelForWall->setVisible(false);
    }

    //level bar
    levelBar = cocos2d::Sprite::create("GameScene/LevelCircularBox.png");
    levelBar->setPosition(Vec2(origin.x + visibleSize.width * 0.9f, origin.y + visibleSize.height * 0.90));
    this->addChild(levelBar);

    //level label
    levelLabel = cocos2d::Label::createWithTTF(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("Level", 0) + 1), "fonts/motschcc.ttf", 25);
    levelLabel->setColor(Color3B(119, 74, 59));
    levelLabel->setPosition(cocos2d::Vec2(levelBar->getContentSize().width / 2, levelBar->getContentSize().height / 2));
    levelBar->addChild(levelLabel, 1000);

    //xp bar
    cocos2d::Sprite *xpBar = cocos2d::Sprite::create("GameScene/Level_text_box.png");
    xpBar->setPosition(Vec2(levelBar->getContentSize().width / 2, -xpBar->getContentSize().height * 0.75));
    levelBar->addChild(xpBar);

    //xp label
    XPLabel = cocos2d::Label::createWithTTF(StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("XP", 0)) + " / " + StringUtils::toString(UserDefault::getInstance()->getIntegerForKey("MaxXp", 0)), "fonts/motschcc.ttf", 10);
    XPLabel->setPosition(cocos2d::Vec2(xpBar->getContentSize().width / 2, xpBar->getContentSize().height / 2));
    xpBar->addChild(XPLabel, 1000);


    //Progress Bar
    Sprite *progressBgSprite = Sprite::create("GameScene/LevelCircularBox_LoadingWhite.png");
    levelBar->addChild(progressBgSprite, 50);
    progressBgSprite->setPosition(Vec2(levelBar->getContentSize().width * 0.50, levelBar->getContentSize().height * 0.50));

    //level progress bar
    Sprite *progressBar = Sprite::create("GameScene/LevelCircularBox_Loading.png");
    _levelProgresSlider = ProgressTimer::create(progressBar);
    _levelProgresSlider->setType(ProgressTimer::Type::RADIAL);
    levelBar->addChild(_levelProgresSlider, 100);
    _levelProgresSlider->setPosition(Vec2(levelBar->getContentSize().width * 0.50, levelBar->getContentSize().height * 0.50));
    _levelProgresSlider->setReverseDirection(true);
    float previouslevelxp = UserDefault::getInstance()->getIntegerForKey("PreviousLevelXp", 0);
    float xp = UserDefault::getInstance()->getIntegerForKey("XP", 0);
    float maxxp = UserDefault::getInstance()->getIntegerForKey("MaxXp", 0);
    float percent = ((xp - previouslevelxp) / (maxxp - previouslevelxp)) * 100;
    _levelProgresSlider->setPercentage(percent);


    //Score
    int sScore = DNDataManager::sharedManager()->_score;
    score = Label::createWithTTF(StringUtils::toString(sScore), "fonts/motschcc.ttf", 50);
    score->setPosition(Vec2(this->getContentSize().width * 0.60, origin.y + visibleSize.height * 0.90));
    this->addChild(score, 1000);


    //1Up
    mashroomBG = cocos2d::Sprite::create("GameScene/Mashroombox.png");
    mashroomBG->setPosition(Vec2(origin.x + visibleSize.width * 0.40, origin.y + visibleSize.height - mashroomBG->getContentSize().height));
    this->addChild(mashroomBG);
    mashroomBG->setVisible(false);

    cocos2d::Sprite *mashroom = cocos2d::Sprite::create("GameScene/Mashroom.png");
    mashroom->setPosition(Vec2(mashroomBG->getContentSize().width * 0.50, mashroomBG->getContentSize().height / 2));
    mashroomBG->addChild(mashroom);

    mashroom->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.30, 1), ScaleTo::create(0.30, 0.50), NULL)));
    mashroomBG->setVisible(false);


    //Adding RGB Button for GamePlay
    _redButton = cocos2d::MenuItemImage::create("GameScene/red_normal.png", "GameScene/red_pressed.png", CC_CALLBACK_0(DNHUDLayer::changeCharacterColorRed, this));
    _redButton->setTag(1);

    cocos2d::MenuItemImage *_redButton1 = cocos2d::MenuItemImage::create("GameScene/red_normal.png", "GameScene/red_pressed.png");
    _redButton1->setPosition(Vec2(_redButton->getContentSize().width / 2, _redButton->getContentSize().height / 2));

    _greenButton = cocos2d::MenuItemImage::create("GameScene/green_normal.png", "GameScene/green_pressed.png", CC_CALLBACK_0(DNHUDLayer::changeCharacterColorGreen, this));
    _greenButton->setTag(2);

    _blueButton = cocos2d::MenuItemImage::create("GameScene/blue_normal.png", "GameScene/blue_pressed.png", CC_CALLBACK_0(DNHUDLayer::changeCharacterColorBlue, this));
    _blueButton->setTag(3);

    _redButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.10, origin.y + _redButton->getContentSize().width * 0.5));
    _greenButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.75, origin.y + _greenButton->getContentSize().height * 0.5));
    _blueButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.90, origin.y + _blueButton->getContentSize().height * 0.5));


    //Adding Menu

    _greenButton->setVisible(false);
    _greenButton->setEnabled(false);
    _menu = Menu::create(_redButton, _greenButton, _blueButton, NULL);
    _menu->setPosition(Vec2::ZERO);
    _menu->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_menu, 1);

    _pauseButton = cocos2d::MenuItemImage::create("GameScene/Pause_Pressed.png", "GameScene/Pause_Pressed.png", CC_CALLBACK_0(DNHUDLayer::pauseButtonTapped, this));
    _pauseButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.1f,origin.y + visibleSize.height * 0.90));

    menu1 = Menu::create(_pauseButton, NULL);
    menu1->setPosition(Vec2::ZERO);
    menu1->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menu1, 10000);

    //adding toggle button
    _buttonToggle = MenuItemToggle::createWithTarget(this,
                                                     menu_selector(DNHUDLayer::changebuttonPosition),
                                                     MenuItemSprite::create(
                                                             Sprite::create("GameScene/Joystick_Button.png"),
                                                             Sprite::create("GameScene/Joystick_Button.png")),
                                                     MenuItemSprite::create(
                                                             Sprite::create("GameScene/Joystick_Button_copy.png"),
                                                             Sprite::create("GameScene/Joystick_Button_copy.png")),
                                                     NULL);
    _buttonToggle->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.10, origin.x + visibleSize.height * 0.5));
    _buttonToggle->setSelectedIndex(1);

    _buttonToggle->setEnabled(false);
    _buttonToggle->setVisible(false);

    menu = Menu::create(_buttonToggle, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menu, 1);

    return true;
}

#pragma -mark changebuttonPosition

void DNHUDLayer::changebuttonPosition(Ref *pSender) {

    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    MenuItemToggle *button = dynamic_cast<MenuItemToggle *>(pSender);
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    switch (button->getSelectedIndex()) {
        case 0:
            DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.25, DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->getPositionY()));
            break;
        case 1:
            DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->setPosition(cocos2d::Vec2(origin.x + visibleSize.width * 0.75, DNDataManager::sharedManager()->_HUDLayerRef->_greenButton->getPositionY()));
            break;


        default:
            break;
    }


}

#pragma -mark addGreenButton

void DNHUDLayer::addGreenButton() {
    _greenButton->setVisible(true);
    _greenButton->setEnabled(true);
}

#pragma -mark loadLevelDetails

void DNHUDLayer::loadLevelDetails() {
    //loading data from plist
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/LevelDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());

    ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr.clear();
    _typeOneDetailsArr = rootDict["Level"].asValueVector();

    int level = UserDefault::getInstance()->getIntegerForKey("Level", 0);
    if (level == 9) {
        return;
    }
    ValueMap typeDetailDict = _typeOneDetailsArr.at(level).asValueMap();

    //updating values
    int points = typeDetailDict["Score"].asInt();
    UserDefault::getInstance()->setIntegerForKey("Points", points);
    UserDefault::getInstance()->setIntegerForKey("XPAwarded", typeDetailDict["XPAwarded"].asInt());
    UserDefault::getInstance()->setIntegerForKey("MaxXp", typeDetailDict["XP"].asInt());
    if (UserDefault::getInstance()->getIntegerForKey("XP", 0) > typeDetailDict["XP"].asInt()) {
        UserDefault::getInstance()->setIntegerForKey("Level", level + 1);
        ValueMap typeDetailDict1 = _typeOneDetailsArr.at(level).asValueMap();
        UserDefault::getInstance()->setIntegerForKey("PreviousLevelXp", typeDetailDict1["XP"].asInt());
        loadLevelDetails();
    }

}


#pragma -mark changeCharacterColor

void DNHUDLayer::changeCharacterColorRed() {
    //  DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    if (_isTapAllowed) {
        if (DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated) {
            DNDataManager::sharedManager()->setMashroomTutorialPlayed(true);
            DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated = false;
            DNDataManager::sharedManager()->_gameLayerRef->removeTutorialLayer();
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(true);
        }
        if (!_isStarting) {
            if (!DNDataManager::sharedManager()->getTutorialPlayed()) {
                if (DNDataManager::sharedManager()->isButtonTutorial) {
                    return;
                }

            }
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Red_Run", true);
        } else {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Red_Idle", true);
        }
        DNDataManager::sharedManager()->buttonNumber = 1;
        DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(1);

        //checking for tutorial
        if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
            DNDataManager::sharedManager()->_gameLayerRef->removeTutorialLayer();
            DNDataManager *dataManager = DNDataManager::sharedManager();

            dataManager->_gameLayerRef->setIdleAnimation();

            dataManager->_HUDLayerRef->_buttonToggle->setEnabled(true);
            dataManager->_HUDLayerRef->_buttonToggle->setVisible(true);
            dataManager->_isinGameScene = true;

            DNDataManager::sharedManager()->_isGreenbuttonIntroduced = false;
            DNDataManager::sharedManager()->_gameLayerRef->startCountDown();
            UserDefault::getInstance()->setBoolForKey("greenbuttonTutorial", true);
        }


    } else if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
        _isTapAllowed = true;
    }
}

#pragma -mark changeCharacterColor

void DNHUDLayer::changeCharacterColorGreen() {

    // DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    if (_isTapAllowed) {
        if (DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated) {
            DNDataManager::sharedManager()->setMashroomTutorialPlayed(true);
            DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated = false;
            DNDataManager::sharedManager()->_gameLayerRef->removeTutorialLayer();
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(true);
        }

        if (!_isStarting) {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Green_Run", true);
        } else {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Green_Idle", true);
        }
        DNDataManager::sharedManager()->buttonNumber = 2;
        DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(2);


        if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
            DNDataManager *dataManager = DNDataManager::sharedManager();
            dataManager->_gameLayerRef->removeTutorialLayer();
            dataManager->_gameLayerRef->setIdleAnimation();

            dataManager->_HUDLayerRef->_buttonToggle->setEnabled(true);
            dataManager->_HUDLayerRef->_buttonToggle->setVisible(true);
            dataManager->_isinGameScene = true;
            DNDataManager::sharedManager()->_isGreenbuttonIntroduced = false;
            dataManager->_gameLayerRef->startCountDown();
            UserDefault::getInstance()->setBoolForKey("greenbuttonTutorial", true);
        }


    } else if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
        _isTapAllowed = true;
    }
}

#pragma -mark changeCharacterColor

void DNHUDLayer::changeCharacterColorBlue() {
    //  DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");

    if (_isTapAllowed) {
        if (DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated) {
            DNDataManager::sharedManager()->setMashroomTutorialPlayed(true);
            DNDataManager::sharedManager()->_gameLayerRef->_mashroomTutorialCreated = false;
            DNDataManager::sharedManager()->_gameLayerRef->removeTutorialLayer();
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setEnabled(true);
            DNDataManager::sharedManager()->_HUDLayerRef->_pauseButton->setVisible(true);
        }


        if (!_isStarting) {
            if (!DNDataManager::sharedManager()->getTutorialPlayed()) {
                if (DNDataManager::sharedManager()->isButtonTutorial) {
                    DNDataManager::sharedManager()->_tutorialRef->tutorialForControl();
                    DNDataManager::sharedManager()->_gameLayerRef->_tutorialLayer->fadeOutbackground();
                    DNDataManager::sharedManager()->isButtonTutorial = false;
                }

            }
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Blue_Run", true);
        } else {
            DNDataManager::sharedManager()->_gameLayerRef->characterRunAnimation->setAnimation(1, "Blue_Idle", true);

        }
        DNDataManager::sharedManager()->buttonNumber = 3;
        DNDataManager::sharedManager()->_gameLayerRef->_character->setTag(3);


        if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
            DNDataManager *dataManager = DNDataManager::sharedManager();
            dataManager->_gameLayerRef->removeTutorialLayer();


            dataManager->_gameLayerRef->setIdleAnimation();

            dataManager->_HUDLayerRef->_buttonToggle->setEnabled(true);
            dataManager->_HUDLayerRef->_buttonToggle->setVisible(true);
            dataManager->_isinGameScene = true;
            DNDataManager::sharedManager()->_isGreenbuttonIntroduced = false;
            DNDataManager::sharedManager()->_gameLayerRef->startCountDown();
            UserDefault::getInstance()->setBoolForKey("greenbuttonTutorial", true);
        }


    } else if (!UserDefault::getInstance()->getBoolForKey("greenbuttonTutorial", false) && DNDataManager::sharedManager()->_isGreenbuttonIntroduced) {
        _isTapAllowed = true;
    }
}

#pragma -mark showAdsAfterFifteenSeconds

void DNHUDLayer::showAdsAfterFifteenSeconds(float dt) {
    // "After 15 second");
    if (sdkbox::PluginAdMob::isAvailable("gameover")) {
        this->unschedule(CC_SCHEDULE_SELECTOR(DNHUDLayer::showAdsAfterFifteenSeconds));
    }
}

#pragma -mark pauseButtonTapped

void DNHUDLayer::pauseButtonTapped() {
    // LocalNotification::show("Hello Notification!", 5, 1);
    if (DNDataManager::sharedManager()->getTutorialPlayed()) {
        this->schedule(CC_SCHEDULE_SELECTOR(DNHUDLayer::showAdsAfterFifteenSeconds), 5);

        if (!DNDataManager::sharedManager()->_isPauseButtoncreated) {
            DNDataManager::sharedManager()->_isPauseButtoncreated = true;

            //Analytics
            DNDataManager::sharedManager()->callAnalytics("Pause_button", "Clicked", "Gameplay_Pause", 1);


            DNDataManager::sharedManager()->_isinGameScene = false;
            DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
            _pauseButton->setEnabled(false);
            _pauseButton->setVisible(false);

            fadeInBackground();

            _pauseMenu = DNPauseMenu::createPauseMenu();
            this->addChild(_pauseMenu, _fadeLayer->getLocalZOrder() + 2);

            _pauseMenu->showPauseMenu();

            DNDataManager::sharedManager()->_gameLayerRef->setIdleAnimation();

            DNDataManager::sharedManager()->_gameSceneRef->pauseGame();
        }
    }
}


#pragma -mark playButtonTapped

void DNHUDLayer::playButtonTapped() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    _pauseButton->setVisible(true);
    _pauseButton->setEnabled(true);
    _pauseMenu->hidePauseMenu();
    DNDataManager::sharedManager()->_isPauseButtoncreated = false;

    fadeOutbackground();
    DNDataManager::sharedManager()->_gameLayerRef->startCountDown();
    this->removeChild(_pauseMenu);

    if (_settingScene) {
        this->removeChild(_settingScene);
    }
}

#pragma -mark fadeInBackground

void DNHUDLayer::fadeInBackground() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
    }
    _fadeLayer = LayerColor::create(Color4B(FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, FADE_LAYER_COLOUR, 0));
    _fadeLayer->retain();
    _fadeLayer->runAction(FadeTo::create(0.5f, FADE_LAYER_OPACITY));
    this->addChild(_fadeLayer, 9999995);
}

#pragma -mark fadeOutbackground

void DNHUDLayer::fadeOutbackground() {
    if (_fadeLayer) {
        CallFunc *calSelector = CallFunc::create(this, callfunc_selector(DNHUDLayer::removeFadeLayerFromParent));
        _fadeLayer->runAction(Sequence::create(FadeTo::create(0.5f, 0), calSelector, NULL));
    }
}

#pragma -mark removeFadeLayerFromParent

void DNHUDLayer::removeFadeLayerFromParent() {
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();

    }
}

#pragma -mark update

void DNHUDLayer::update(float dt) {
    if (_countDown >= 0) {
        _countDownLabel->setString(StringUtils::toString(_countDown));
        _countDown--;

    } else {
        fadeOutbackground();
        removeChild(_countDownLabel);
        DNDataManager::sharedManager()->_gameSceneRef->resumeGame();
        unscheduleUpdate();
    }
}

#pragma -mark settingButtonTapped

void DNHUDLayer::settingButtonTapped() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    _pauseMenu->hidePauseMenu();
    _settingScene = DNSettingScene::createScene();
    this->addChild(_settingScene, _fadeLayer->getLocalZOrder() + 2);
    _settingScene->showSettingMenu();

}

#pragma -mark removeSettingMenu

void DNHUDLayer::removeSettingMenu() {
    DNDataManager::sharedManager()->play2dSound("Sound/UI_Button.mp3");
    _settingScene->hideSettingMenu();
    DNDataManager::sharedManager()->isInPauseMenu = true;
    removeChild(_settingScene);
    _pauseMenu->showPauseMenu();

}

#pragma -mark createReviveMenu

void DNHUDLayer::createReviveMenu() {
    _pauseButton->setEnabled(false);
    _pauseButton->setVisible(false);

    fadeInBackground();

    _reviveMenu = DNReviveMenu::createReviveMenu();
    this->addChild(_reviveMenu, _fadeLayer->getLocalZOrder() + 2);

    _reviveMenu->showReviveMenu();
    DNDataManager::sharedManager()->_gameSceneRef->pauseGame();
}

#pragma mark - DialyObjective dialog

void DNHUDLayer::addAlphabetCompleteDialog() {
    // if(charLabel == NULL)
    {
        //creating pop up for word collection
        if (UserDefault::getInstance()->getIntegerForKey("dailyChallengeCount", 0) == 0) {
            UserDefault::getInstance()->setIntegerForKey("dailyChallengeCount", 1);
            UserDefault::getInstance()->setIntegerForKey("XP", UserDefault::getInstance()->getIntegerForKey("XP", 0) + 20);
        } else if (UserDefault::getInstance()->getIntegerForKey("dailyChallengeCount", 0) == 1) {
            UserDefault::getInstance()->setBoolForKey("DailyObjectiveDone", true);
            UserDefault::getInstance()->setIntegerForKey("dailyChallengeCount", 0);
            UserDefault::getInstance()->setIntegerForKey("XP", UserDefault::getInstance()->getIntegerForKey("XP", 0) + 30);
        }

        dailyObjectiveLabelForWord->setVisible(true);

        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        //runaction to move the character label
        //   MoveTo *move = MoveTo::create(5, Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height));

        Sequence *finSeq = Sequence::create(DelayTime::create(3), FadeOut::create(0.1), NULL);
        dailyObjectiveLabelForWord->runAction(finSeq);
    }

}

void DNHUDLayer::addWallComplteDialog() {
    //if(charLabel == NULL)
    {
        //pop up for wall passing
        if (UserDefault::getInstance()->getIntegerForKey("dailyChallengeCount", 0) == 0) {
            UserDefault::getInstance()->setIntegerForKey("dailyChallengeCount", 1);
            UserDefault::getInstance()->setIntegerForKey("XP", UserDefault::getInstance()->getIntegerForKey("XP", 0) + 20);
        } else if (UserDefault::getInstance()->getIntegerForKey("dailyChallengeCount", 0) == 1) {
            UserDefault::getInstance()->setBoolForKey("DailyObjectiveDone", true);
            UserDefault::getInstance()->setIntegerForKey("dailyChallengeCount", 0);
            UserDefault::getInstance()->setIntegerForKey("XP", UserDefault::getInstance()->getIntegerForKey("XP", 0) + 30);
        }

        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        dailyObjectiveLabelForWall->setVisible(true);

        //runaction to move the character label
        // MoveTo *move = MoveTo::create(5, Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height));

        Sequence *finSeq = Sequence::create(DelayTime::create(3), FadeOut::create(0.1), NULL);
        dailyObjectiveLabelForWall->runAction(finSeq);
    }

}

