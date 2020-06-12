LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)
$(call import-add-path, $(LOCAL_PATH))

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame




LOCAL_SRC_FILES := hellocpp/main.cpp \
../../../Classes/AppDelegate.cpp \
../../../Classes/DNAcheivementManager/DNAcheivementManager.cpp../../../Classes/DNDataManager/DNDataManager.cpp \
../../../Classes/DNDOManager/DNDOCharLabel.cpp../../../Classes/DNDOManager/DNDOManager.cpp../../../Classes/DNSplashScreen/DNSplashScreen.cpp../../../Classes/Scene/DNTutorialLayer/DNTutoriallayer.cpp../../../Classes/Scene/DNMainMenuLayer/DNBgPackLayer.cpp../../../Classes/Scene/DNMainMenuLayer/DNCharacterPack.cpp../../../Classes/Scene/DNSettingScene/DNSettingScene.cpp../../../Classes/Scene/DNGameScene/DNVictoryMenu.cpp../../../Classes/Scene/DNGameScene/DNBackGroundLayer/DNBackGroundLayer.cpp \
../../../Classes/Scene/DNGameScene/DNGameScene.cpp \
../../../Classes/Scene/DNGameScene/DNHUDLayer.cpp \
../../../Classes/Scene/DNGameScene/DNPauseMenu.cpp../../../Classes/Scene/DNGameScene/DNReviveMenu.cpp../../../Classes/Scene/DNMainMenuLayer/DNMainMenuLayer.cpp../../../Classes/Scene/DNGameSceneLayer/DNGameSceneLayer.cpp \
../../../Classes/Scene/DNGameSceneLayer/DNCharacterSprite/DNCharacterSprite.cpp \
../../../Classes/Scene/DNGameSceneLayer/DNObstecleSprite/DNObstecleSprite.cpp \
../../../Classes/Scene/DNGameScene/UserDialogueLayer.cpp \
../../../Classes/DNNetworkCheck/DNGameUtility.cpp \
../../../Classes/DNNetworkCheck/DNCommonUtils.cpp \
../../../Classes/Scene/DNMainMenuLayer/SpriteFromURL.cpp \
../../../Classes/Scene/DNGameSceneLayer/DNMashroomSprite/DNMashroomSprite.cpp../../../Classes/JNICommunicator/LVJniCommunicator.cpp







LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../ClassesLOCAL_C_INCLUDES




LOCAL_CPPFLAGS := -DSDKBOX_ENABLED
LOCAL_LDLIBS := -landroid \
-llog

LOCAL_WHOLE_STATIC_LIBRARIES := PluginIAP \
sdkbox \
PluginFacebook \
PluginShare \
PluginAdMob \
PluginSdkboxPlay \
PluginFirebase \
PluginSdkboxAds \
PluginOneSignal

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module, ./sdkbox)
$(call import-module, ./PluginSdkboxPlay)
$(call import-module, ./PluginShare)
$(call import-module, ./PluginSdkboxAds)
$(call import-module, ./PluginAdMob)
$(call import-module, ./PluginFirebase)
$(call import-module, ./PluginFacebook)
$(call import-module, ./PluginOneSignal)
$(call import-module, ./PluginIAP)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
