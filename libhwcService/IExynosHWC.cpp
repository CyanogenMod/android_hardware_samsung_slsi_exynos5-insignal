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

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Timers.h>

#include <binder/Parcel.h>
#include <binder/IInterface.h>

#include "IExynosHWC.h"

namespace android {

enum {
    SET_WFD_MODE = 0,
    SET_EXT_FB_MODE,
    SET_CAMERA_MODE,
    SET_HDMI_MIRROR_MODE,
    SET_VIDEO_SEEK_STATUS,
    SET_DISPLAY_ORIENTATION,
    SET_PROTECTION_MODE,
    SET_EXTERNAL_DISP_LAY_NUM,
};

class BpExynosHWCService : public BpInterface<IExynosHWCService> {
public:
    BpExynosHWCService(const sp<IBinder>& impl)
        : BpInterface<IExynosHWCService>(impl)
    {
    }

    virtual int setWFDMode(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_WFD_MODE, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setExtraFBMode(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_EXT_FB_MODE, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setCameraMode(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_CAMERA_MODE, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setHDMIMirrorMode(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_HDMI_MIRROR_MODE, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setVideoSeekStatus(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_VIDEO_SEEK_STATUS, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setDispOrientation(unsigned int transform)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(transform);
        int result = remote()->transact(SET_DISPLAY_ORIENTATION, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setProtectionMode(unsigned int mode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(mode);
        int result = remote()->transact(SET_PROTECTION_MODE, data, &reply);
        result = reply.readInt32();
        return result;
    }

    virtual int setExternalDispLayerNum(unsigned int num)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IExynosHWCService::getInterfaceDescriptor());
        data.writeInt32(num);
        int result = remote()->transact(SET_EXTERNAL_DISP_LAY_NUM, data, &reply);
        result = reply.readInt32();
        return result;
    }
};

IMPLEMENT_META_INTERFACE(ExynosHWCService, "android.hal.ExynosHWCService");

status_t BnExynosHWCService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case SET_WFD_MODE: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setWFDMode(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_EXT_FB_MODE: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setExtraFBMode(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_CAMERA_MODE: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setCameraMode(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_HDMI_MIRROR_MODE: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setHDMIMirrorMode(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_VIDEO_SEEK_STATUS: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setVideoSeekStatus(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_DISPLAY_ORIENTATION: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int transform = data.readInt32();
            int res = setDispOrientation(transform);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_PROTECTION_MODE: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int mode = data.readInt32();
            int res = setProtectionMode(mode);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        case SET_EXTERNAL_DISP_LAY_NUM: {
            CHECK_INTERFACE(IExynosHWCService, data, reply);
            int num = data.readInt32();
            int res = setExternalDispLayerNum(num);
            reply->writeInt32(res);
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}
}
