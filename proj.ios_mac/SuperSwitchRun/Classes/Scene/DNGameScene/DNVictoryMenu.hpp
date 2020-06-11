//
//  DNVictoryMenu.hpp
//  DesiNinja
//
//  Created by Yathiraj on 21/08/17.
//
//

#ifndef DNVictoryMenu_hpp
#define DNVictoryMenu_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PluginFacebook/PluginFacebook.h"
class DNVictoryMenu : public cocos2d::Layer,public sdkbox::FacebookListener {
public:
    DNVictoryMenu();
    virtual ~DNVictoryMenu();
    static DNVictoryMenu* createVictoryMenu();
    virtual bool init();
   
    void facebookShareScoreOrScreenShot();
    void facebookFriendPostScore();
    void afterCaptureScreen(bool yes, const std::string &outputFilename);
    void facebookDialog();
    std::string _captureFilename;
    std::string _captureFilename1;
    void twittershareScreenshot();
    void screenshotForTwitter();
    int myScoreForSharing;
    void send_score(cocos2d::Ref *sender);
    
    void afterCaptureScreen1(bool yes, const std::string &outputFilename1);
    
    cocos2d::Sprite* _background;
    
    void showVictoryMenu();
    void hideVictoryMenu();
    void menuButtonTapped();
    void replayButtonTapped();
    
    //Facebook callback
    void onLogin(bool isLogin, const std::string& msg) override;
    void onSharedSuccess(const std::string& message) override;
    void onSharedFailed(const std::string& message) override;
    void onSharedCancel() override;
    void onAPI(const std::string& key, const std::string& jsonData) override;
    void onPermission(bool isLogin, const std::string& msg) override;
    void onFetchFriends(bool ok, const std::string& msg) override;
    void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ) override;
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) override;
    void onInviteFriendsResult( bool result, const std::string& msg ) override;
    void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ) override;
    void onExitButtonPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    CREATE_FUNC(DNVictoryMenu);
    };


#endif /* DNVictoryMenu_hpp */
