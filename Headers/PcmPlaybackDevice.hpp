#ifndef VIDEO_CONFERENCE_PCM_PLAYBACK_DEVICE_HPP
#define VIDEO_CONFERENCE_PCM_PLAYBACK_DEVICE_HPP

#include "PcmDevice.hpp"

class PcmPlaybackDevice : public PcmDevice
{
protected:
    /**
    * Method: pcmStreamUniqueThread
    * Task: Virtual method to playback data to audio device
    */
    virtual void pcmStreamUniqueThread();

public:
    /**
     * Method: Constructor
     * Task: Construct PcmPlaybackDevice Object with name and playback type.
     * Args: @param std::string Name - name of the device
     *       @param unsigned int sampleRate - sample rate to capture data
     *       @param int frames - frames to capture on each sample
     */
    explicit PcmPlaybackDevice(std::string deviceName, unsigned int sampleRate, int frames);

    /**
     * Method: Destructor
     * Task: Destruct object and free memory.
     * Note: Virtual to allow for subclassing.
     */
    virtual ~PcmPlaybackDevice() = default;
};


#endif //VIDEO_CONFERENCE_PCM_PLAYBACK_DEVICE_HPP
