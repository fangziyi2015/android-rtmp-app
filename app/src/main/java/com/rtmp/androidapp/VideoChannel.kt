package com.rtmp.androidapp

import androidx.appcompat.app.AppCompatActivity
import androidx.camera.view.PreviewView

class VideoChannel(
    activity: AppCompatActivity,
    previewView: PreviewView,
    pushHelper: PushHelper,
    fps: Int,
    bitrate: Int
) {

    private var isLive = false
    private val width = 720
    private val height = 1280
    private var cameraXHelper: CameraXHelper? = null

    init {
        cameraXHelper = CameraXHelper(activity, previewView,width,height)
        cameraXHelper?.onPreviewCallback = {
            if (isLive) {
                pushHelper.startVideoPush(it)
            }
        }

        cameraXHelper?.onChangeCallback = {
            pushHelper.initVideoEncoder(width, height, fps, bitrate)
        }
    }

    fun startLive() {
        isLive = true
    }

    fun stopLive() {
        isLive = false
    }
}