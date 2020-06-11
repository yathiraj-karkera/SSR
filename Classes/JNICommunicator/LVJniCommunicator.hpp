//
//  LVJniCommunicator.hpp
//  LifavatorVideo
//
//  Created by deepthi on 3/3/17.
//
//

#ifndef LVJniCommunicator_hpp
#define LVJniCommunicator_hpp

#include <stdio.h>

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif
USING_NS_CC;
class LVJniCommunicator {
    
public:
    LVJniCommunicator();
    virtual ~LVJniCommunicator();
    
    bool init(void);
    static LVJniCommunicator* sharedManager(void);
    
    void getTheVideoDownloaded(const char *url,const char *fileNam);
    void onPlayButtonClick(const char* path);
    void onClickOfFeedback();
    void fireLocalNotification(float secs,int day, const char* msg,const char* title,int uniqueNotificationID);
    
    void downloadVideoWithURLAndFileName(const char *url,const char *fileNam);
    void playVideoFromResFolder();
    
    bool isAskedForPermission(int val);
    
    bool checkForFileExist(const char *fileNam);
    
    void GooglSignin();
    
    void PurchasedIap();
    
    //Network check
    bool isConnetToNetwork();

};

#endif /* LVJniCommunicator_hpp */
