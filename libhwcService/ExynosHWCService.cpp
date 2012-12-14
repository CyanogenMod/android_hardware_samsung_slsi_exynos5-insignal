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
#include "ExynosHWCService.h"
#include "exynos_v4l2.h"
#include "videodev2_exynos_media.h"

#define HWC_SERVICE_DEBUG 1

namespace android {

ANDROID_SINGLETON_STATIC_INSTANCE(ExynosHWCService);

ExynosHWCService::ExynosHWCService() :
    mHWCService(NULL),
    mHWCCtx(NULL)
{
   ALOGD_IF(HWC_SERVICE_DEBUG, "ExynosHWCService Constructor is called");
}

ExynosHWCService::~ExynosHWCService()
{
   ALOGD_IF(HWC_SERVICE_DEBUG, "ExynosHWCService Destructor is called");
}

int ExynosHWCService::setWFDMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    return NO_ERROR;
}

int ExynosHWCService::setExtraFBMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    return NO_ERROR;
}

int ExynosHWCService::setCameraMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    return NO_ERROR;
}

int ExynosHWCService::setForceMirrorMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    mHWCCtx->force_mirror_mode = mode;
    mHWCCtx->procs->invalidate(mHWCCtx->procs);
    return NO_ERROR;
}

int ExynosHWCService::setVideoSeekStatus(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    return NO_ERROR;
}

int ExynosHWCService::setDispOrientation(unsigned int transform)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%x", __func__, transform);
    return NO_ERROR;
}

int ExynosHWCService::setProtectionMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
    return NO_ERROR;
}

int ExynosHWCService::setExternalDispLayerNum(unsigned int num)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, num);
    return NO_ERROR;
}

int ExynosHWCService::setForceGPU(unsigned int on)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::on/off=%d", __func__, on);
    mHWCCtx->force_gpu = on;
    mHWCCtx->procs->invalidate(mHWCCtx->procs);
    return NO_ERROR;
}

void ExynosHWCService::setHdmiResolution(int resolution, int s3dMode)
{
    if (resolution == 0)
        resolution = mHWCCtx->mHdmiCurrentPreset;

    if (s3dMode == S3D_NONE) {
        mHWCCtx->mHdmiPreset = resolution;
        mHWCCtx->mHdmiResolutionChanged = true;
        mHWCCtx->procs->invalidate(mHWCCtx->procs);
        return;
    }

    switch (resolution) {
    case HDMI_720P_60:
        resolution = S3D_720P_60_BASE + s3dMode;
        break;
    case HDMI_720P_59_94:
        resolution = S3D_720P_59_94_BASE + s3dMode;
        break;
    case HDMI_720P_50:
        resolution = S3D_720P_50_BASE + s3dMode;
        break;
    case HDMI_1080P_24:
        resolution = S3D_1080P_24_BASE + s3dMode;
        break;
    case HDMI_1080P_23_98:
        resolution = S3D_1080P_23_98_BASE + s3dMode;
        break;
    case HDMI_1080P_30:
        resolution = S3D_1080P_30_BASE + s3dMode;
        break;
    case HDMI_1080I_60:
        if (s3dMode != S3D_SBS)
            return;
        resolution = V4L2_DV_1080I60_SB_HALF;
        break;
    case HDMI_1080I_59_94:
        if (s3dMode != S3D_SBS)
            return;
        resolution = V4L2_DV_1080I59_94_SB_HALF;
        break;
    case HDMI_1080P_60:
        if (s3dMode != S3D_SBS && s3dMode != S3D_TB)
            return;
        resolution = S3D_1080P_60_BASE + s3dMode;
        break;
    default:
        return;
    }
    mHWCCtx->mHdmiPreset = resolution;
    mHWCCtx->mHdmiResolutionChanged = true;
    mHWCCtx->mS3DMode = S3D_MODE_READY;
    mHWCCtx->procs->invalidate(mHWCCtx->procs);
}

void ExynosHWCService::setHdmiCableStatus(int status)
{
    mHWCCtx->hdmi_hpd = !!status;
}

void ExynosHWCService::setHdmiMode(int mode)
{
}

void ExynosHWCService::setHdmiHdcp(int status)
{
    if (exynos_v4l2_s_ctrl(mHWCCtx->hdmi_layers[1].fd, V4L2_CID_TV_HDCP_ENABLE,
                           !!status) < 0)
        ALOGE("%s: s_ctrl(CID_TV_HDCP_ENABLE) failed %d", __func__, errno);
}

void ExynosHWCService::setHdmiDRM(bool status)
{
}

void ExynosHWCService::setHdmiHwcLayer(uint32_t hwcLayer)
{
}

void ExynosHWCService::setHdmiEnable(uint32_t enable)
{
}

void ExynosHWCService::setHdmiLayerEnable(uint32_t hdmiLayer)
{
}

void ExynosHWCService::setHdmiLayerDisable(uint32_t hdmiLayer)
{
}

