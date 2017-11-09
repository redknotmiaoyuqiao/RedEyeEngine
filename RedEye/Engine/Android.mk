LOCAL_PATH := $(call my-dir)
#APP_PLATFORM := android-19
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__ANDROID__ -std=c++11 -fpermissive -Wno-write-strings

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../glm/

LOCAL_SRC_FILES += ./Input.cpp
LOCAL_SRC_FILES += ./Mesh.cpp
LOCAL_SRC_FILES += ./Model.cpp
LOCAL_SRC_FILES += ./Screen.cpp

#$(warning $(LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES += RedGL

LOCAL_MODULE := Engine
include $(BUILD_STATIC_LIBRARY)
