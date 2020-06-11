//
//  CCObjectiveCCalls.h
//  Game       : ColorCube
//  Game Studio:JUEGOSTUDIOS

#ifndef _DNObjectiveCCalls_h
#define _DNObjectiveCCalls_h
#include "cocos2d.h"
#include <stddef.h>

//USAGE CALL FROM C++ to LMObjectiveCCalls TO ---> LMObjectiveC
//BASICALLY FROM C++ TO OBJECTIVE C
//This class is useful to call objective c specfic functions !!!

using namespace cocos2d;

class DNObjectiveCCalls
{
public:
    static bool isConnetToNetwork();
    static std::string getDeviceID();
    static std::string getDeviceToken();
    static void doiOSGoogleSign();
    static void doiOSGoogleSignOut();
    static void doiOSGooglePlusShare();
    static void doiOSGooglePlusShareWithText(std::string shareMessage);
    
    
    static void doFacebookAsk();
    static void checkResponse();
    static void sendGift(std::string playerId,std::string responseId);
    static void deleteFacebookResponse(std::string requestId);
    static void inviteFriend();
};



#endif
