//
//  DNConstants.h
//  DesiNinja
//
//  Created by Yathiraj on 03/08/17.
//
//

#ifndef DNConstants_h
#define DNConstants_h

//Game Speed
#define GAMEMEDIUMSPEED 10.5
#define GAMEEASYSPEED 10
#define GAMEHARDSPEED 11
#define GAME_DT_VALUE 0.1
#define GAME_DT_DIVIDER 60


#define BACKGROUNDSPEED 2
#define FADE_LAYER_COLOUR 0
#define FADE_LAYER_OPACITY 100
#define BUILDINGLAYER1SPEED 2
#define BUILDINGLAYER2SPEED 1
#define CLOUDSPEED  0.25
#define GROUNDSPEED 6

#define POSX(a) a->getPositionX()
#define POSY(a) a->getPositionY()
#define WIDTH(a) a->getContentSize().width
#define HEIGHT(a) a->getContentSize().height

#endif /* DNConstants_h */
