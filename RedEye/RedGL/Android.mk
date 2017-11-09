LOCAL_PATH := $(call my-dir)
#APP_PLATFORM := android-19
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__ANDROID__ -std=c++11 -fpermissive -Wno-write-strings

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm/

LOCAL_SRC_FILES += ./GLProgram.cpp
LOCAL_SRC_FILES += ./GLRenderTarget.cpp
LOCAL_SRC_FILES += ./GLShader.cpp
LOCAL_SRC_FILES += ./GLTexture.cpp
LOCAL_SRC_FILES += ./GLVAO.cpp

#$(warning $(LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES += File

LOCAL_MODULE := RedGL
include $(BUILD_STATIC_LIBRARY)
