
//  DNCharacterSprite.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.



#ifndef DNCharacterSprite_hpp
#define DNCharacterSprite_hpp

#include <stdio.h>
#include "cocos2d.h"

class DNCharacterSprite: public cocos2d::Sprite
{
    DNCharacterSprite();
    ~DNCharacterSprite();
    
    public :
    void characterAnim();
    
    
    static DNCharacterSprite *create(const char *FileName);
    static DNCharacterSprite* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
    /**A SpriteFrame will be fetched from the SpriteFrameCache by spriteFrameName param. If the SpriteFrame doesn't exist it will raise an exception. **/
    static DNCharacterSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    //Members
    void addLableToSprite(std::string stringToBeAdded);
    
    
    
    
};
#endif /* DNCharacterSprite_hpp */