void ExynosHWCService::setHdmiAudioChannel(uint32_t channels)
{
    if (exynos_v4l2_s_ctrl(mHWCCtx->hdmi_layers[0].fd,
            V4L2_CID_TV_SET_NUM_CHANNELS, channels) < 0)
        ALOGE("%s: failed to set audio channels", __func__);
}

void ExynosHWCService::setHdmiSubtitles(bool use)
{
    mHWCCtx->mUseSubtitles = use;
}

void ExynosHWCService::setHdmiRotate(int rotVal, uint32_t hwcLayer)
{
}

void ExynosHWCService::setHdmiPath(int path)
{
}

void ExynosHWCService::setHdmiDRM(int drmMode)
{
}

void ExynosHWCService::getHdmiResolution(uint32_t *width, uint32_t *height)
{
    switch (mHWCCtx->mHdmiCurrentPreset) {
    case V4L2_DV_480P59_94:
    case V4L2_DV_480P60:
        *width = 640;
        *height = 480;
        break;
    case V4L2_DV_576P50:
        *width = 720;
        *height = 576;
        break;
    case V4L2_DV_720P24:
    case V4L2_DV_720P25:
    case V4L2_DV_720P30:
    case V4L2_DV_720P50:
    case V4L2_DV_720P59_94:
    case V4L2_DV_720P60:
    case V4L2_DV_720P60_FP:
    case V4L2_DV_720P60_SB_HALF:
    case V4L2_DV_720P60_TB:
    case V4L2_DV_720P59_94_FP:
    case V4L2_DV_720P59_94_SB_HALF:
    case V4L2_DV_720P59_94_TB:
    case V4L2_DV_720P50_FP:
    case V4L2_DV_720P50_SB_HALF:
    case V4L2_DV_720P50_TB:
        *width = 1280;
        *height = 720;
        break;
    case V4L2_DV_1080I29_97:
    case V4L2_DV_1080I30:
    case V4L2_DV_1080I25:
    case V4L2_DV_1080I50:
    case V4L2_DV_1080I60:
    case V4L2_DV_1080P24:
    case V4L2_DV_1080P25:
    case V4L2_DV_1080P30:
    case V4L2_DV_1080P50:
    case V4L2_DV_1080P60:
    case V4L2_DV_1080I59_94:
    case V4L2_DV_1080P59_94:
    case V4L2_DV_1080P24_FP:
    case V4L2_DV_1080P24_SB_HALF:
    case V4L2_DV_1080P24_TB:
    case V4L2_DV_1080P23_98_FP:
    case V4L2_DV_1080P23_98_SB_HALF:
    case V4L2_DV_1080P23_98_TB:
    case V4L2_DV_1080I60_SB_HALF:
    case V4L2_DV_1080I59_94_SB_HALF:
    case V4L2_DV_1080I50_SB_HALF:
    case V4L2_DV_1080P60_SB_HALF:
    case V4L2_DV_1080P60_TB:
    case V4L2_DV_1080P30_FP:
    case V4L2_DV_1080P30_SB_HALF:
    case V4L2_DV_1080P30_TB:
        *width = 1920;
        *height = 1080;
        break;
    }
}

uint32_t ExynosHWCService::getHdmiCableStatus()
{
    return !!mHWCCtx->hdmi_hpd;
}

uint32_t ExynosHWCService::getHdmiAudioChannel()
{
    int channels;
    if (exynos_v4l2_g_ctrl(mHWCCtx->hdmi_layers[0].fd,
            V4L2_CID_TV_MAX_AUDIO_CHANNELS, &channels) < 0)
        ALOGE("%s: failed to get audio channels", __func__);
    return channels;
}

int ExynosHWCService::createServiceLocked()
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::", __func__);
    sp<IServiceManager> sm = defaultServiceManager();
    sm->addService(String16("Exynos.HWCService"), mHWCService);
    if (sm->checkService(String16("Exynos.HWCService")) != NULL) {
        ALOGD_IF(HWC_SERVICE_DEBUG, "adding Exynos.HWCService succeeded");
        return 0;
    } else {
        ALOGE_IF(HWC_SERVICE_DEBUG, "adding Exynos.HWCService failed");
        return -1;
    }
}

ExynosHWCService *ExynosHWCService::getExynosHWCService()
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::", __func__);
    ExynosHWCService& instance = ExynosHWCService::getInstance();
    Mutex::Autolock _l(instance.mLock);
    if (instance.mHWCService == NULL) {
        instance.mHWCService = &instance;

        int status = ExynosHWCService::getInstance().createServiceLocked();
        if (status != 0) {
            ALOGE_IF(HWC_SERVICE_DEBUG, "getExynosHWCService failed");
        }
    }
    return instance.mHWCService;
}

void ExynosHWCService::setExynosHWCCtx(ExynosHWCCtx *HWCCtx)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "HWCCtx=0x%x", (int)HWCCtx);
    if(HWCCtx) {
        mHWCCtx = HWCCtx;
    }
}

}
