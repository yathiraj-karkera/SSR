//
//  CBDOManager.cpp
//  ChhotaBheemUpTheHill
//
//  Created by Deepthi on 5/10/17.
//
//

#include "DNDOManager.hpp"
#include "DNDataManager.h"
#include "SimpleAudioEngine.h"
#include  "DNAcheivementManager.hpp"
#include "cocos2d.h"
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
using namespace CocosDenshion;

static DNDOManager *gSharedManager = NULL;

#pragma mark - create
DNDOManager* DNDOManager::sharedManager(void) {
    
    DNDOManager *pRet = gSharedManager;
    
    if (!gSharedManager)
    {
        pRet = gSharedManager = new DNDOManager();
        
        if (! gSharedManager->init())
        {
            delete gSharedManager;
            gSharedManager = NULL;
            pRet = NULL;
        }
    }
    return pRet;
}


bool DNDOManager::init()
{
    return true;
}

static int myrandom (int i) { return std::rand()%i;}


#pragma mark - constructor,destructor
DNDOManager::DNDOManager()
{
    if(DNDataManager::sharedManager()->_isinGameScene)
    {
        DNDataManager::sharedManager()->_doManager = this;
        
    }
    _objectCompletionLayer = NULL;
   
}

DNDOManager::~DNDOManager()
{
    
}

#pragma mark - Type1
void DNDOManager::getRandomDoType1Arr()
{
    {
        //Get Detail from plist
        std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/DailyObjectiveDetails.plist");
        ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
        
        ValueVector _typeOneDetailsArr1;
        _typeOneDetailsArr1 = rootDict["Type1"].asValueVector();
        int size = _typeOneDetailsArr1.size()-1;
        int randomtaskIndex = random(0,size);
        
        ValueMap typeDetailDict = _typeOneDetailsArr1.at(randomtaskIndex).asValueMap();
        UserDefault::getInstance()->setStringForKey("alphaObjective", typeDetailDict["text"].asString());
        UserDefault::getInstance()->setIntegerForKey("DOTypeOne",randomtaskIndex);
    }
}

