//
//  DNTutoriallayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 14/08/17.
//
//

#ifndef DNTutoriallayer_hpp
#define DNTutoriallayer_hpp

#include <stdio.h>
#include "cocos2d.h"


class DNTutoriallayer : public cocos2d::Layer {
public:
    DNTutoriallayer();
    virtual ~DNTutoriallayer();
    static DNTutoriallayer* createTutorialLayer();
    virtual bool init();
    cocos2d::Layer *_fadeLayer;
    void fadeInBackground();
    void fadeOutbackground();
    void removeFadeLayerFromParent();
    
    void tutorialForCaharacterColor();
    void tutorialForControl();
    void tutorialForMashroom();
    void tutorialForGreenButtonIntrduce();

    cocos2d::Sprite* tutorialArrow;
    cocos2d::Sprite* tutorialBox;
    cocos2d::Label *_tutorialLabel;

};

#endif /* DNTutoriallayer_hpp */
