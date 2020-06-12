//
//  DNObstecleSprite.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNObstecleSprite.hpp"
#include "cocos2d.h"


#pragma mark - constructor
DNObstecleSprite::DNObstecleSprite()
{
    
    
}

#pragma mark - Destructor
DNObstecleSprite::~DNObstecleSprite()
{
    
}

//creating a new sprite
#pragma mark - KQMatchTheFollowingSprites
DNObstecleSprite *DNObstecleSprite::create(const char* filename)
{
    DNObstecleSprite *sprite = new DNObstecleSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
    
}
