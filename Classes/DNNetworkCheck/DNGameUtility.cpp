
//  FQGameUtility.hpp
//  Game       : FreakQuiz
//  Created by : Dhanashree Kamath K on 08/17//16.
//  Game Studio:JUEGOSTUDIOS


#include "DNGameUtility.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "LVJniCommunicator.hpp"
#else
#include "DNObjectiveCCalls.h"
#endif
#include "cocos2d.h"
using namespace cocos2d::network;

#pragma mark - Constructor
DNGameUtility::DNGameUtility(void)
{

}

#pragma mark - Destructor
DNGameUtility::~DNGameUtility(void)
{

}

#pragma mark - Response
void  DNGameUtility::getResponseBuffer(void *data , rapidjson::Document &  document)
{
    cocos2d::network::HttpResponse *response = (cocos2d::network::HttpResponse*)data;
    
    
    if (!response)
    {
        return ;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        
    }
    
    int statusCode = response->getResponseCode();
    char statusString[256] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    //m_labelStatusCode->setString(statusString);
   
    
    
    
    if (!response->isSucceed())
    {
        
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    
    char * charbuffer =new char[buffer->size()+1];
    
    
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        charbuffer[i]=(*buffer)[i];
    }
    
    charbuffer[buffer->size()]=0;
    
   
    
    
    document.Parse<0>(charbuffer);
    delete[] charbuffer;
}

bool DNGameUtility::checkIsNetworkAvailable()
{
    bool isNetworkAvailable=true;
   
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   isNetworkAvailable =  LVJniCommunicator::sharedManager()->isConnetToNetwork();
    
#else
    isNetworkAvailable = DNObjectiveCCalls::isConnetToNetwork();
#endif
  
    return isNetworkAvailable;
}

