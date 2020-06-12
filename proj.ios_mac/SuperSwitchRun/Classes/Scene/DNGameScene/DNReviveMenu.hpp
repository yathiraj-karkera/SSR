//
//  DNReviveMenu.hpp
//  DesiNinja
//
//  Created by Yathiraj on 08/08/17.
//
//

#ifndef DNReviveMenu_hpp
#define DNReviveMenu_hpp
#include "cocos2d.h"
#include <stdio.h>
#include "PluginAdMob/PluginAdMob.h"
#include "ui/CocosGUI.h"

class DNReviveMenu : public cocos2d::Layer, public sdkbox::AdMobListener {
public:
    DNReviveMenu();
    virtual ~DNReviveMenu();
    static DNReviveMenu* createReviveMenu();
    virtual bool init();

    cocos2d::Sprite* _background;
    cocos2d::ui::LoadingBar* _adTimeOutBar;
    void showReviveMenu();
    void hideReviveMenu();
    void removeReviveMenu();
    
    cocos2d::Layer *_fadeLayer;
    void fadeInBackground();
    void fadeOutbackground();
    void removeFadeLayerFromParent();
    void loadingBarProgress(float f);
    void watchAdSuccess();
    void showAds();

    
private:
    void adViewDidReceiveAd(const std::string &name) {}
    void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {}
    void adViewWillPresentScreen(const std::string &name) {}
    void adViewDidDismissScreen(const std::string &name) {}
    void adViewWillDismissScreen(const std::string &name) {}
    void adViewWillLeaveApplication(const std::string &name) {}

};

#endif /* DNReviveMenu_hpp */



