//
//  DNGameScene.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//


#ifndef DNGameScene_hpp
#define DNGameScene_hpp

#include "cocos2d.h"
#include "DNBackGroundLayer.hpp"
#include "DNHUDLayer.hpp"
#include "DNGameSceneLayer.hpp"
#include "DNPauseMenu.hpp"
#include "DNMainMenuLayer.hpp"

class DNGameSceneLayer;
class DNGameScene : public cocos2d::Scene
{
public:
    
    DNGameScene();
    virtual ~DNGameScene();
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void loadGame();
    void createBackgroundLayer();
    void createGameSceneLayer();
    void createHUDLayer();
    void pauseGame();
    void resumeGame();
    void analyticsFirebase();
    void fadeBackgroundInForMenu();
    void fadeBackgroundOutForMenu();
    void removeFadeLayer();
    void showHUDLayer();
    void hideHUDLayer();
    void enablePauseMenu();
    
   
    //class reference
    DNTutoriallayer *_tutorialLayer;
    DNBackGroundLayer* _background;
    DNHUDLayer* _HUDLayer;
    DNGameSceneLayer* _gameLayer;
    
    
        
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(DNGameScene);
};

#endif // DNGameScene_hpp

