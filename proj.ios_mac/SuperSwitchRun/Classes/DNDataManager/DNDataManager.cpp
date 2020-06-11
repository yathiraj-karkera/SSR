//
//  DNDataManager.cpp
//  DesiNinja
//
//  Created by Yathiraj on 02/08/17.
//
//

#include "DNDataManager.h"
//#include "PluginFirebase/PluginFirebase.h"
#include "DNGameUtility.hpp"
#include "cocos2d.h"


static DNDataManager *gSharedManager = NULL;


DNDataManager::DNDataManager()
{
  
}
DNDataManager::~DNDataManager()
{
}

DNDataManager* DNDataManager::sharedManager(void) {
  
  DNDataManager *pRet = gSharedManager;
  
  if (! gSharedManager)
  {
    pRet = gSharedManager = new DNDataManager();
    
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
bool DNDataManager::init(void)
{
    
  return true;
}

#pragma mark - getTutorialPlayed
bool DNDataManager::getTutorialPlayed()
{
    return this->_isTutorialPlayed;
}

#pragma mark - setTutorialPlayed
void DNDataManager::setTutorialPlayed(bool isTutorialPlayed)
{
    this->_isTutorialPlayed = isTutorialPlayed;
    UserDefault::getInstance()->setBoolForKey("TutorialPlayed", isTutorialPlayed);
}

#pragma mark - getMashroomTutorialPlayed
bool DNDataManager::getMashroomTutorialPlayed()
{
    return this->_isMashroomTutorialPlayed;
}

#pragma mark - setMashroomTutorialPlayed
void DNDataManager::setMashroomTutorialPlayed(bool isMashroomPlayed)
{
    this->_isMashroomTutorialPlayed = isMashroomPlayed;
    UserDefault::getInstance()->setBoolForKey("MashroomTutorial", isMashroomPlayed);
}


#pragma mark Sound
void DNDataManager::play2dSound(std::string soundName)
{
    float music = UserDefault::getInstance()->getFloatForKey("effect", 0.5);
    cocos2d::experimental::AudioEngine::play2d(soundName,false,music);

}

#pragma mark callAnalytics
void DNDataManager::callAnalytics(std::string parameter,std::string action, std::string logText,int kFIRParameterValue)
{
    //calling analytics
//    if(DNGameUtility::checkIsNetworkAvailable())
//    {
//        std::map<std::string, std::string> params;
//        params["Parameter"] = parameter;
//        params["Action"] = action;
//        params[sdkbox::Firebase::Analytics::kFIRParameterValue] = kFIRParameterValue;
//        sdkbox::Firebase::Analytics::logEvent(logText, params);
 //   }
}
