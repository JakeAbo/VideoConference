#include "PcmCaptureDevice.hpp"

PcmCaptureDevice::PcmCaptureDevice(std::string deviceName, unsigned int sampleRate, int frames)
    : PcmDevice(std::move(deviceName), SND_PCM_STREAM_CAPTURE, sampleRate, frames)
{

}

void PcmCaptureDevice::pcmStreamUniqueThread()
{
    int rc;
    char* buffer;
    unsigned int size;

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(_deviceParams, &_deviceFrames, &_deviceDir);

    /* 2 bytes/sample, 2 channels */
    size = _deviceFrames * 4;
    buffer = (char *) malloc(size * sizeof(char));

    /* We want to loop for [timeout] seconds */
    /* If timeout equals 0 loop infinitely  */
    snd_pcm_hw_params_get_period_time(_deviceParams, &_deviceSampleRate, &_deviceDir);

    while(_keepRunning)
    {
        rc = snd_pcm_readi(_deviceHandle, buffer, _deviceFrames);
        if (rc == -EPIPE)
        {
            /* EPIPE means overrun */
            /* throw PcmDeviceException("overrun occurred", rc); */
            snd_pcm_prepare(_deviceHandle);
        }

        else if (rc < 0)
        {
            /* throw PcmDeviceException("error from read", rc); */
        }

        else if (rc != (int)_deviceFrames)
        {
            /* throw PcmDeviceException("short read, read: ", rc); */
        }

        /* Process data */
        std::cout << buffer << std::endl;
    }
}
