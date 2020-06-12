//
//  DNSplashScreen.hpp
//  DesiNinja
//
//  Created by Yathiraj on 28/08/17.
//
//

#ifndef DNSplashScreen_hpp
#define DNSplashScreen_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"


class DNSplashScreen : public cocos2d::Scene
{
public:
    
    DNSplashScreen();
    virtual ~DNSplashScreen();
    
    static cocos2d::Scene* createScene();
     virtual bool init();
    void replaceSceneAfterDelay();
    void initializingUi();
    
    int _reviveCount;
    int _deathCount;
    int _wallCount;
    int _powerUpCount;
    
    
    
    CREATE_FUNC(DNSplashScreen);

};
#endif /* DNSplashScreen_hpp */
