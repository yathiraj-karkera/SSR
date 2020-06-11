//
//  CCObjectiveC.h
//  Game       : ColorCube
//  Game Studio:JUEGOSTUDIOS

#ifndef _DNObjectiveC_h
#define _DNObjectiveC_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "cocos2d.h"
#import "AppController.h"

using namespace cocos2d;

@interface DNObjectiveC :  NSObject
{
    
    
}
+(bool)isConnetToNetwork;

+(NSString*)getDeviceId;
+(NSString*)getDeviceToken;

+(void) doiOSGoogleSign;
+(void) doiOSGoogleSignOut;

+(void) doiOSGooglePlusShare;
+(void) doiOSGooglePlusShareWithMessage:(NSString *)shareText;

+(void) doFacebookAsk;
+(void) doCheckResponse;
+(void) doShareGift:(NSString *)playerIds with:(NSString *)responseIds;
+(void) doDeleteFacebookResponse:(NSString *)responseIds;
+(void)inviteFriend;

@end
#endif
