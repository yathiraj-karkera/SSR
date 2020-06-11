//
//  UserDialogueLayer.hpp
//  MyGame3
//
//  Created by Yathiraj on 09/09/17.
//
//

#ifndef UserDialogueLayer_hpp
#define UserDialogueLayer_hpp

#include "cocos2d.h"


#include <stdio.h>
class UserDialogueLayer : public cocos2d::Layer {
public:
    UserDialogueLayer();
    virtual ~UserDialogueLayer();
    static UserDialogueLayer* createDialogueMenu();
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
   void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);    

    
        cocos2d::Sprite* _quitMenuBackground;
    
    
        void createQuitDialogue();
        void showDoyouWantToQuitMenu();
        void hideDoyouWantToQuitMenu();
        void removeDoyouWantToQuitMenu();
        void quitGame();
        cocos2d::Layer *_fadeLayer;
        void fadeInBackground();
        void fadeOutbackground();
        void removeFadeLayerFromParent();
        void internetIssuePopUp();
        void removeInternetPopup();
    
    void createLoadingPopUp();
    void connectingPopUp();
    void noAdPopUp();
    bool _isinternetCheck = false;
    bool _isConnecting = false;
};


#endif /* UserDialogueLayer_hpp */
