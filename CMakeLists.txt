cmake_minimum_required(VERSION 3.17)
project(VideoConference)

set(CMAKE_CXX_STANDARD 17)

include_directories(Headers Src)
add_executable(VideoConference Src/main.cpp Src/PcmDevice.cpp Headers/PcmDevice.hpp Headers/PcmDeviceException.hpp
        Src/PcmCaptureDevice.cpp Headers/PcmCaptureDevice.hpp Src/PcmPlaybackDevice.cpp Headers/PcmPlaybackDevice.hpp)

find_package(ALSA REQUIRED)

if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries (VideoConference ${ALSA_LIBRARIES})
endif(ALSA_FOUND)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(VideoConference PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(VideoConference "${CMAKE_THREAD_LIBS_INIT}")
endif()