
#include <PcmPlaybackDevice.hpp>

#include "PcmPlaybackDevice.hpp"

PcmPlaybackDevice::PcmPlaybackDevice(std::string deviceName, unsigned int sampleRate, int frames)
    : PcmDevice(std::move(deviceName), SND_PCM_STREAM_PLAYBACK, sampleRate, frames)
{

}

void PcmPlaybackDevice::pcmStreamUniqueThread()
{
    while(_keepRunning)
        std::cout << "PcmPlaybackDevice::pcmStreamUniqueThread" << std::endl;
}

