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

#ifndef ANDROID_EXYNOS_HWC_SERVICE_H_
#define ANDROID_EXYNOS_HWC_SERVICE_H_

#include <utils/Errors.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <binder/IServiceManager.h>
#include <utils/Singleton.h>
#include <utils/StrongPointer.h>
#include "IExynosHWC.h"
#include "ExynosHWC.h"

typedef struct exynos5_hwc_composer_device_1_t ExynosHWCCtx;

namespace android {

class ExynosHWCService
    : public BnExynosHWCService,  Singleton<ExynosHWCService> {

public:
    static ExynosHWCService* getExynosHWCService();
    ~ExynosHWCService();

    virtual int setWFDMode(unsigned int mode);
    virtual int setExtraFBMode(unsigned int mode);
    virtual int setCameraMode(unsigned int mode);
    virtual int setHDMIMirrorMode(unsigned int mode);
    virtual int setVideoSeekStatus(unsigned int mode);
    virtual int setDispOrientation(unsigned int transform);
    virtual int setProtectionMode(unsigned int mode);
    virtual int setExternalDispLayerNum(unsigned int num);
    virtual int setForceGPU(unsigned int on);

    virtual void setHdmiResolution(int resolution, int s3dMode);
    virtual void setHdmiCableStatus(int status);
    virtual void setHdmiMode(int mode);
    virtual void setHdmiHdcp(int status);
    virtual void setHdmiDRM(bool status);
    virtual void setHdmiHwcLayer(uint32_t hwcLayer);
    virtual void setHdmiEnable(uint32_t enable);
    virtual void setHdmiLayerEnable(uint32_t hdmiLayer);
    virtual void setHdmiLayerDisable(uint32_t hdmiLayer);
    /*
     * These functions may not be necessary
     */
    /* GPU does rotation now in normal scenarios */
    virtual void setHdmiRotate(int rotVal, uint32_t hwcLayer);
    /* We don't use writeback anymore */
    virtual void setHdmiPath(int path);
    /* Obsoleted by setProtectionMode */
    virtual void setHdmiDRM(int drmMode);
    /*
     * End of possibly unnecessary functions
     */
    virtual void getHdmiResolution(uint32_t *width, uint32_t *height);
    virtual uint32_t getHdmiCableStatus();
    virtual uint32_t getHdmiAudioChannel();

    void setExynosHWCCtx(ExynosHWCCtx *);
private:
    friend class Singleton<ExynosHWCService>;
    ExynosHWCService();
    int createServiceLocked();
    ExynosHWCService *mHWCService;
    Mutex mLock;
    ExynosHWCCtx *mHWCCtx;
};

}
#endif
