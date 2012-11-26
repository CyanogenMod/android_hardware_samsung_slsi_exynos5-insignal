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
