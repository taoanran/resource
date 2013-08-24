LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(common_C_INCLUDES) \

LOCAL_SRC_FILES := \
   catchPicFromFB.cpp 

LOCAL_SHARED_LIBRARIES := \
    libutils libcutils


LOCAL_MODULE := catchPicFromFB 

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
