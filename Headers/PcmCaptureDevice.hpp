#ifndef VIDEO_CONFERENCE_PCM_CAPTURE_DEVICE_HPP
#define VIDEO_CONFERENCE_PCM_CAPTURE_DEVICE_HPP

#include "PcmDevice.hpp"

class PcmCaptureDevice : public PcmDevice
{
protected:
    /**
    * Method: pcmStreamUniqueThread
    * Task: Virtual method to capture data from audio device
    */
    virtual void pcmStreamUniqueThread();

public:
    /**
     * Method: Constructor
     * Task: Construct PcmCaptureDevice Object with name and capture type.
     * Args: @param std::string Name - name of the device
     *       @param unsigned int sampleRate - sample rate to capture data
     *       @param int frames - frames to capture on each sample
     */
    explicit PcmCaptureDevice(std::string deviceName, unsigned int sampleRate, int frames);

    /**
     * Method: Destructor
     * Task: Destruct object and free memory.
     * Note: Virtual to allow for subclassing.
     */
    virtual ~PcmCaptureDevice() = default;
};


#endif //VIDEO_CONFERENCE_PCM_CAPTURE_DEVICE_HPP
