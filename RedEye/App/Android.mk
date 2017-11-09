LOCAL_PATH := $(call my-dir)
#APP_PLATFORM := android-19
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__ANDROID__ -std=c++11 -fpermissive -Wno-write-strings

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm/

LOCAL_C_INCLUDES += ./App.hpp

LOCAL_SRC_FILES += ./App.cpp

#$(warning $(LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES += RedGL
LOCAL_STATIC_LIBRARIES += Component

LOCAL_MODULE := App
include $(BUILD_STATIC_LIBRARY)
