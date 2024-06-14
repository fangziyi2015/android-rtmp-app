package com.rtmp.androidapp

import androidx.appcompat.app.AppCompatActivity
import androidx.camera.view.PreviewView

class PushHelper(activity: AppCompatActivity, previewView: PreviewView) {

    companion object {
        init {
            System.loadLibrary("androidapp")
        }
    }

    private var videoChannel: VideoChannel? = null

    init {
        initNative()
        videoChannel = VideoChannel(activity, previewView, this, 25, 8000000)
    }

    fun startLive(url: String) {
        startLiveNative(url)
        videoChannel?.startLive()
    }

    fun stopLive() {
        stopLiveNative()
        videoChannel?.stopLive()
    }

    fun release() {
        releaseNative()
    }

    fun initVideoEncoder(width: Int, height: Int, fps: Int, bitrate: Int) {
        initVideoEncoderNative(width, height, fps, bitrate)
    }

    fun startVideoPush(data: ByteArray) {
        startPushVideoNative(data)
    }

    private external fun initNative()
    private external fun startLiveNative(url: String)
    private external fun stopLiveNative()
    private external fun releaseNative()
    private external fun initVideoEncoderNative(width: Int, height: Int, fps: Int, bitrate: Int)
    private external fun startPushVideoNative(data: ByteArray)

}