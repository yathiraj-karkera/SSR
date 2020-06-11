//
//  LVJniCommunicator.cpp
//  LifavatorVideo
//
//  Created by deepthi on 3/3/17.
//
//

#include "LVJniCommunicator.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif
#define  LOG_TAG    "libSimpleAudioEngine"
//#define  CCLog(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
static LVJniCommunicator *gSharedManager = NULL;


LVJniCommunicator::LVJniCommunicator() {
    // TODO Auto-generated constructor stub
    
}

LVJniCommunicator::~LVJniCommunicator() {
    // TODO Auto-generated destructor stub
}

LVJniCommunicator* LVJniCommunicator::sharedManager() {
    
    LVJniCommunicator *pRet = gSharedManager;
    
    if (!gSharedManager) {
        
        pRet = gSharedManager = new LVJniCommunicator();
        
        if (!gSharedManager->init()) {
            
            delete gSharedManager;
            gSharedManager = NULL;
            pRet = NULL;
        }
    }
    return pRet;
}

bool LVJniCommunicator::init(void) {
    
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// get env and cache it
static JNIEnv* getJNIEnv(void)
{
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    if (NULL == jvm) {
        return NULL;
    }
    
    JNIEnv *env = NULL;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    switch (ret) {
        case JNI_OK :
            // Success!
            return env;
            
        case JNI_EDETACHED :
            // Thread not attached
            
            // TODO : If calling AttachCurrentThread() on a native thread
            // must call DetachCurrentThread() in future.
            // see: http://developer.android.com/guide/practices/design/jni.html
            
            if (jvm->AttachCurrentThread(&env, NULL) < 0)
            {
                return NULL;
            } else {
                // Success : Attached and obtained JNIEnv!
                return env;
            }
            
        case JNI_EVERSION :
            // Cannot recover from this error
        default :
            return NULL;
    }
}
#endif


bool LVJniCommunicator::checkForFileExist(const char *fileNam)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    jmethodID methodID = 0;
//    JNIEnv *pEnv = 0;
//    pEnv = getJNIEnv();
//    
//    CCLOG(" .......fileNam.....=%s",fileNam);
//
//    
//    jstring jproductid = pEnv->NewStringUTF(fileNam);
//
//    
//    jclass ret = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
//    methodID = pEnv->GetStaticMethodID(ret, "isFileExists", "(Ljava/lang/String;)Z");
//    
//    if (! methodID)
//    {
//        // CCLog("Failed to find static method id of %s", "sendInAppRequest");
//        return 0;
//    }
//    jboolean isConnect;
//    
//    isConnect=  pEnv->CallStaticBooleanMethod(ret,methodID,jproductid);
//    pEnv->DeleteLocalRef(ret);
//    return isConnect;
#endif
}

#pragma mark - NetworkCheck
bool LVJniCommunicator::isConnetToNetwork()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    
    jclass ret = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    methodID = pEnv->GetStaticMethodID(ret, "checkIsNetworkAvailable", "()Z");
    
    if (! methodID)
    {
       
        return false;
    }
    jboolean isConnect;
    
    isConnect=  pEnv->CallStaticBooleanMethod(ret,methodID);
    pEnv->DeleteLocalRef(ret);
    return isConnect;
#endif
    return true;
}




#pragma mark - GoogleSIgnin
void LVJniCommunicator::GooglSignin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "signin", "()V");
    if (! methodID)
    {
        return;
    }
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
    
#endif
}




//IAPPurchased()

#pragma mark - GoogleSIgnin
void LVJniCommunicator::PurchasedIap()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "IAPPurchased", "()V");
    if (! methodID)
    {
        return;
    }
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
    
#endif
}








#pragma mark - Java to CPP callbacks
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onInterstitialAdClosed(JNIEnv *env, jclass, jint text)
        {
            
           // cocos2d::__NotificationCenter::getInstance()->postNotification("onVideoDownloading", nullptr);
         //   NotificationCenter::getInstance()->postNotification("onInterstitialAdClosed", nullptr);



          //  cocos2d::__NotificationCenter::getInstance()->postNotification("onVideoDownloading",progressVal);

        }
    }



#endif

