//
//  DNAcheivementManager.cpp
//  MyGame3
//
//  Created by Yathiraj on 10/09/17.
//
//

#include "DNAcheivementManager.hpp"
#include "DNGameUtility.hpp"
#include "DNDataManager.h"
#include "cocos2d.h"


static DNAcheivementManager *gSharedManager = NULL;


DNAcheivementManager::DNAcheivementManager()
{
    
}
DNAcheivementManager::~DNAcheivementManager()
{
}

DNAcheivementManager* DNAcheivementManager::sharedManager(void) {
    
    DNAcheivementManager *pRet = gSharedManager;
    
    if (! gSharedManager)
    {
        pRet = gSharedManager = new DNAcheivementManager();
        
        if (! gSharedManager->init())
        {
            delete gSharedManager;
            gSharedManager = NULL;
            pRet = NULL;
        }
    }
    return pRet;
}

#pragma mark - Init
bool DNAcheivementManager::init(void)
{
    
    return true;
}

#pragma mark - unlockBg
//unlock bg one by one
void DNAcheivementManager::unloackBg(int index)
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/BackgroundDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Background"].asValueVector();
    ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    auto bgNameForLockedOrUnlocked = typeDetailDict["BgName"].asString();
    UserDefault::getInstance()->setBoolForKey(bgNameForLockedOrUnlocked.c_str(), true);
    
    
    
}

#pragma mark - unlockCharacter
//unlock character one by one
void DNAcheivementManager::unloackCharacter(int index)
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/ChacterDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Character"].asValueVector();
    ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    auto characterNameForLockedOrUnlocked = typeDetailDict["Name"].asString();
    UserDefault::getInstance()->setBoolForKey(characterNameForLockedOrUnlocked.c_str(), true);
    
}
#pragma mark - unlockAllBg
//unlock bg all at once
void DNAcheivementManager::unlockAllBg()
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/BackgroundDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Background"].asValueVector();
    for (int index = 0;index<_typeOneDetailsArr.size();index++ )
    {
        ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
        auto bgNameForLockedOrUnlocked = typeDetailDict["BgName"].asString();
        UserDefault::getInstance()->setBoolForKey(bgNameForLockedOrUnlocked.c_str(), true);
        
    }
    UserDefault::getInstance()->setBoolForKey("isBGPackPurchased", true);
}

#pragma mark - unlockAllCharacter
//unlock all character at once
void DNAcheivementManager::unlockAllCharacter()
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/ChacterDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Character"].asValueVector();
    for (int index = 0;index<_typeOneDetailsArr.size();index++ )
    {
        ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
        auto characterNameForLockedOrUnlocked = typeDetailDict["Name"].asString();
        UserDefault::getInstance()->setBoolForKey(characterNameForLockedOrUnlocked.c_str(), true);
        
    }
    UserDefault::getInstance()->setBoolForKey("isCharacterPackPurchased", true);
}

#pragma mark - AcheivementXp
//unlock store acheivemnts and award local xp
void DNAcheivementManager::AcheivementXp(int xp,std::string acheivement)
{
    UserDefault::getInstance()->setIntegerForKey("XP",UserDefault::getInstance()->getIntegerForKey("XP",0) + xp);
    UserDefault::getInstance()->setIntegerForKey("AchievmentCount",UserDefault::getInstance()->getIntegerForKey("AchievmentCount",0)+1);
    if(DNGameUtility::checkIsNetworkAvailable())
    {
        sdkbox::PluginSdkboxPlay::unlockAchievement(acheivement);
    }
}

#pragma mark - isCharacterUnlocked
//check is perticular charcter unlocked
bool DNAcheivementManager::isCharacterUnlocked(int index)
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/ChacterDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Character"].asValueVector();
    ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    auto characterNameForLockedOrUnlocked = typeDetailDict["Name"].asString();
    return UserDefault::getInstance()->getBoolForKey(characterNameForLockedOrUnlocked.c_str(), false);
}

#pragma mark - unlockBg
//check is perticular bg unlocked
bool DNAcheivementManager::isBGUnlocked(int index)
{
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/BackgroundDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    cocos2d::ValueVector _typeOneDetailsArr;
    _typeOneDetailsArr = rootDict["Background"].asValueVector();
    ValueMap typeDetailDict = _typeOneDetailsArr.at(index).asValueMap();
    auto bgNameForLockedOrUnlocked = typeDetailDict["BgName"].asString();
    return UserDefault::getInstance()->getBoolForKey(bgNameForLockedOrUnlocked.c_str(), false);
}
