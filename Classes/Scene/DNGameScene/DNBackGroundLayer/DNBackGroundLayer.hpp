//
//  DNBackGroundLayer.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#ifndef DNBackGroundLayer_hpp
#define DNBackGroundLayer_hpp

#include <stdio.h>
#include "cocos2d.h"

class DNBackGroundLayer : public cocos2d::Layer {
public:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    DNBackGroundLayer();
    virtual ~DNBackGroundLayer();
    static DNBackGroundLayer* createBackground();
    virtual bool init();
    
    //sprites for diff layer
    cocos2d::Sprite* m_LayerBackground;
    cocos2d::Sprite* m_LayerGround1;
    cocos2d::Sprite* m_LayerGround2;
    cocos2d::Sprite* m_LayerGrass1;
    cocos2d::Sprite* m_LayerGrass2;
    cocos2d::Sprite* m_LayerBuilding1;
    cocos2d::Sprite* m_LayerBuilding2;
    cocos2d::Sprite* m_LayerBuilding3;
    cocos2d::Sprite* m_LayerBuilding4;
    cocos2d::Sprite* m_LayerCloud1;
    cocos2d::Sprite* m_LayerCloud2;
    
    //update
    void bgUpdate(float dtSpeed);
};


#endif /* DNBackGroundLayer_hpp */
