//
//  DNHUDLayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#ifndef DNHUDLayer_hpp
#define DNHUDLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DNPauseMenu.hpp"
#include "DNSettingScene.hpp"
#include "DNReviveMenu.hpp"


class DNHUDLayer : public cocos2d::Layer {
public:
    DNHUDLayer();
    virtual ~DNHUDLayer();
    static DNHUDLayer* createHUD();
    virtual bool init();
 
    //class reference
    DNSettingScene* _settingScene;
    DNReviveMenu* _reviveMenu;
    DNPauseMenu* _pauseMenu;
    
   
    
    //RGB buttons and functions
    cocos2d::Menu* menu1;
    cocos2d::MenuItemImage* _redButton;
    cocos2d::MenuItemImage* _greenButton;
    cocos2d::MenuItemImage* _blueButton;
    void addGreenButton();
    void changeCharacterColorRed();
    void changeCharacterColorGreen();
    void changeCharacterColorBlue();
    
    //level data
    cocos2d::Label *score;
    cocos2d::Label* XPLabel;
    cocos2d::Sprite *levelBar;
    cocos2d::Label* levelLabel;
    cocos2d::ProgressTimer* _levelProgresSlider;
    void  loadLevelDetails();
    
    //Fade Layer
    cocos2d::Layer *_fadeLayer;
    void fadeInBackground();
    void fadeOutbackground();
    void removeFadeLayerFromParent();
    

    bool _isMoved = false;
    bool _isTapAllowed = true;
    bool _isStarting = true;
    cocos2d::Label *dailyObjectiveLabelForWord;
    cocos2d::Label *dailyObjectiveLabelForWall;
    cocos2d::Menu* menu;
    
    cocos2d::Sprite *mashroomBG;
    
    void createReviveMenu();
    void settingButtonTapped();
    void removeSettingMenu();
    
    //green button toggle
    cocos2d::MenuItemToggle *_buttonToggle;
    void changebuttonPosition(Ref* pSender);
    
    
    void showAdsAfterFifteenSeconds(float dt);
    
    //pause menu
    cocos2d::MenuItemImage *_pauseButton;
    cocos2d::Menu *_menu;
    void pauseButtonTapped();
    void playButtonTapped();
    
    void addAlphabetCompleteDialog();
    void addWallComplteDialog();

   //count down
    int _countDown = 3;
    cocos2d::Label* _countDownLabel;
    void update(float tick);
    
};


#endif /* DNHUDLayer_hpp */







