LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
	swconvertor.c \
	csc_tiled_to_linear_y_neon.s \
	csc_tiled_to_linear_uv_neon.s \
	csc_tiled_to_linear_uv_deinterleave_neon.s \
	csc_interleave_memcpy_neon.s \
	csc_ARGB8888_to_YUV420SP_NEON.s \
	csc_ARGB8888_to_ABGR8888.s

ifeq ($(BOARD_USE_KHRONOS_OMX_HEADER),true)
LOCAL_C_INCLUDES += $(TOP)/hardware/samsung_slsi/openmax/include/khronos
else
LOCAL_C_INCLUDES += $(TOP)/frameworks/native/include/media/openmax
endif
	
LOCAL_C_INCLUDES := \
	$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include \
	$(LOCAL_PATH)/../include \
	$(TOP)/hardware/samsung_slsi/exynos/include

LOCAL_ADDITIONAL_DEPENDENCIES := \
	$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

LOCAL_MODULE := libswconverter

LOCAL_PRELINK_MODULE := false

LOCAL_CFLAGS :=

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := liblog

include $(BUILD_STATIC_LIBRARY)
