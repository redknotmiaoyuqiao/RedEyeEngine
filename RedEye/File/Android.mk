LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__ANDROID__ -std=c++11 -fpermissive -Wno-write-strings

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm
LOCAL_C_INCLUDES += ./IMAGE/

LOCAL_SRC_FILES += ./ImageFile.cpp
LOCAL_SRC_FILES += ./IMAGE/stb_image.cpp

#$(warning $(LOCAL_SRC_FILES))

LOCAL_MODULE := File

include $(BUILD_STATIC_LIBRARY)
