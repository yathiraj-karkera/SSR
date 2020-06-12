//
//  DNMainMenuLayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 21/08/17.
//
//

#ifndef DNMainMenuLayer_hpp
#define DNMainMenuLayer_hpp

#include <stdio.h>
#include "UserDialogueLayer.hpp"
#include "PluginIAP/PluginIAP.h"
#include "PluginFacebook/PluginFacebook.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"



class DNMainMenuLayer : public cocos2d::Layer, public sdkbox::FacebookListener, sdkbox::IAPListener, sdkbox::SdkboxPlayListener {

public:
    DNMainMenuLayer();
    virtual ~DNMainMenuLayer();
    static DNMainMenuLayer* createMeinMenu();
    virtual bool init();
    void IAPopen();
    void showAchievement();
    void showLeaderBoard();
    void showPopUpForFacebookLeaderboard();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    bool _isMoved = false;
    bool _isTapAllowed = true;
    cocos2d::Sprite* gameLogo;
    cocos2d::Sprite* _handPointer;
    cocos2d::MenuItemSprite* _leftArrow;
    cocos2d::MenuItemSprite* _rightArrow;
    cocos2d::Label* _startLabel;
    cocos2d::MenuItemSprite* _startLabelMaskBg;
    cocos2d::Sprite* _background;
    cocos2d::Sprite* _background2;
    cocos2d::Sprite* _lock;
    cocos2d::MenuItemImage* _purchaseButton;
    cocos2d::Label* unlockDescription;
    cocos2d::Menu *menu;
    cocos2d::Menu *_menu;
    cocos2d::Menu* arrowMenu;
    cocos2d::MenuItemImage* _chracterPackMenu;
    
    cocos2d::Menu * _iapMenu;
    std::vector<sdkbox::Product> _products;
    cocos2d::MenuItemSprite* bgPackButton;
    cocos2d::MenuItemSprite* characterPackButton;
    cocos2d::MenuItemSprite* noAdButton;
    cocos2d::MenuItemSprite* _rateButton;
    cocos2d::MenuItemSprite* _restorePurchaseButton;
    
    cocos2d::Point _pos1;
    cocos2d::Point _pos2;
  
    
    void analyticsFirebase();
    void unlockCharacter(bool purchase);
    void unlockCharacter1();
    void unLockBackGround();
    void purchaseOneByOneBg();
    void showFriendsScoreAndName();
    void rateUsBtnClick();
    void onIAPRestoreClick();
    void arrowsClicked(Ref* sender);
    void loadCharacterPack();
    UserDialogueLayer* _userDialogue;
    
    bool isFBPopupAdded=false;
    
    void inviteFriends();

    cocos2d::LayerColor *_fadeLayer;
    
    void unloadBgPack();
    void enabledLockScreen();
    void disableLockScreen();
    void pleaseWaitPopUp();
    void onTapToStartbuttonClicked();
     void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onExitNoButtonPressed();
    void purchaseBg();
    
    std::vector<std::string > _productPrice;
    cocos2d::Label* _price;
    
    time_t  _timev;
    float _timer1;
    float _timer2;
    tm * _time1;
    tm * _time2;
    
private:
    void updateIAP(const std::vector<sdkbox::Product>& products);
    
    void onInitialized(bool ok);
    
    void onSuccess(sdkbox::Product const& p) ;
    
    void onFailure(sdkbox::Product const& p, const std::string &msg);
    
    void onCanceled(sdkbox::Product const& p);
    
    void onRestored(sdkbox::Product const& p);
    
    void onProductRequestSuccess(std::vector<sdkbox::Product> const &products);
    
    void onProductRequestFailure(const std::string &msg);
    
    void onRestoreComplete(bool ok, const std::string &msg);
    
    void onIAP(cocos2d::Ref *sender);
    
    void onRequestIAP(cocos2d::Ref* sender);

    void onRestoreIAP(cocos2d::Ref* sender);
    
    
    int _coinCount;
    cocos2d::Label* _txtCoin;
    
    
    
    
    
     void onConnectionStatusChanged( int status );
     void onScoreSubmitted( const std::string& leaderboard_name, long score, bool maxScoreAllTime, bool maxScoreWeek, bool maxScoreToday);
     void onIncrementalAchievementUnlocked( const std::string& achievement_name );
     void onIncrementalAchievementStep( const std::string& achievement_name, double step );
     void onAchievementUnlocked( const std::string& achievement_name, bool newlyUnlocked );
    

    
    
    void onLogin(bool isLogin, const std::string& error);
    void onAPI(const std::string& tag, const std::string& jsonData);
    void onSharedSuccess(const std::string& message);
    void onSharedFailed(const std::string& message);
    void onSharedCancel();
    void onPermission(bool isLogin, const std::string& error);
    void onInviteFriendsResult( bool result, const std::string& msg );
    
    rapidjson::Document document;
    
    cocos2d::Sprite* Show_friends_score_popup;
    cocos2d::Sprite* Show_friends_Score_name;
    void addScrollView();
    
public:
    
    void onFetchFriends(bool ok, const std::string& msg){};
    void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ){}
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ){}
    void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ){}
      
};


#endif /* DNMainMenuLayer_hpp */
