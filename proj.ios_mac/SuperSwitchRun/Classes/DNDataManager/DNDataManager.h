//
//  DNDataManager.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#ifndef DNDataManager_hpp
#define DNDataManager_hpp
#include "cocos2d.h"
#include "DNGameSceneLayer.hpp"
#include "DNBackGroundLayer.hpp"
#include "DNHUDLayer.hpp"
#include "DNGameScene.hpp"
#include "DNBgPackLayer.hpp"
#include "DNTutoriallayer.hpp"
#include "DNMainMenuLayer.hpp"
#include "DNDOManager.hpp"
#include "DNCharacterPack.hpp"
#include "UserDialogueLayer.hpp"
#include "DNReviveMenu.hpp"
#include "audio/include/AudioEngine.h"



USING_NS_CC;
class DNGameSceneLayer;
class DNGameScene;
class DNDataManager: public Ref
{
  
private:
    
    DNDataManager();
    ~DNDataManager();
  
public:
    bool init(void);
    
    static DNDataManager* sharedManager(void);
    
    //Class reference
    DNGameSceneLayer* _gameLayerRef;
    DNBackGroundLayer* _backGroundLayerRef;
    DNHUDLayer* _HUDLayerRef;
    DNGameScene* _gameSceneRef;
    DNBgPackLayer* _bgPackRef;
    DNTutoriallayer* _tutorialRef;
    DNMainMenuLayer* _mainmenu;
    DNDOManager* _doManager;
    DNCharacterPack *_characterPack;
    UserDialogueLayer* _dialogueLayer;
    DNReviveMenu* _reviveMenu;
 
    //gameScene values
    int _score = 0;
    int _mashroomCount = 0;
    int _WallPassForDO = 0;
    int _colorCode = 0;
    int _totalNoOfWallPassForDO = 0;
    int buttonNumber = 1;
    
    bool isInPauseMenu = false;
    bool isButtonTutorial = false;
    bool _isReplayButtonTapped = false;
    bool _isCharcterPackMenuCreated = false;
    bool _isUserDialogueCreated = false;
    bool _isBgAvailable = true;
    bool _isDefaultBg ;
    bool _isinGameScene = false;
    bool _isvictoryScreenCreated = false;
    bool _isDailyObjectiveMenuPresent = true;
    
    
    bool _isPauseButtoncreated = false;
    
    //Revivemenu
    bool _isReviveButtoncreated = false;
    bool _isRevivemenuButtonTapped = false;
    
    bool _isFromGameOver = true;
    bool _isGreenbuttonIntroduced = false;
    bool _isMultitouch = false;
    
    //Game speed
    float _gameSpeed;
    float _groundSpeed;
    float _BuildingLayer1Speed;
    float _BuildingLayer2Speed;
    float _cloudSpeed;
    
    //tutorial
    bool _isTutorialPlayed = false;
    bool getTutorialPlayed();
    void setTutorialPlayed(bool isTutorialPlayed);
    bool isgreenButtonTutorial = false;
    
    //mushroom
    bool _isMashroomTutorialPlayed = false;
    bool getMashroomTutorialPlayed();
    void setMashroomTutorialPlayed(bool isMashroomPlayed);

    //Ad
    bool _isRewardAwarded = false;
    bool _isFromReviveAd = false;
    bool _isAdWatched = false;
    bool _isReviveAdshouldPlay = true;
    
    bool previouslyPurchased = false;
    
    //sound function
    void play2dSound(std::string soundName);
    
    //analytics
    void callAnalytics(std::string parameter,std::string action, std::string logText,int kFIRParameterValue);
    
    //Achievements
   std::vector<ValueMap> AchievementAray;
    
    float alphaPosX;
    };

#endif /* DNDataManager_hpp */
