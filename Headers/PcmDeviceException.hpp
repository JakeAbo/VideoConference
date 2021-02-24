#ifndef VIDEO_CONFERENCE_PCM_DEVICE_EXCEPTION_HPP
#define VIDEO_CONFERENCE_PCM_DEVICE_EXCEPTION_HPP

#include <string>
#include <exception>
#include <utility>
#include <alsa/asoundlib.h>
#include <iostream>

class PcmDeviceException: public std::exception
{
protected:
    std::string _msg;

    void addErrorCodeToMessage(int errorCode)
    {
        _msg += " due to ";
        _msg += snd_strerror(errorCode);
    }

public:
    /** Method: Constructor PcmDeviceException Object (C Strings)
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     *
     *  @param errorCode error Code given to generate matched string
     */
    explicit PcmDeviceException(const char* message, int errorCode)
        : _msg(message)
    {
        addErrorCodeToMessage(errorCode);
    }

    explicit PcmDeviceException(const char* message)
        : _msg(message)
    {
    }

    /** Method: Constructor PcmDeviceException (C++ STL strings).
     *  @param message The error message.
     *  @param errorCode error Code given to generate matched string
     */
    explicit PcmDeviceException(std::string message, int errorCode)
        : _msg(std::move(message))
    {
        addErrorCodeToMessage(errorCode);
    }

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~PcmDeviceException() noexcept = default;

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in possession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const noexcept
    {
        return _msg.c_str();
    }
};


#endif //VIDEO_CONFERENCESS_PCM_DEVICE_EXCEPTION_HPP
