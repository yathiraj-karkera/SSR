//
//  DNMashroomSprite.hpp
//  DesiNinja
//
//  Created by Yathiraj on 11/08/17.
//
//

#ifndef DNMashroomSprite_hpp
#define DNMashroomSprite_hpp

#include <stdio.h>
#include "cocos2d.h"

class DNMashroomSprite: public cocos2d::Sprite
{
    DNMashroomSprite();
    ~DNMashroomSprite();
    
    public :

    static DNMashroomSprite *create(const char *FileName);
    bool isScoreSet ;
    
    
};



#endif /* DNObstecleSprite_hpp */
