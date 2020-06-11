//
//  DNPauseMenu.hpp
//  DesiNinja
//
//  Created by Yathiraj on 07/08/17.
//
//

#ifndef DNPauseMenu_hpp
#define DNPauseMenu_hpp
#include "cocos2d.h"
#include "DNSettingScene.hpp"

#include <stdio.h>
class DNPauseMenu : public cocos2d::Layer {
public:
    DNPauseMenu();
    virtual ~DNPauseMenu();
    static DNPauseMenu* createPauseMenu();
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    cocos2d::Sprite* _background;
    cocos2d::Menu* _pauseMenu;
    
    void showPauseMenu();
    void hidePauseMenu();
    
    //fade layer
    cocos2d::Layer *_fadeLayer;
    void fadeInBackground();
    void fadeOutbackground();
    void removeFadeLayerFromParent();
    
    void playButtonTapped();
    void quitButtonTapped();
    void replayButtonTapped();
    void analyticsFirebase();
    void menuButtonTapped();
    void settingButtonTapped();
    
    void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
};

#endif /* DNPauseMenu_hpp */
