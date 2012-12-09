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

#ifndef ANDROID_EXYNOS_IHWC_H_
#define ANDROID_EXYNOS_IHWC_H_

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>

namespace android {

class IExynosHWCService : public IInterface {
public:

    DECLARE_META_INTERFACE(ExynosHWCService);

    /*
     * setWFDMode() function sets the WFD operation Mode.
     * It enables / disables the WFD.
     */
    virtual int setWFDMode(unsigned int mode) = 0;

    /*
     * setExtraFBMode() function Enables / disables the extra FB usage.
     */
    virtual int setExtraFBMode(unsigned int mode) = 0;
    virtual int setCameraMode(unsigned int mode) = 0;
    virtual int setHDMIMirrorMode(unsigned int mode) = 0;
    virtual int setVideoSeekStatus(unsigned int mode) = 0;
    virtual int setDispOrientation(unsigned int transform) = 0;
    virtual int setProtectionMode(unsigned int mode) = 0;
    virtual int setExternalDispLayerNum(unsigned int num) = 0;
    virtual int setForceGPU(unsigned int on) = 0;

    virtual void setHdmiResolution(int resolution, int s3dMode) = 0;
    virtual void setHdmiCableStatus(int status) = 0;
    virtual void setHdmiMode(int mode) = 0;
    virtual void setHdmiHdcp(int status) = 0;
    virtual void setHdmiDRM(bool status) = 0;
    virtual void setHdmiHwcLayer(uint32_t hwcLayer) = 0;
    virtual void setHdmiEnable(uint32_t enable) = 0;
    virtual void setHdmiLayerEnable(uint32_t hdmiLayer) = 0;
    virtual void setHdmiLayerDisable(uint32_t hdmiLayer) = 0;
    /*
     * These functions may not be necessary
     */
    /* GPU does rotation now in normal scenarios */
    virtual void setHdmiRotate(int rotVal, uint32_t hwcLayer) = 0;
    /* We don't use writeback anymore */
    virtual void setHdmiPath(int path) = 0;
    /* Obsoleted by setProtectionMode */
    virtual void setHdmiDRM(int drmMode) = 0;
    /*
     * End of possibly unnecessary functions
     */
    virtual void getHdmiResolution(uint32_t *width, uint32_t *height) = 0;
    virtual uint32_t getHdmiCableStatus() = 0;
    virtual uint32_t getHdmiAudioChannel() = 0;
};

/* Native Interface */
class BnExynosHWCService : public BnInterface<IExynosHWCService> {
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);

};
}
#endif
