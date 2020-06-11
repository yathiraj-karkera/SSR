//
//  CCObjectiveC.m
//  Game       : ColorCube
//  Created by : Shunmugam
//  Game Studio:JUEGOSTUDIOS
//

#import <Foundation/Foundation.h>
#import "DNObjectiveC.h"
#include "AppController.h"
#include "DNReachability.h"


//#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)
//
//#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)

using namespace cocos2d;


@implementation DNObjectiveC

#pragma mark - Network Check
+(bool)isConnetToNetwork{
    
    DNReachability  *reachability = [DNReachability reachabilityForInternetConnection];
    NetworkStatus remoteHostStatus = [reachability currentReachabilityStatus];
    if(remoteHostStatus == NotReachable) {
        return false;
    }
    return true;
}

+(NSString*)getDeviceId
{
    UIDevice *device = [UIDevice currentDevice];
    
    NSString  *currentDeviceId = [[device identifierForVendor]UUIDString];
    return currentDeviceId;
}

+(NSString*)getDeviceToken
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    
    NSString *currentDeviceId=[controller getDeviceToken];
    NSLog(@"Device tocken is%@",currentDeviceId);
    return currentDeviceId;
}

+(void) doiOSGoogleSign
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller googleSignIn];
}

+(void) doiOSGoogleSignOut
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller googleSignOut];
}

+(void) doiOSGooglePlusShare
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller googlePlusShare];
}

+(void) doiOSGooglePlusShareWithMessage:(NSString *)shareText
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller googleplusShareWithMessage:shareText];
    
}

+(void) doFacebookAsk
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller facebookAppRequest];
}
+(void) doCheckResponse
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller facebookAppResponse];
}
+(void) doShareGift:(NSString *)playerIds with:(NSString *)responseIds
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller giveGift:playerIds with:responseIds];
}
+(void) doDeleteFacebookResponse:(NSString *)responseIds
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller removeFacebookResponse:responseIds];
}
+(void)inviteFriend
{
    AppController *controller = (AppController *) [UIApplication sharedApplication].delegate;
    [controller inviteFriend];

}
#pragma mark - Dealloc

- (void)dealloc
{
    [super dealloc];
}
@end

