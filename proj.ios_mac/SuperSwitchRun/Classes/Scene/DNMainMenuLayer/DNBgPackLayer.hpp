//
//  DNBgPackLayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 09/08/17.
//
//

#ifndef DNBgPackLayer_hpp
#define DNBgPackLayer_hpp

#include "ui/UIPageView.h"
#include "cocos2d.h"

#include <stdio.h>
class DNBgPackLayer : public cocos2d::Layer {
public:
    DNBgPackLayer();
    virtual ~DNBgPackLayer();
    static DNBgPackLayer* createBGPackLayer();
    virtual bool init();
    cocos2d::ui::PageView* pageview;
    
    cocos2d::ValueVector _typeOneDetailsArr;
    cocos2d::Vector<cocos2d::Sprite*> _layoutVector;
    
    void pageViewEvent(cocos2d::Ref* sender,cocos2d::ui::PageView::EventType type);
    void setBg();
    void AddLockScreen();
    void checkBg(int index);
    
    
};



#endif /* DNBgPackLayer_hpp */
