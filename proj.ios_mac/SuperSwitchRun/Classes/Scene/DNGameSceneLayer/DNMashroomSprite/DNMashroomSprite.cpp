//
//  DNMashroomSprite.cpp
//  DesiNinja
//
//  Created by Yathiraj on 11/08/17.
//
//

#include "DNMashroomSprite.hpp"
#include "cocos2d.h"


#pragma mark - constructor
DNMashroomSprite::DNMashroomSprite()
{
    
    
}

#pragma mark - Destructor
DNMashroomSprite::~DNMashroomSprite()
{
    
}

//creating a new sprite
#pragma mark - KQMatchTheFollowingSprites
DNMashroomSprite *DNMashroomSprite::create(const char* filename)
{
    DNMashroomSprite *sprite = new DNMashroomSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
    
}
