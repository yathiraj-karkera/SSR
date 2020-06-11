//
//  CBDOManager.hpp
//  ChhotaBheemUpTheHill
//
//  Created by Deepthi on 5/10/17.
//
//

#ifndef DNDOManager_hpp
#define DNDOManager_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DNDOCharLabel.hpp"

USING_NS_CC;


class DNDOManager : public Ref
{
private:
     DNDOManager();
    ~DNDOManager();
    
    virtual bool init();
    
public:
    static DNDOManager* sharedManager(void);
    
    Vector<Label*> _charArr;
    ValueVector _doTypeArr;
    ValueVector _doType1CharArr;
    ValueVector _typeOneDetailsArr;
    ValueVector _typeThreeDetailsArr;
    ValueVector _typeFiveDetailsArr;
    

    //Type1
    Layer *_alphabetLayer;
    Layer *_objectCompletionLayer = NULL;
    Vector<Label*> charLabelVec;
    
    bool _shouldDisplayAlphabet = false;
    void getRandomDoType1Arr();
    std::string getAlphabetFromArr();
    void addingAlphabet();
    void onAlphabetCollision(DNDOCharLabel* alphabet);
    void addingAlphabetToTheLayer();
    void removeAlphabetFromLayer();
    void onType1Successfull(bool shouldDisplay=true);
    
//    //Type2
    bool _shouldCheckForDoWallPass = true;
    int  _totalNoOfWallPassForDO = 0;
    int _colorCode ;
    void getRandomDoType2Arr();
    void getType2FromArr();
    void onType2ObjectiveSuccessfull(bool shouldDisplay=true);
};

#endif /* DNDOManager_hpp */
