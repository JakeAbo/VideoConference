#include "PcmDevice.hpp"

PcmDevice::PcmDevice(std::string deviceName, snd_pcm_stream_t type, unsigned int sampleRate, int frames)
    : _deviceName(std::move(deviceName)), _deviceType(type), _deviceSampleRate(sampleRate), _deviceFrames(frames),
      _deviceHandle(nullptr), _deviceParams(nullptr), _deviceDir(-1)
{}

PcmDevice::~PcmDevice()
{
    /* Closing device if opened before */
    if(_deviceHandle != nullptr)
    {
        snd_pcm_drain(_deviceHandle);
        snd_pcm_close(_deviceHandle);
    }
}

void PcmDevice::openDevice() noexcept(false)
{
    int rc;

    /* Open PCM device for recording (capture) or streaming (playback). */
    rc = snd_pcm_open(&_deviceHandle, _deviceName.c_str(), _deviceType, 0);
    if (rc < 0)
    {
        throw PcmDeviceException("Unable to open pcm device", rc);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&_deviceParams);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(_deviceHandle, _deviceParams);

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(_deviceHandle, _deviceParams, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(_deviceHandle, _deviceParams, SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(_deviceHandle, _deviceParams, 2);

    /* [_deviceSampleRate] bits/second sampling rate (CD quality) */
    snd_pcm_hw_params_set_rate_near(_deviceHandle, _deviceParams, &_deviceSampleRate, &_deviceDir);

    /* Set period size to [_deviceFrames] frames. */
    snd_pcm_hw_params_set_period_size_near(_deviceHandle, _deviceParams, &_deviceFrames, &_deviceDir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(_deviceHandle, _deviceParams);
    if (rc < 0)
    {
        throw PcmDeviceException("Unable to set hw parameters", rc);
    }
}

void PcmDevice::stream()
{
    _keepRunning = true;
    std::thread runThread = std::thread(&PcmDevice::pcmStreamUniqueThread, this);
    runThread.detach();
}