//
//  DNGameSceneLayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#ifndef DNGameSceneLayer_hpp
#define DNGameSceneLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DNObstecleSprite.hpp"
#include "DNCharacterSprite.hpp"
#include <spine/spine-cocos2dx.h>
#include "DNBgPackLayer.hpp"
#include "DNMashroomSprite.hpp"
#include "DNTutoriallayer.hpp"
#include "DNDOCharLabel.hpp"
#include "DNMainMenuLayer.hpp"
#include "DNReviveMenu.hpp"
#include "DNVictoryMenu.hpp"
#include "DNCharacterPack.hpp"
#include "UserDialogueLayer.hpp"
#include "DNDataManager.h"


using namespace spine;
class DNDataManager;
class DNGameSceneLayer : public cocos2d::Layer {
public:
    DNGameSceneLayer();
    virtual ~DNGameSceneLayer();
    static DNGameSceneLayer* createGameLayer();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //Class reference
    DNBgPackLayer* _bgPackLayer;
    DNMashroomSprite* _mashroom;
    DNDOCharLabel* _charLabel;
    DNTutoriallayer *_tutorialLayer;
    DNMainMenuLayer* _mainMenu;
    DNReviveMenu* _reviveMenu;
    DNVictoryMenu* _victoryMenu;
    DNCharacterPack* _characterPack;
    UserDialogueLayer* _userDialogue;
    DNDataManager *dataManager;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    //tutorial
    bool _tutorialCreated = false;
    bool _mashroomTutorialCreated = false;
    
    Sprite* _sheild;
    
    cocos2d::Vector<DNObstecleSprite*>obstecleVector;
    cocos2d::Vector<spine::SkeletonAnimation*>wallVector;
    cocos2d::Vector<DNObstecleSprite*>tutorialVector;
    cocos2d::Sprite *_character;
    
    
    
    bool _isStarting = true;
    
    ValueVector _walls;
    ValueVector _doType1CharArr;
    int _index = 0;
    
    spine::SkeletonAnimation *characterRunAnimation;
    
    bool isMashroomcreated = false;
    bool isAlphabetCreated = false;
    int _wallCount = 0;
    int _previousColor = 1;
    bool _noAdAvailable = true;
    int redWallCount = 0;
    int greenWallCount = 0;
    int blueWallCount = 0;
    int _maxmushroom = 2;
    int _mushroomcreated = 0;
    int _wallDistance1 = 300;
    int _wallDistance2 = 350;
    bool _isValidScore ;
 
    cocos2d::Label* _countDown;
    int _countvalue = 3;
    void timer(float d);
    
    Label *charLabel = NULL;
    
    void update(float dt);
    
    bool isBothAreCollided(cocos2d::Node *objA, cocos2d::Node *objB);
    void doScoreUpdation(DNObstecleSprite *currentObstacle);
    void updateObstacleMovments(float dtSpeed);
    void doDeathAction();
    void updateTutorialsActions(float dtSpeed);
    void updateMashroomMovements(float dtSpeed);
    void updateMushroomPositionAndCheckCollision(float dtSpeed);
    
    void createCharacter();
    void createObstecles();
    void createBGPack();
    void removeBGPack();
    void startCountDown();
    void createMashroom();
    void createTutorial();
    void removeTutorialLayer();
    void setGamePositionAfterRevive();
    void createInternetPopUp();
    void removeInternetPopUp();
    void createConectingPopUp();
    void createReviveMenu();
    void removeReviveMenu();
    void createMainMenuLayer();
    void removeMenuLayer();
    void createVictoryScren();
    void removeVictoryScren();
    void updateLevelBar();
    void replayGame();
    void createCharacterPack();
    void removeCharacterPack();
    void moveObstecles();
    void onNoAdShowVictoryScreen();
    void removeNoAdPopUp();
    void createPopUpLayer(int b);
    void removePopUpLayer();
    void createQuitPopUp();
    
    void setIdleAnimation();
    void setRunAnimation();
    
    void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    float getDistanceBetweenFloat(float x, float y);
    
    
};


#endif /* DNGameSceneLayer_hpp */
