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

int ExynosHWCService::setHDMIMirrorMode(unsigned int mode)
{
    ALOGD_IF(HWC_SERVICE_DEBUG, "%s::mode=%d", __func__, mode);
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

void ExynosHWCService::setHdmiResolution(int resolution, int s3dMode)
{
}

void ExynosHWCService::setHdmiCableStatus(int status)
{
}

void ExynosHWCService::setHdmiMode(int mode)
{
}

void ExynosHWCService::setHdmiHdcp(int status)
{
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
}

uint32_t ExynosHWCService::getHdmiCableStatus()
{
    return 0;
}

uint32_t ExynosHWCService::getHdmiAudioChannel()
{
    return 0;
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
