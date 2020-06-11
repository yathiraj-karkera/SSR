//
//  DNSettingScene.hpp
//  DesiNinja
//
//  Created by Yathiraj on 07/08/17.
//
//

#ifndef DNSettingScene_hpp
#define DNSettingScene_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
//#include "UIScene.h"

#include "ui/CocosGUI.h"
class DNSettingScene : public cocos2d::Layer
{
public:
    
    DNSettingScene();
    ~DNSettingScene();
    
    static DNSettingScene* createScene();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    virtual bool init();
    cocos2d::Sprite *_volumeSprite;
    cocos2d::Sprite *_musicSprite;
    
   
    void showSettingMenu();
    void hideSettingMenu();
    void okButtonPressed();
    cocos2d::Sprite* _background;

    cocos2d::Layer *_fadeLayer;
    void fadeInBackground();
    void fadeOutbackground();
    void removeFadeLayerFromParent();
    
    void sliderEvent(Ref* pSender, cocos2d::ui::Slider::EventType type);
    void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
   
    // implement the "static create()" method manually
    CREATE_FUNC(DNSettingScene);
};

#endif /* DNSettingScene */
