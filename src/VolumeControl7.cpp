#include "VolumeControl7.h"
#include <FunctionDiscoveryKeys.h>
#include <gwlog.h>
#include <math.h>

#define CHECK_STATE(state) \
    if (m_state != state) \
    { \
        LOGE(TAG, "%s(%d) %s: Wrong state: cur(%s) != expected(%s)", \
            __CALL_INFO__, \
            m_state.GetStateName(), gw::State::GetStateName(state)); \
        return -1; \
    }

/**
 * Release resource safely.
 */
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

VolumeControl7::VolumeControl7(void) :
m_state(gw::eLCS_NONE)
,m_deviceEnumerator(NULL)
{
}

VolumeControl7::~VolumeControl7(void)
{
}

int VolumeControl7::Init()
{
    CHECK_STATE(gw::eLCS_NONE)

    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (SUCCEEDED(hr) || hr == RPC_E_CHANGED_MODE)
    {
        m_state.SetState(gw::eLCS_INIT);
        return 0;
    }

    return -2;
}

int VolumeControl7::Open()
{
    CHECK_STATE(gw::eLCS_INIT)

    m_deviceEnumerator = CreateDeviceEnumerator();
    if (m_deviceEnumerator == NULL)
    {
        return NULL;
    }

    m_state.SetState(gw::eLCS_OPEN);

    return 0;
}

/**
 * @param deviceType: MIC or SPEAKER
 */
int VolumeControl7::GetVolume(DeviceType deviceType)
{
    CHECK_STATE(gw::eLCS_OPEN)

    // level is the volume level in the range from 0 to 100.
    int volume = -1;
    IMMDevice* device = NULL;

    IAudioEndpointVolume* endpointVolume = GetEndpointVolume(
        deviceType, m_deviceEnumerator, device);

    if (endpointVolume)
    {
        float level;
        // Volume level is in the range from 0.0 to 1.0.
        HRESULT hr = endpointVolume->GetMasterVolumeLevelScalar(&level);
        if (SUCCEEDED(hr))
        {
            volume = ConvertLevelFromFloatToInt(level);
        }
        else
        {
            LOGE(TAG, "%s(%d) %s: GetMasterVolumeLevelScalar() is failed. hr(%X)", __CALL_INFO__, hr);
        }
    }

    SafeRelease(&device);

    return volume;
}

/**
 * Caller is responsible to release com object created here.
 * (deviceEnumerator, device).
 */
IAudioEndpointVolume* VolumeControl7::GetEndpointVolume(
    DeviceType deviceType,
    IMMDeviceEnumerator*& deviceEnumerator,
    IMMDevice*& device)
{
    HRESULT hr;

    if (deviceEnumerator == NULL)
    {
        LOGE_CALL(TAG, "deviceEnumerator is null.");
        return NULL;
    }

    const EDataFlow dataFlow = GetDataFlow(deviceType);
    const ERole role = (dataFlow == eRender) ? eMultimedia : eCommunications;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(dataFlow, role, &device);
    if (FAILED(hr))
    {
        LOGE(TAG, "%s(%d) %s: deviceEnumerator->GetDefaultAudioEndpoint() is failed. hr(%X) deviceType(%d) device(%p)",
            __CALL_INFO__, hr, deviceType, device);
        return NULL;
    }

    return GetEndpointVolume(device);
}

IMMDeviceEnumerator* VolumeControl7::CreateDeviceEnumerator()
{
    IMMDeviceEnumerator *deviceEnumerator = NULL;

    //We initialize the device enumerator here
    HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr))
    {
        LOGE(TAG, "%s(%d) %s: Unable to instantiate device enumerator: hr(%X)",
            __CALL_INFO__, hr);
    }

    return deviceEnumerator;
}

IAudioEndpointVolume* VolumeControl7::GetEndpointVolume(IMMDevice* device)
{
    HRESULT hr; 
    //This is the Core Audio interface of interest
    IAudioEndpointVolume *endpointVolume = NULL;

    // argument sanity check
    if (device == NULL)
    {
        LOGE_CALL(TAG, "device is null.");
        return NULL;
    }

    //We activate it here
    hr = device->Activate(
        __uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, 
        reinterpret_cast<void **>(&endpointVolume));
    if (FAILED(hr))
    {
        LOGE(TAG, "%s(%d) %s: device->Activate() error. hr(%X)",
            __CALL_INFO__, hr);
        return NULL;
    }

    return endpointVolume;
}

