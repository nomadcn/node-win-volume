#pragma once
#ifndef VOLUME_CONTROL_7_H
#define VOLUME_CONTROL_7_H

#include "volume.h"
#include <Windows.h>
#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <State.h>

/**
 * Adjusts volume level of default speaker and mic with the Core Audio API.
 * It runs on Windows Vista or later.
 * All methods are not thread-safe.
 */
class VolumeControl7
{
public:
    enum DeviceType { MIC, SPEAKER };

public:
	VolumeControl7(void);
	virtual ~VolumeControl7(void);

    int Init();
	int Open();
	int GetVolume(DeviceType deviceType);
	int SetVolume(DeviceType deviceType, const int value);
    int GetMute(DeviceType deviceType);
    int SetMute(DeviceType deviceType, bool mute);
	int Close();
    int Exit();

private:
    EDataFlow GetDataFlow(DeviceType type) const
    {
        return (type == MIC) ? eCapture : eRender;
    }
    IMMDeviceEnumerator* CreateDeviceEnumerator();
    IAudioEndpointVolume* GetEndpointVolume(IMMDevice* device);
    IAudioEndpointVolume* GetEndpointVolume(
        DeviceType deviceType,
        IMMDeviceEnumerator*& deviceEnumerator,
        IMMDevice*& device);

    float ConvertLevelFromIntToFloat(const int level) const;
    int ConvertLevelFromFloatToInt(const float level) const;

private:
    gw::State m_state;
    IMMDeviceEnumerator* m_deviceEnumerator;
};

#endif
