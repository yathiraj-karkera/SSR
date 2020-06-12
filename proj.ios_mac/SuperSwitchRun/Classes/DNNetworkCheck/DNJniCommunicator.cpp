//
//  DNJniCommunicator.cpp
//  ColorCube
//
//
//

#include "DNJniCommunicator.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
#define  LOG_TAG    "libSimpleAudioEngine"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"
#include "cocos2d.h"

static DNJniCommunicator *gSharedManager = NULL;

DNJniCommunicator::DNJniCommunicator()
{
    // TODO Auto-generated constructor stub
    
}

DNJniCommunicator::~DNJniCommunicator() {
    // TODO Auto-generated destructor stub
}

DNJniCommunicator* DNJniCommunicator::sharedManager() {
    
    DNJniCommunicator *pRet = gSharedManager;
    
    if (!gSharedManager) {
        
        pRet = gSharedManager = new DNJniCommunicator();
        
        if (!gSharedManager->init()) {
            
            delete gSharedManager;
            gSharedManager = NULL;
            pRet = NULL;
        }
    }
    
    return pRet;
}
#pragma mark - Init
bool DNJniCommunicator::init(void) {
    
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// get env and cache it
static JNIEnv* getJNIEnv(void)
{
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    if (NULL == jvm) {
        LOGD("Failed to get JNIEnv. JniHelper::getJavaVM() is NULL");
        return NULL;
    }
    
    JNIEnv *env = NULL;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
    
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
                LOGD("Failed to get the environment using AttachCurrentThread()");
                return NULL;
            } else {
                // Success : Attached and obtained JNIEnv!
                return env;
            }
            
        case JNI_EVERSION :
            // Cannot recover from this error
            LOGD("JNI interface version 1.4 not supported");
        default :
            LOGD("Failed to get the environment using GetEnv()");
            return NULL;
    }
}

#endif

#pragma mark - Get DeviceId
void DNJniCommunicator::getDeviceId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/Cpp_To_Java");//com.fanta.fantagameon.Facebook
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    
    methodID = pEnv->GetMethodID(cls, "get_Device_ID", "()V");
    
    if (! methodID)
    {
//        CCLOG(" --- Failed to find static method id of  --- %s - ", "get_Device_ID");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,fbObject);
    
    pEnv->DeleteLocalRef(obj);
    
//    CCLOG("connect method end");
#endif
    
}
#pragma mark - Network check
bool DNJniCommunicator::isConnetToNetworkAnd()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    
    jclass ret = pEnv->FindClass("org/cocos2dx/cpp/BaseConnection");
    methodID = pEnv->GetStaticMethodID(ret, "isConnectedToNetwork", "()Z");
    
    if (! methodID)
    {
//        CCLOG("Failed to find static method id of %s", "isConnetToNetwork");
//         CCLOG("Failed to find static method id of %s", "isConnetToNetwork");
        return false;
    }
    jboolean isConnect;
    
    isConnect=  pEnv->CallStaticBooleanMethod(ret,methodID);
    pEnv->DeleteLocalRef(ret);
    return isConnect;
#endif
    
    return true;
}

#pragma mark - Local Notification
 void DNJniCommunicator::checkCall()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/Cpp_To_Java");//com.fanta.fantagameon.Facebook
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    
    methodID = pEnv->GetMethodID(cls, "checkCall", "()V");
    
    if (! methodID)
    {
               CCLOG(" --- Failed to find static method id of  --- %s - ", "checkCall");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,fbObject);
    
    pEnv->DeleteLocalRef(obj);
#endif
}
#pragma mark - Local Notification
void DNJniCommunicator::fireLocalNotification(float secs,int day, const char* msg,const char* title,int uniqueNotificationID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/Cpp_To_Java");//com.fanta.fantagameon.Facebook
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    jstring msgStr = pEnv->NewStringUTF(msg);
    jstring titleStr = pEnv->NewStringUTF(title);
    
    methodID = pEnv->GetMethodID(cls, "fireLocalNotification", "(FILjava/lang/String;Ljava/lang/String;I)V");
    
    if (! methodID)
    {
        CCLOG(" --- Failed to find static method id of  fireLocalNotification");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,secs,day,msgStr,titleStr,uniqueNotificationID);
    
    pEnv->DeleteLocalRef(obj);
    
     CCLOG("connect method end ------------------------------ called local notification");
#endif
    
}
void DNJniCommunicator::cancelLocalNotification(int uniqueNotificationID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/Cpp_To_Java");//com.fanta.fantagameon.Facebook
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    methodID = pEnv->GetMethodID(cls, "cancelLocalNotification", "(I)V");
    
    if (! methodID)
    {
        //CCLOG(" --- Failed to find static method id of  --- %s - ", "get_Device_ID");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,uniqueNotificationID);
    
    pEnv->DeleteLocalRef(obj);
    
    //CCLOG("cancelLocalNotification method end");
#endif
    
}


















//MARK:ACHIEVEMENTS
void DNJniCommunicator::unlockAchievement(int number)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    jint jproductid = (jint)number;
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    
    methodID = pEnv->GetMethodID(cls, "unlockAchievement", "(I)V");
    
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,jproductid);
    
    pEnv->DeleteLocalRef(obj);
    
#endif
    
}

void DNJniCommunicator::UpdateLeaderBoardScore(int score)
{
     CCLOG("UpdateLeaderBoardScore 11  = %d",score);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    jint jproductid = (jint)score;
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    
    methodID = pEnv->GetMethodID(cls, "UpdateLeaderBoardScore", "(I)V");
    
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID,score);
    
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::showAchievement()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");

