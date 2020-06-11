//
//  CCObjectiveCCalls.m
//  Game       : ColorCube
//  Created by : Shunmugam.
//  Game Studio:JUEGOSTUDIOS

#import <Foundation/Foundation.h>

#include "DNObjectiveCCalls.h"


#include "DNObjectiveC.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AppController.h"

#pragma mark - Network Check
bool DNObjectiveCCalls::isConnetToNetwork()
{
    return [DNObjectiveC isConnetToNetwork];
}

std::string DNObjectiveCCalls::getDeviceID()
{
    NSString *devicedId= [DNObjectiveC getDeviceId];
    std::string *dId = new std::string([devicedId UTF8String]);
    return *dId;
}

std::string DNObjectiveCCalls::getDeviceToken()
{
    NSString *devicedToken= [DNObjectiveC getDeviceToken];
    
    std::string *dId = new std::string([devicedToken UTF8String]);
    return *dId;
}

void DNObjectiveCCalls::doiOSGoogleSign()
{
    [DNObjectiveC doiOSGoogleSign];
}

void DNObjectiveCCalls::doiOSGoogleSignOut()
{
    [DNObjectiveC doiOSGoogleSignOut];
}

void DNObjectiveCCalls::doiOSGooglePlusShare()
{
    [DNObjectiveC doiOSGooglePlusShare];
}

void DNObjectiveCCalls::doiOSGooglePlusShareWithText(std::string shareMessage)
{
    NSString* result = [NSString stringWithUTF8String:shareMessage.c_str()];
    [DNObjectiveC doiOSGooglePlusShareWithMessage:result];
    
    
}
void DNObjectiveCCalls::doFacebookAsk()
{
    [DNObjectiveC doFacebookAsk];
}
void DNObjectiveCCalls::checkResponse()
{
    [DNObjectiveC doCheckResponse];
}
void DNObjectiveCCalls::sendGift(std::string playerId,std::string responseId)
{
    NSString* playerIds = [NSString stringWithUTF8String:playerId.c_str()];
    NSString* responseIds = [NSString stringWithUTF8String:responseId.c_str()];
    [DNObjectiveC doShareGift:playerIds with:responseIds];
}
void DNObjectiveCCalls::deleteFacebookResponse(std::string requestId)
{
    NSString* responseIds = [NSString stringWithUTF8String:requestId.c_str()];
    [DNObjectiveC doDeleteFacebookResponse:responseIds];
}
void DNObjectiveCCalls::inviteFriend()
{
    [DNObjectiveC inviteFriend];
}
