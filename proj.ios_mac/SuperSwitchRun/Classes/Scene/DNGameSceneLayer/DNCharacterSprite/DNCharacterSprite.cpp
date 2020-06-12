//
//  DNCharacterSprite.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNCharacterSprite.hpp"
#include "cocos2d.h"


#pragma mark - constructor
DNCharacterSprite::DNCharacterSprite()
{
    
    
}

#pragma mark - Destructor
DNCharacterSprite::~DNCharacterSprite()
{
    
}

//creating a new sprite
#pragma mark - KQMatchTheFollowingSprites
DNCharacterSprite *DNCharacterSprite::create(const char* filename)
{
    DNCharacterSprite *sprite = new DNCharacterSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
    
}

#pragma mark - createWithSpriteFrame
DNCharacterSprite* DNCharacterSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    //spriteFrame	A sprite frame which involves a texture and a rect.
    
    DNCharacterSprite *sprite = new (std::nothrow) DNCharacterSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

//Create the sprite with frame name
#pragma mark - createWithSpriteFrameName
DNCharacterSprite* DNCharacterSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    cocos2d::SpriteFrame *frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    return createWithSpriteFrame(frame);
}

#pragma mark - Method to add label to custom sprite
void DNCharacterSprite::addLableToSprite(std::string stringToBeAdded)
{
    auto customSpriteName = cocos2d::Label::createWithTTF(stringToBeAdded.c_str(), "fonts/arial.ttf", 28);
    customSpriteName->setPosition(cocos2d::Vec2(this->getContentSize().width/2,-this->getContentSize().width/4));
    this->addChild(customSpriteName, 1);
    customSpriteName->setColor(cocos2d::Color3B::BLUE);
    
}