//    jclass cls = pEnv->FindClass("org.cocos2dx.cpp.AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "showAchievement", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::showAndroidLeaderBoard()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "showLeaderBoard", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

//MARK: GOOGLE SIGN
void DNJniCommunicator::doGoogleSignin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "GooglePlusSignIn", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::doGoogleSignOut()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "signOut", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::doGooglePlusShare()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "GoogleShare", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::doGooglePlusShareWithMessage(const char *str)
{
    CCLOG("Do google plus share with message");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jstring jproductid = pEnv->NewStringUTF(str);

    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;

    
    methodID = pEnv->GetMethodID(cls, "GoogleShareWithMessage", "(Ljava/lang/String;)V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID, jproductid);
    pEnv->DeleteLocalRef(obj);
#endif
}
#pragma mark - facebook request
void DNJniCommunicator::checkFaceBookLifeRequest()
{
    CCLOG("Do facebook req -- ");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "getFaceBookGiftResponse", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif

}

void DNJniCommunicator::sendFaceBookLifeRequest()
{
    CCLOG("Do facebook sendFaceBookLifeRequest -- ");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    pEnv = getJNIEnv();
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    jobject obj = pEnv->AllocObject(cls);
    methodID = pEnv->GetMethodID(cls, "onClickRequestButton", "()V");
    if (! methodID)
    {
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID);
    pEnv->DeleteLocalRef(obj);
#endif
}

void DNJniCommunicator::giftRequestALife(const char *userId,const char *responseID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");//com.fanta.fantagameon.Facebook
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    jstring userIdstr = pEnv->NewStringUTF(userId);
    jstring responseIDstr = pEnv->NewStringUTF(responseID);
    
    methodID = pEnv->GetMethodID(cls, "giveGiftResponse", "(Ljava/lang/String;Ljava/lang/String;)V");
    
    if (! methodID)
    {
        CCLOG(" --- Failed to find static method id of  fireLocalNotification");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID, userIdstr,responseIDstr);
    
    pEnv->DeleteLocalRef(obj);
    
    CCLOG("connect method end ------------------------------ called local gift");
#endif
}

void DNJniCommunicator::removeFFaceBookRequestResponse(const char *responseID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    
    pEnv = getJNIEnv();
    
    
    jclass cls = pEnv->FindClass("org/cocos2dx/cpp/AppActivity");
    
    jobject obj = pEnv->AllocObject(cls);
    
    jlong fbObject  = (jlong)this;
    jstring responseIDstr = pEnv->NewStringUTF(responseID);
    
    methodID = pEnv->GetMethodID(cls, "removeRequestResponse", "(Ljava/lang/String;)V");
    
    if (! methodID)
    {
        CCLOG(" --- Failed to find static method id of  delete Response");
        return;
    }
    
    pEnv->CallVoidMethod(obj, methodID, responseIDstr);
    
    pEnv->DeleteLocalRef(obj);
    
    CCLOG("connect method end ------------------------------ called local notification");
#endif
}
//
//void DNJniCommunicator::sendLocalNotification(const char *str)
//{
//    CCLOG("Do google plus share with message");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    jmethodID methodID = 0;
//    JNIEnv *pEnv = 0;
//    pEnv = getJNIEnv();
//    jstring jproductid = pEnv->NewStringUTF(str);
//    
//    jclass cls = pEnv->FindClass("com.google.firebase.quickstart.fcm.MyFirebaseMessagingService");
//    jobject obj = pEnv->AllocObject(cls);
//    
//    jlong fbObject  = (jlong)this;
//    
//    
//    methodID = pEnv->GetMethodID(cls, "sendNotification", "(Ljava/lang/String;)V");
//    if (! methodID)
//    {
//        return;
//    }
//    
//    pEnv->CallVoidMethod(obj, methodID, jproductid);
//    pEnv->DeleteLocalRef(obj);
//#endif
//}
//



#pragma mark - Java to CPP callbacks
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JavaTocpp_updateResponseId(JNIEnv* env, jobject obj, jstring messageContent, jstring name, jstring requestId, jstring responseId)
    {
        CCLOG("RETUREND RESPONSE TO ADD IN SCROLL VIEW");
        const char *nativeStringmessageContent = env->GetStringUTFChars(messageContent, 0);
        const char *nativeStringname = env->GetStringUTFChars(name, 0);
        const char *nativeStringrequestId = env->GetStringUTFChars(requestId, 0);
        const char *nativeStringresponseId = env->GetStringUTFChars(responseId, 0);
        
//        CSDataManager::sharedManager()->menuceneRef->updateFacebookResponseIds(nativeStringmessageContent,nativeStringname,nativeStringrequestId,nativeStringresponseId);
    }
    
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JavaTocpp_addResponseInScrollView(JNIEnv* env, jobject obj)
    {
        CCLOG("RETUREND RESPONSE TO ADD IN SJava_org_cocos2dx_cpp_JavaTocpp_addResponseInScrollView");
//        CSDataManager::sharedManager()->menuceneRef->addResponseInScrollView();
//        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create(CC_CALLBACK_0(CSMenuScene::addResponseInScrollView, CSDataManager::sharedManager()->menuceneRef)), NULL));

    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JavaTocpp_setFaceBookReceivedIdNull(JNIEnv* env, jobject obj)
    {
        CCLOG("make it null");
        UserDefault::getInstance()->setStringForKey("userRequestAlreadyReceived","");
    }
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JavaTocpp_clearaskALifeRequestVector(JNIEnv* env, jobject obj)
    {
        CCLOG("make it clear");
       // CSDataManager::sharedManager()->_askALifeRequestVector.clear();
    }
}
#endif

