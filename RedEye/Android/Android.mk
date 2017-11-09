LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := redeye

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3 -lz
LOCAL_CFLAGS := -std=c++11

$(warning $(LOCAL_PATH))

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../

LOCAL_SRC_FILES := interface.cpp

LOCAL_STATIC_LIBRARIES += App
LOCAL_STATIC_LIBRARIES += Engine

include $(BUILD_SHARED_LIBRARY)
