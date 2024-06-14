#ifndef ANDROID_RTMP_APP_VIDEOCHANNEL_H
#define ANDROID_RTMP_APP_VIDEOCHANNEL_H

#include <pthread.h>
#include <cstring>
#include <x264.h>
#include "util.h"
#include "log4c.h"
#include "rtmp.h"

class VideoChannel {
private:
    typedef void(*PushCallback)(RTMPPacket *packet);

    pthread_mutex_t mutex_t;

    int y_len;
    int uv_len;

    x264_t *videoEncoder;
    x264_picture_t *pic_in;

    PushCallback pushCallback;

public:
    VideoChannel();

    ~VideoChannel();

    void initEncoder(int width, int height, int fps, int bitrate);

    void encodeData(signed char *data);

    void send_sps_pps(uint8_t sps[100], uint8_t pps[100], int sps_len, int pps_len);

    void send_ip_frame(int type, int payload, uint8_t *pPayload);

    void setPushCallback(PushCallback callback);
};


#endif //ANDROID_RTMP_APP_VIDEOCHANNEL_H
