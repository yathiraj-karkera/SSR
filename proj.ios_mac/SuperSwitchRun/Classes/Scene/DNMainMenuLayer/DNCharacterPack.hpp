//
//  DNCharacterPack.hpp
//  DesiNinja
//
//  Created by Yathiraj on 23/08/17.
//
//

#ifndef DNCharacterPack_hpp
#define DNCharacterPack_hpp

#include "ui/UIPageView.h"
#include "cocos2d.h"
#include "PluginIAP/PluginIAP.h"

#include <stdio.h>
class DNCharacterPack : public cocos2d::Layer{
public:
    DNCharacterPack();
    virtual ~DNCharacterPack();
    static DNCharacterPack* createCharacterPackLayer();
    cocos2d::Sprite* _background;
     cocos2d::ValueVector _typeOneDetailsArr;
    cocos2d::ui::PageView* _pageview;
    cocos2d::Label *_characterName;
    cocos2d::Label *_unlockLabel;
    
    virtual bool init();
    void pageViewEvent(cocos2d::Ref* sender,cocos2d::ui::PageView::EventType type);
    cocos2d::Vector<cocos2d::Sprite*> _layoutVector;
    cocos2d::MenuItemImage* _purchaseButton;
    cocos2d::Sprite *_lock;
    cocos2d::MenuItemSprite* _leftArrow;
    cocos2d::MenuItemSprite* _rightArrow;
    
    void showCharacterPackMenu();
    void hideCharacterPackMenu();
    void creatingCharacterAnimation(int index);
    void arrowsClicked(Ref* sender);
    void purchasecharacter();
    void purchaseCharacterIap();
    
    cocos2d::Label* _price;
    
    void onRequestIAP(cocos2d::Ref* sender);
    void onRestoreIAP(cocos2d::Ref* sender);
    void onIAP(cocos2d::Ref* sender);
    
  
    
    
    
    int _coinCount;
    cocos2d::Label* _txtCoin;
    
    cocos2d::Menu * _iapMenu;
    std::vector<sdkbox::Product> _products;
    
};



#endif /* DNCharacterPack_hpp */
