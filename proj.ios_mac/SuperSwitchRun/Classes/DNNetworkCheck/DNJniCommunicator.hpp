//
//  DNJniCommunicator.hpp
//  ColorCube
//
//
//

#ifndef DNJniCommunicator_hpp
#define DNJniCommunicator_hpp

#include <stdio.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif

#include "cocos2d.h"
using namespace cocos2d;

class DNJniCommunicator {
    
public:
    DNJniCommunicator();
    virtual ~DNJniCommunicator();
    
    bool init(void);
    static DNJniCommunicator* sharedManager(void);
    
    //DEVICEiD
    void getDeviceId();
    
    bool isConnetToNetworkAnd();
    
    //MARK: ACHIEVEMENTS
    void showAchievement();
    void showAndroidLeaderBoard();
    void unlockAchievement(int number);
    void UpdateLeaderBoardScore(int score);
    
    //MARK: GOOGLE
    void doGoogleSignin();
    void doGoogleSignOut();
    void doGooglePlusShare();
    void doGooglePlusShareWithMessage(const char * str);
    
    //MARK: Local Notification
    void sendLocalNotification(const char *str);
    
    void checkCall();
    
    //facebook gamerequest
    void checkFaceBookLifeRequest();
    void sendFaceBookLifeRequest();
    void giftRequestALife(const char *userId,const char *responseID);
    void removeFFaceBookRequestResponse(const char *responseID);
    
    //Local Notification
    void fireLocalNotification(float secs,int day, const char* msg,const char* title,int uniqueNotificationID);
    void cancelLocalNotification(int uniqueNotificationID);

};

#endif /* DNJniCommunicator_hpp */
