/**
 * Volume Control C/C++ bindings for nwjs or nodejs
 */

#include <uv.h>
#include <node.h>
#include <gwlog.h>
#include "VolumeControl7.h"

using namespace v8;

const char* TAG = "volume";
const char* VERSION = "1.0.2";

static VolumeControl7* g_volumeControl = NULL;

static void SetIntToReturnValue(
    int ret, const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);
}

static VolumeControl7::DeviceType GetDeviceType(const char* type)
{
    return (strcmp(type, "mic") == 0) ?
        VolumeControl7::MIC : VolumeControl7::SPEAKER;
}

void Init(const FunctionCallbackInfo<Value>& args)
{
    if (args.Length() == 1 && args[0]->IsString())
    {
        String::Utf8Value js_logPath(args[0]);
        gwlog_init(*js_logPath, eLO_FILE, eLL_DEBUG);

        LOGD(TAG, "%s(%d) %s: Version(%s)", __CALL_INFO__, VERSION);
    }

    LOGD_CALL(TAG, "start");

    if (g_volumeControl == NULL)
    {
        g_volumeControl = new VolumeControl7();
    }

    int ret = g_volumeControl->Init();
    SetIntToReturnValue(ret, args);

    LOGD(TAG, "%s(%d) %s: end ret(%d)", __CALL_INFO__, ret);
}

void Open(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;

    if (g_volumeControl)
    {
        ret = g_volumeControl->Open();
    }

    SetIntToReturnValue(ret, args);

    LOGD(TAG, "%s(%d) %s: end ret(%d)", __CALL_INFO__, ret);
}

void GetVolume(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int volume = -1;

    // "mic" or "speaker"
    String::Utf8Value js_deviceType(args[0]);

    if (g_volumeControl)
    {
        volume = g_volumeControl->GetVolume(GetDeviceType(*js_deviceType));
    }

    SetIntToReturnValue(volume, args);

    LOGD_CALL(TAG, "end");
}

void SetVolume(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;

    if (g_volumeControl)
    {
        String::Utf8Value js_deviceType(args[0]);
        int volume = args[1]->Int32Value();

        ret = g_volumeControl->SetVolume(
            GetDeviceType(*js_deviceType), volume);
    }

    SetIntToReturnValue(ret, args);

    LOGD_CALL(TAG, "end");
}

/**
 * @return 0(not mute) 1(mute) -1(error)
 */
void GetMute(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int mute = -1;

    if (g_volumeControl)
    {
        String::Utf8Value js_deviceType(args[0]);
        VolumeControl7::DeviceType deviceType = GetDeviceType(*js_deviceType);

        mute = g_volumeControl->GetMute(deviceType);
    }

    SetIntToReturnValue(mute, args);

    LOGD_CALL(TAG, "end");
}

void SetMute(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;

    if (g_volumeControl)
    {
        String::Utf8Value js_deviceType(args[0]);
        VolumeControl7::DeviceType deviceType = GetDeviceType(*js_deviceType);
        bool mute = args[1]->BooleanValue();

        ret = g_volumeControl->SetMute(deviceType, mute);
    }

    SetIntToReturnValue(ret, args);

    LOGD_CALL(TAG, "end");
}

void Close(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;

    if (g_volumeControl)
    {
        ret = g_volumeControl->Close();
    }

    SetIntToReturnValue(ret, args);

    LOGD_CALL(TAG, "end");
}

void Exit(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = 0;

    if (g_volumeControl)
    {
        ret = g_volumeControl->Exit();
        delete g_volumeControl;
        g_volumeControl = NULL;
    }

    SetIntToReturnValue(ret, args);

    LOGD_CALL(TAG, "end");

    gwlog_exit();
}

/**
 * all Node.js Addons must export an initialization function.
 */
void InitNode(Local<Object> exports)
{
    LOGD_CALL(TAG, "start");

    NODE_SET_METHOD(exports, "init", Init);
    NODE_SET_METHOD(exports, "open", Open);
    NODE_SET_METHOD(exports, "getVolume", GetVolume);
    NODE_SET_METHOD(exports, "setVolume", SetVolume);
    NODE_SET_METHOD(exports, "getMute", GetMute);
    NODE_SET_METHOD(exports, "setMute", SetMute);
    NODE_SET_METHOD(exports, "close", Close);
    NODE_SET_METHOD(exports, "exit", Exit);

    LOGD_CALL(TAG, "end");
}

NODE_MODULE(volume, InitNode)
