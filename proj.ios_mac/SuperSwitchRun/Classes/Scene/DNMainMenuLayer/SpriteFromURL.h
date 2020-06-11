//
//  SpriteFromUrl.h
//  SpriteFromUrl
//
//  Created by Guru on 11/27/14 Version 1.0.
//
//

#ifndef __SpriteFromUrl__SpriteFromUrl__
#define __SpriteFromUrl__SpriteFromUrl__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

#include "network/HttpClient.h"
using namespace cocos2d::network;


//
// Shader Effect. Effect class to create or change the effect color of the image using shaders
//
class SpriteFromUrl;

class Effect : public Ref
{
public:
    GLProgramState* getGLProgramState() const { return _glprogramstate; }
    virtual void setTarget(SpriteFromUrl *sprite){}
    
protected:
    bool initGLProgramState(const std::string &fragmentFilename);
    Effect();
    virtual ~Effect();
    GLProgramState *_glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string _fragSource;
    EventListenerCustom* _backgroundListener;
#endif
};

/*
 
 SpriteFromUrl class is derived from Sprite base class. 
 This class will used to create 2d image i.e sprite from Url link.
 By using Url link it will initialte http request and creates image from the response data and save it in device.
 It supports creating circle image.
 It supports creating black & white image.
 
*/

class SpriteFromUrl:public Sprite
{
    const char * mUrl;
    
public:
    //Placeholder image name
    std::string name;
    
    //Bool value to indicate obtain round pic
    bool isRoundProfilePicNeeded;
    bool isBlackandWhiteImageRequired;
    
    //Init Sprite with round image and without round image
    virtual bool init(const char * url,const char * samplepath,bool isBlackWhiteImageRequired);
    virtual bool init(const char * url,const char * samplepath ,bool isRoundImageNeeded,bool isBlackWhiteImageRequired);
    
    //Create Sprite
    static SpriteFromUrl * create(const char * url,const char * samplepath,bool isBlackWhiteImageRequired);
    static SpriteFromUrl * create(const char * url,const char * samplepath ,bool isRoundImageNeeded,bool isBlackWhiteImageRequired);
    static SpriteFromUrl* createWithTexture(Texture2D *texture);
    
    //On Enter
    void onEnter();
    void onExit();
    cocos2d::__Dictionary * imageRequests;
    //Http connect
    void requestForImage(const char* url,SpriteFromUrl * sprite);
    void onHttpRequestCompleted(HttpClient* pSender,HttpResponse* pResponse);
    bool setTextureFromFilePath(SpriteFromUrl * sprite,std::string path);
    std::string nameFromUrl(std::string url);
    
    //Set url
    void setURL(const char * url);
    
    //Resize sample image to url image size
    void changeDimentionToUrlSprite(Texture2D* urlSpr);
    
    //Circle image mask
    void createCircleImage(Sprite* newSpr);
    SpriteFromUrl* maskedSpriteWithSprite(Sprite* pTextureSprite, Sprite* pMaskSprite,Vec2 profilePicPos);
    
    //Loading animatiomn
    Sprite* loadingSp;
    void addLoadingAnimation();
  
};

#endif /* defined(__SpriteFromUrl__SpriteFromUrl__) */
