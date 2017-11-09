LOCAL_PATH := $(call my-dir)
#APP_PLATFORM := android-19
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__ANDROID__ -std=c++11 -fpermissive -Wno-write-strings

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm/

LOCAL_SRC_FILES += ./Camera.cpp
LOCAL_SRC_FILES += ./GameObject.cpp
LOCAL_SRC_FILES += ./HdriSkybox.cpp
LOCAL_SRC_FILES += ./Light.cpp
LOCAL_SRC_FILES += ./Material.cpp
LOCAL_SRC_FILES += ./PBRMaterial.cpp
LOCAL_SRC_FILES += ./Skybox.cpp
LOCAL_SRC_FILES += ./Spirit3D.cpp

#$(warning $(LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES += RedGL
LOCAL_STATIC_LIBRARIES += Engine

LOCAL_MODULE := Component
include $(BUILD_STATIC_LIBRARY)
