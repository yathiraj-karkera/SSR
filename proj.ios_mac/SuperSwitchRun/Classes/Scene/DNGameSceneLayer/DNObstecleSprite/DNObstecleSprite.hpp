//
//  DNObstecleSprite.hpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#ifndef DNObstecleSprite_hpp
#define DNObstecleSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

class DNObstecleSprite: public cocos2d::Sprite
{
    DNObstecleSprite();
    ~DNObstecleSprite();
    
    public :

    static DNObstecleSprite *create(const char *FileName);
    bool isScoreSet ;
    bool isScoreDeducted ;
   
    
};



#endif /* DNObstecleSprite_hpp */