#pragma mark getAlphabetFromArr
std::string DNDOManager::getAlphabetFromArr()
{
    
    //retriving data from plist
    std::string pszPathOne = FileUtils::getInstance()->fullPathForFilename("Plist/DailyObjectiveDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    
    ValueVector a = rootDict["Type1"].asValueVector();
    ValueMap typeDetailDict = a.at(UserDefault::getInstance()->getIntegerForKey("DOTypeOne",0)).asValueMap();
    
    
    
    std::string text = typeDetailDict["charDetails"].asString();
    
    std::istringstream ss(text);
    std::string token;
    
    //seperating letters and storing in a vector
    this->_doType1CharArr.clear();
    while(std::getline(ss, token, ','))
    {
        this->_doType1CharArr.push_back(Value(token));
    }
    
    int stringIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType1CharArrIndexVal",0);
    std::string alphatext = this->_doType1CharArr.at(stringIndexVal).asString();
    return alphatext;
}

#pragma mark addingAlphabet
void DNDOManager::addingAlphabet()
{
       int charIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType1CharArrIndexVal",0);
    
    //taking alphabet from array
    std::string alphabet = getAlphabetFromArr();
    
    DNDataManager::sharedManager()->_gameLayerRef->_charLabel->setString(alphabet.c_str());
    
}

#pragma mark onAlphabetCollision
void DNDOManager::onAlphabetCollision(DNDOCharLabel *alphabet)
{
    //position for the collected alphabet
    Point pos = alphabet->getParent()->convertToWorldSpace(alphabet->getPosition());

    this->_charArr.eraseObject(alphabet);
    
    //removing alphabet from layer
     alphabet->setVisible(false);
    int charIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType1CharArrIndexVal",0);
    charIndexVal = charIndexVal + 1;
    
    UserDefault::getInstance()->setIntegerForKey("DOType1CharArrIndexVal",charIndexVal);
    this->addingAlphabetToTheLayer();
    
    //checking for word completion
    if(this->_doType1CharArr.size() == charIndexVal)
    {
        this->_doType1CharArr.clear();
       
        
        
        UserDefault::getInstance()->setIntegerForKey("DOType1CharArrIndexVal",0);
        
                
        for(int i=0;i<this->_doTypeArr.size();i++)
        {
            std::string str = this->_doTypeArr.at(i).asString();
            
            if(strcmp(str.c_str(),"0")==0)
            {
                this->_doTypeArr.erase(this->_doTypeArr.begin()+i);
                break;
            }
        }
        
        std::string doTypeIndexVal;
        for(int i=0;i<this->_doTypeArr.size();i++)
        {
            std::string str = this->_doTypeArr.at(i).asString();
            
            if(i==0)
            {
                doTypeIndexVal = str;
            }
            else
            {
                doTypeIndexVal = doTypeIndexVal+","+str;
            }
        }
        
        //resetting all the values
        this->_shouldDisplayAlphabet = false;
        DNDataManager::sharedManager()->_gameLayerRef->isAlphabetCreated = true;
        UserDefault::getInstance()->setStringForKey("DOTypes",doTypeIndexVal.c_str());
        UserDefault::getInstance()->setBoolForKey("DailyObjectiveAlphaComplete",true);
       // DNDataManager::sharedManager()->_HUDLayerRef->addAlphabetCompleteDialog();
        
       //keeping track of word collected
        UserDefault::getInstance()->setIntegerForKey("WordsCollected",UserDefault::getInstance()->getIntegerForKey("WordsCollected",0) + 1);
        
        //Acheivement
        int wordsCollected = UserDefault::getInstance()->getIntegerForKey("WordsCollected",0);
        
        switch (wordsCollected) {
            case 10:
                DNAcheivementManager::sharedManager()->AcheivementXp(20,"Elementary");
               
                break;
            case 100:
                DNAcheivementManager::sharedManager()->AcheivementXp(30,"Scrabbler");
             
                break;
            case 500:
                DNAcheivementManager::sharedManager()->AcheivementXp(60,"Word_King");
              
                break;
            default:
                break;
        }
        
    }
}

#pragma mark addingAlphabetToTheLayer
void DNDOManager::addingAlphabetToTheLayer()
{
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
   
    int charIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType1CharArrIndexVal",0);
    for(int i=0;i<this->_doType1CharArr.size();i++)
    {
        //masking the alpha in the word based on  letters collected
        if(charIndexVal>i)
        {
            charLabelVec.at(i)->setOpacity(255);
        }
        else
        {
            charLabelVec.at(i)->setOpacity(50);
            
        }
    }
    
    
    //move action for the popup
    MoveTo *move = MoveTo::create(0.5, Vec2(0,0));
    
    Sequence *finSeq=Sequence::create(move,DelayTime::create(1.0),CallFunc::create(CC_CALLBACK_0(DNDOManager::removeAlphabetFromLayer,this)),NULL);
    this->_alphabetLayer->runAction(finSeq);
}

#pragma mark - removeAlphabetFromLayer
void DNDOManager::removeAlphabetFromLayer()
{
    //removing alphabet layer
   // this->_alphabetLayer->setVisible(false);
    this->_alphabetLayer->setPosition(Vec2(0,-200));
    int charIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType1CharArrIndexVal",0);
    if(this->_doType1CharArr.size() == charIndexVal)
    {
        DNDataManager::sharedManager()->_HUDLayerRef->addAlphabetCompleteDialog();
    }
  
}


#pragma mark - Type2
void DNDOManager::getRandomDoType2Arr()
{
    {
        //Get Detail from plist
        std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/DailyObjectiveDetails.plist");
        ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
        
        ValueVector _typeTwoDetailsArr = rootDict["Type2"].asValueVector();
        int size = _typeTwoDetailsArr.size()-1;
        int randomtaskIndex = random(0,size);
        UserDefault::getInstance()->setIntegerForKey("DOTypeTwo",randomtaskIndex);
    }
    
}

#pragma mark - getType2FromArr
void DNDOManager::getType2FromArr()
{
    int typeTwo = UserDefault::getInstance()->getIntegerForKey("DOTypeTwo",-1);
    
    if(typeTwo == -1)
    {
        this->getRandomDoType2Arr();
    }
    
    //taking data from the plist  for the wall completion objecting
    
    int doIndexVal = UserDefault::getInstance()->getIntegerForKey("DOType2ArrIndexVal",0);
    int typeOneIndexDetails =  UserDefault::getInstance()->getIntegerForKey("DOTypeTwo",0);
    
    std::string pszPathOne = cocos2d::FileUtils::getInstance()->fullPathForFilename("Plist/DailyObjectiveDetails.plist");
    ValueMap rootDict = FileUtils::getInstance()->getValueMapFromFile(pszPathOne.c_str());
    ValueVector a = rootDict["Type2"].asValueVector();
    ValueMap typeDetailDict = a.at(typeOneIndexDetails).asValueMap();
    
    UserDefault::getInstance()->setIntegerForKey("_totalNoOfWallPassForDO", typeDetailDict["noOfWalls"].asInt());
    UserDefault::getInstance()->setIntegerForKey("_colorCode", typeDetailDict["ColorCode"].asInt());
    UserDefault::getInstance()->setStringForKey("wallObjective", typeDetailDict["text"].asString());
    
}

#pragma mark - onType2ObjectiveSuccessfull
void DNDOManager::onType2ObjectiveSuccessfull(bool shouldDisplay)
{
    //resetting all the values
    DNDataManager::sharedManager()->_totalNoOfWallPassForDO = 0;
    DNDataManager::sharedManager()->_WallPassForDO = 0;
    UserDefault::getInstance()->setIntegerForKey("_totalNoOfWallPassForDO", 0);
    UserDefault::getInstance()->setIntegerForKey("_colorCode", 0);
    
    UserDefault::getInstance()->setIntegerForKey("wallsPassedForDO",0);
    UserDefault::getInstance()->setBoolForKey("DailyObjectiveWallComplete",true);
    
    //calling objective complete popup
     DNDataManager::sharedManager()->_HUDLayerRef->addWallComplteDialog();
}



