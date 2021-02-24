#include "PcmCaptureDevice.hpp"
#include "PcmPlaybackDevice.hpp"

int methodReceiveFromPcm1AndOutputToPcm2()
{
    long loops;
    int rc;
    int size;
    snd_pcm_t *handlePlayback, *handleCapture;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;
    char *buffer;

    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handleCapture, "default",
                      SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handlePlayback, "default",
                      SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handlePlayback, params);
    snd_pcm_hw_params_any(handleCapture, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handlePlayback, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_access(handleCapture, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handlePlayback, params,
                                 SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_format(handleCapture, params,
                                 SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(handlePlayback, params, 2);
    snd_pcm_hw_params_set_channels(handleCapture, params, 2);

    /* 44100 bits/second sampling rate (CD quality) */
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handlePlayback, params,
                                    &val, &dir);
    snd_pcm_hw_params_set_rate_near(handleCapture, params,
                                    &val, &dir);

    /* Set period size to 32 frames. */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handlePlayback,
                                           params, &frames, &dir);
    snd_pcm_hw_params_set_period_size_near(handleCapture,
                                           params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handlePlayback, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    rc = snd_pcm_hw_params(handleCapture, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Until here */

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params, &frames,
                                      &dir);
    size = frames * 4; /* 2 bytes/sample, 2 channels */
    buffer = (char *) malloc(size);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params,
                                      &val, &dir);
    /* 5 seconds in microseconds divided by
     * period time */
    loops = 60000000 / val;

    while (loops > 0) {
        loops--;
        rc = snd_pcm_readi(handleCapture, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handleCapture);
        } else if (rc < 0) {
            fprintf(stderr,
                    "error from read: %s\n",
                    snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }

        rc = snd_pcm_writei(handlePlayback, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means underrun */
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handlePlayback);
        } else if (rc < 0) {
            fprintf(stderr,
                    "error from writei: %s\n",
                    snd_strerror(rc));
        }  else if (rc != (int)frames) {
            fprintf(stderr,
                    "short write, write %d frames\n", rc);
        }
    }

    snd_pcm_drain(handlePlayback);
    snd_pcm_close(handlePlayback);

    snd_pcm_drain(handleCapture);
    snd_pcm_close(handleCapture);
    free(buffer);

    return 0;
}

int main()
{
    PcmCaptureDevice d("default", 44100, 32);
    d.openDevice();
    d.stream();

    PcmPlaybackDevice e("default", 44100, 32);
    e.openDevice();
    e.stream();

    while (true);

    return 0;
}