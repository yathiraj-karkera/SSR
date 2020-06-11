//
//  DNAcheivementManager.hpp
//  MyGame3
//
//  Created by Yathiraj on 10/09/17.
//
//

#ifndef DNAcheivementManager_hpp
#define DNAcheivementManager_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
class DNAcheivementManager: public Ref
{
    
private:
    
    DNAcheivementManager();
    ~DNAcheivementManager();
    
public:
    bool init(void);
    
    static DNAcheivementManager* sharedManager(void);
    
    //BG unlocks
    bool isBGUnlocked(int index);
    void unloackBg(int index);
    void unlockAllBg();
    
    //Character unlocks
    void unloackCharacter(int index);
    void unlockAllCharacter();
    bool isCharacterUnlocked(int index);
    
    //Acheivement unlock
    void AcheivementXp(int xp,std::string acheivement);
    

   
};

#endif /* DNAcheivementManager_hpp */
