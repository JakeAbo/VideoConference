#ifndef VIDEO_CONFERENCE_PCM_DEVICE_HPP
#define VIDEO_CONFERENCE_PCM_DEVICE_HPP

#include <atomic>
#include <ctime>
#include <thread>

#include "PcmDeviceException.hpp"

class PcmDevice
{
protected:
    std::string _deviceName;
    snd_pcm_t *_deviceHandle;
    snd_pcm_stream_t _deviceType;
    snd_pcm_hw_params_t *_deviceParams;
    unsigned int _deviceSampleRate;
    snd_pcm_uframes_t _deviceFrames;
    int _deviceDir;

    std::atomic_bool _keepRunning = ATOMIC_VAR_INIT(true);

protected:
    /**
     * Method: pcmStreamUniqueThread
     * Task: Pure-virtual method which be implemented on PcmCaptureDevice and PcmPlaybackDevice to read and write data
     *       from device
     */
    virtual void pcmStreamUniqueThread() = 0;

    /**
     * Method: Constructor
     * Task: Construct PcmDevice Object with name and type.
     * Args: @param std::string Name - name of the device
     *       @param snd_pcm_stream_t type - type of the device (SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK)
     */
    explicit PcmDevice(std::string deviceName, snd_pcm_stream_t type, unsigned int sampleRate, int frames);

    /**
     * Method: Destructor
     * Task: Destruct object and free memory.
     * Note: Virtual to allow for subclassing.
     */
    virtual ~PcmDevice();

public:
    /**
     * Method: openDevice
     * Task: Open the pcm device for usage
     * noexcept - method should not throw an exception, while noexcept(false) - method can throw an exception
     */
    void openDevice() noexcept(false);

    /**
     * Method: stream
     * Task: stream method which run a thread to read or write to or from the pcm device.
     * Note: If didn't set timeout run infinitely
     *       Method use pure-virtual method to read/write data to the pcm device.
     */
    void stream();
};


#endif //VIDEO_CONFERENCE_PCM_DEVICE_HPP
