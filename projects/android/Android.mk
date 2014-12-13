LOCAL_PATH := $(call my-dir)/../../

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS	:= -Wall \
-W \
-O2 \
-pipe \
-fPIC \
-D_ANDROID \

LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions

LOCAL_CLANG=true 

LOCAL_MODULE    := easysqlite_android

LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH) \


$(warning $(LOCAL_PATH))
LOCAL_C_INCLUDES:= \
$(ANDROIDNDK_PATH)/sources/android/support/include \
$(ANDROIDNDK_PATH)/sources/cxx-stl/llvm-libc++/libcxx/include \
$(ANDROIDNDK_PATH)/platforms/android-19/arch-arm/usr/include \
$(LOCAL_PATH) \
$(LOCAL_PATH)/easySQLite/easySQLite/easySQLite \
$(LOCAL_PATH)/../sqlite/build \



LOCAL_SRC_FILES := easySQLite/easySQLite/easySQLite/SHA1.cpp \
easySQLite/easySQLite/easySQLite/SqlCommon.cpp \
easySQLite/easySQLite/easySQLite/SqlDatabase.cpp \
easySQLite/easySQLite/easySQLite/SqlField.cpp \
easySQLite/easySQLite/easySQLite/SqlFieldSet.cpp \
easySQLite/easySQLite/easySQLite/SqlRecord.cpp \
easySQLite/easySQLite/easySQLite/SqlRecordSet.cpp \
easySQLite/easySQLite/easySQLite/SqlTable.cpp \
easySQLite/easySQLite/easySQLite/SqlValue.cpp \

include $(BUILD_STATIC_LIBRARY)

