/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_EXYNOS_HWC_H_
#define ANDROID_EXYNOS_HWC_H_
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <s3c-fb.h>

#include <EGL/egl.h>

#define HWC_REMOVE_DEPRECATED_VERSIONS 1

#include <cutils/compiler.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <hardware/gralloc.h>
#include <hardware/hardware.h>
#include <hardware/hwcomposer.h>
#include <hardware_legacy/uevent.h>
#include <utils/String8.h>
#include <utils/Vector.h>

#include <sync/sync.h>

#include "ion.h"
#include "gralloc_priv.h"
#include "exynos_gscaler.h"
#include "exynos_format.h"
#include "exynos_v4l2.h"
#include "s5p_tvout_v4l2.h"
#include "ExynosHWCModule.h"
#include "videodev2.h"

const size_t NUM_HW_WINDOWS = 5;
const size_t NO_FB_NEEDED = NUM_HW_WINDOWS + 1;
const size_t MAX_PIXELS = 2560 * 1600 * 2;
const size_t GSC_W_ALIGNMENT = 16;
const size_t GSC_H_ALIGNMENT = 16;
const size_t GSC_DST_H_ALIGNMENT_RGB888 = 1;
const size_t FIMD_GSC_IDX = 0;
const size_t HDMI_GSC_IDX = 1;
const size_t FIMD_GSC_SBS_IDX = 2;
const size_t FIMD_GSC_TB_IDX = 3;
const size_t HDMI_GSC_SBS_IDX = 4;
const size_t HDMI_GSC_TB_IDX = 5;
const int AVAILABLE_GSC_UNITS[] = { 0, 3, 0, 0, 3, 3 };
const size_t NUM_GSC_UNITS = sizeof(AVAILABLE_GSC_UNITS) /
        sizeof(AVAILABLE_GSC_UNITS[0]);
const size_t BURSTLEN_BYTES = 16 * 8;
const size_t NUM_HDMI_BUFFERS = 3;

#ifdef HWC_SERVICES
#include "../libhwcService/ExynosHWCService.h"
namespace android {
class ExynosHWCService;
}
#endif

#ifdef HWC_DYNAMIC_RECOMPOSITION
#define HWC_FIMD_BW_TH  1.5   /* valid range 1 to 5 */
#define HWC_FPS_TH          3    /* valid range 1 to 60 */
#define VSYNC_INTERVAL (1000000000.0 / 60)
typedef enum _COMPOS_MODE_SWITCH {
    NO_MODE_SWITCH,
    HWC_2_GLES = 1,
    GLES_2_HWC,
} HWC_COMPOS_MODE_SWITCH;
#endif

struct exynos5_hwc_composer_device_1_t;

struct exynos5_gsc_map_t {
    enum {
        GSC_NONE = 0,
        GSC_M2M,
        // TODO: GSC_LOCAL_PATH
    } mode;
    int idx;
};

struct exynos5_hwc_post_data_t {
    int                 overlay_map[NUM_HW_WINDOWS];
    exynos5_gsc_map_t   gsc_map[NUM_HW_WINDOWS];
    size_t              fb_window;
};

const size_t NUM_GSC_DST_BUFS = 3;
struct exynos5_gsc_data_t {
    void            *gsc;
    exynos_gsc_img  src_cfg;
    exynos_gsc_img  dst_cfg;
    buffer_handle_t dst_buf[NUM_GSC_DST_BUFS];
    int             dst_buf_fence[NUM_GSC_DST_BUFS];
    size_t          current_buf;
};

struct hdmi_layer_t {
    int     id;
    int     fd;
    bool    enabled;
    exynos_gsc_img  cfg;

    bool    streaming;
    size_t  current_buf;
    size_t  queued_buf;
};

struct exynos5_hwc_composer_device_1_t {
    hwc_composer_device_1_t base;

    int                     fd;
    int                     vsync_fd;
    exynos5_hwc_post_data_t bufs;

    const private_module_t  *gralloc_module;
    alloc_device_t          *alloc_device;
    const hwc_procs_t       *procs;
    pthread_t               vsync_thread;
    int                     force_gpu;

    int32_t                 xres;
    int32_t                 yres;
    int32_t                 xdpi;
    int32_t                 ydpi;
    int32_t                 vsync_period;

    int  hdmi_mixer0;
    bool hdmi_hpd;
    bool hdmi_enabled;
    bool hdmi_blanked;
    int  hdmi_w;
    int  hdmi_h;

    hdmi_layer_t            hdmi_layers[2];

    exynos5_gsc_data_t      gsc[NUM_GSC_UNITS];

    struct s3c_fb_win_config last_config[NUM_HW_WINDOWS];
    size_t                  last_fb_window;
    const void              *last_handles[NUM_HW_WINDOWS];
    exynos5_gsc_map_t       last_gsc_map[NUM_HW_WINDOWS];
#ifdef HWC_SERVICES

#define S3D_ERROR -1
#define HDMI_PRESET_DEFAULT V4L2_DV_1080P60
#define HDMI_PRESET_ERROR -1

    android::ExynosHWCService   *mHWCService;
    int mHdmiPreset;
    int mHdmiCurrentPreset;
    bool mHdmiResolutionChanged;
    bool mHdmiResolutionHandled;
    int mS3DMode;
    bool mUseSubtitles;
#endif
    int force_mirror_mode;

#ifdef HWC_DYNAMIC_RECOMPOSITION
    int VsyncInterruptStatus;
    int CompModeSwitch;
    uint64_t LastVsyncTimeStamp;
    uint64_t LastModeSwitchTimeStamp;
    int invalidateStatus;
    int needInvalidate;
    int totPixels;
    int setCallCnt;
    pthread_t   vsync_stat_thread;
    int vsyn_event_cnt;
    int invalid_trigger;
#endif
};

#if defined(HWC_SERVICES)
enum {
    S3D_MODE_DISABLED = 0,
    S3D_MODE_READY,
    S3D_MODE_RUNNING,
    S3D_MODE_STOPPING,
};

enum {
    S3D_FB = 0,
    S3D_SBS,
    S3D_TB,
    S3D_NONE,
};
#endif
#endif
