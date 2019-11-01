#include <alsa/asoundlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "file_loader.h"

float pi = 3.1415926;

void init_buf(void)
{
    int i;
    for(i=0; i<100; i++)
    {
//        buf[i*2] = 30000 * sin(2*pi*i/100);
 //       buf[i*2 + 1] = 0;
    }

 //   printf("%d %d %d %d", buf[0], buf[1], buf[2], buf[3]);
    return;
}


int main(void)
{
    int res;
    int i;
    int file_size;
    short *buf;
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *hw_params;

    res = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(res != 0)
    {
        printf("playback open error\n");
        return;
    }

    snd_pcm_hw_params_malloc(&hw_params);
    snd_pcm_hw_params_any(pcm_handle, hw_params);

    res = snd_pcm_hw_params_test_access(pcm_handle, hw_params,
            SND_PCM_ACCESS_RW_INTERLEAVED);
    printf("test access: %d\n", res);

    res = snd_pcm_hw_params_set_access(pcm_handle, hw_params,
            SND_PCM_ACCESS_RW_INTERLEAVED);
    if(res != 0)
    {
        printf("set access error\n");
        return;
    }

    res = snd_pcm_hw_params_set_format(pcm_handle, hw_params,
            SND_PCM_FORMAT_S16_LE);
    if(res != 0)
    {
        printf("set format error\n");
        return;
    }

    res = snd_pcm_hw_params_set_rate(pcm_handle, hw_params, 44100, 0);
    if(res != 0)
    {
        printf("set rate error\n");
        return;
    }

    res = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, 2);
    if(res != 0)
    {
        printf("set channels error\n");
        return;
    }

    res = snd_pcm_hw_params(pcm_handle, hw_params);
    if(res != 0)
    {
        printf("set hw params error\n");
        return;
    }

    file_size = load_file("geisha.pcm", &buf);
    printf("file size: %d\n", file_size);

    for(i=0; i<file_size/4/100; i++)
    {
        res = snd_pcm_writei(pcm_handle, buf, 100);
        printf("write %d, %d\n", i, res);
        buf += 100 * 2;
    }

    return 0;
}