/**
 * Exported method to adjust volume level for both mic or speaker.
 */
int VolumeControl7::SetVolume(DeviceType deviceType, const int volume)
{
    CHECK_STATE(gw::eLCS_OPEN)

    LOGD(TAG, "%s(%d) %s: start type(%d) volume(%d)",
        __CALL_INFO__, deviceType, volume);

    // ret is the volume level in the range from 0 to 100.
    int ret = -1;
    IMMDevice* device = NULL;

    IAudioEndpointVolume* endpointVolume = GetEndpointVolume(
        deviceType, m_deviceEnumerator, device);

    if (endpointVolume)
    {
        float level = ConvertLevelFromIntToFloat(volume);
        // Volume level is in the range from 0.0 to 1.0.
        HRESULT hr = endpointVolume->SetMasterVolumeLevelScalar(level, NULL);
        if (SUCCEEDED(hr))
        {
            ret = 0;
        }
        else
        {
            LOGE(TAG, "%s(%d) %s: SetMasterVolumeLevelScalar() is failed. hr(%X)", __CALL_INFO__, hr);
        }
    }

    SafeRelease(&device);

    LOGD(TAG, "%s(%d) %s: end ret(%d)", __CALL_INFO__, ret);

    return ret;
}

/**
 * @param level: volume level ranges from 0 to 100.
 */
float VolumeControl7::ConvertLevelFromIntToFloat(const int level) const
{
    if (level < 0 || level > 100)
    {
        return -1.0f;
    }

    return (float)level / 100.0f;
}

int VolumeControl7::ConvertLevelFromFloatToInt(const float level) const
{
    return (int)round(level * 100.0f);
}

int VolumeControl7::GetMute(DeviceType deviceType)
{
    CHECK_STATE(gw::eLCS_OPEN)

    LOGD(TAG, "%s(%d) %s: start type(%d)", __CALL_INFO__, deviceType);

    // 0(false), 1(true), -(error)
    int mute = -1;
    IMMDevice* device = NULL;

    IAudioEndpointVolume* endpointVolume = GetEndpointVolume(
        deviceType, m_deviceEnumerator, device);

    if (endpointVolume)
    {
        BOOL value;
        HRESULT hr = endpointVolume->GetMute(&value);
        if (SUCCEEDED(hr))
        {
            mute = value;
        }
        else
        {
            LOGE(TAG, "%s(%d) %s: SetMute() is failed. hr(%X)",
                __CALL_INFO__, hr);
        }
    }
    else
    {
        LOGI_CALL(TAG, "endpointVolume is null.");
    }

    SafeRelease(&device);

    LOGD(TAG, "%s(%d) %s: end mute(%d)", __CALL_INFO__, mute);

    return mute;
}

int VolumeControl7::SetMute(DeviceType deviceType, bool mute)
{
    CHECK_STATE(gw::eLCS_OPEN)

    LOGD(TAG, "%s(%d) %s: start type(%d) mute(%d)",
        __CALL_INFO__, deviceType, mute);

    int ret = -1;
    IMMDevice* device = NULL;

    IAudioEndpointVolume* endpointVolume = GetEndpointVolume(
        deviceType, m_deviceEnumerator, device);

    if (endpointVolume)
    {
        HRESULT hr = endpointVolume->SetMute(mute ? TRUE : FALSE, NULL);
        if (SUCCEEDED(hr))
        {
            ret = 0;
        }
        else
        {
            LOGE(TAG, "%s(%d) %s: SetMute() is failed. hr(%X)",
                __CALL_INFO__, hr);
        }
    }

    SafeRelease(&device);

    LOGD(TAG, "%s(%d) %s: end ret(%d)", __CALL_INFO__, ret);

    return ret;
}

int VolumeControl7::Close()
{
    CHECK_STATE(gw::eLCS_OPEN)

    LOGD_CALL(TAG, "start");

    SafeRelease(&m_deviceEnumerator);
    m_state.SetState(gw::eLCS_INIT);

    LOGD_CALL(TAG, "end");

    return 0;
}

int VolumeControl7::Exit()
{
    CHECK_STATE(gw::eLCS_INIT)

    LOGD_CALL(TAG, "start");

    CoUninitialize();
    m_state.SetState(gw::eLCS_NONE);

    LOGD_CALL(TAG, "end");

    return 0;
}
