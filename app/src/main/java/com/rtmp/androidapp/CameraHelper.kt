package com.rtmp.androidapp

import android.app.Activity
import android.hardware.Camera
import android.view.Surface
import android.view.SurfaceHolder

class CameraHelper : SurfaceHolder.Callback, Camera.PreviewCallback {

    var previewCallback: ((data: ByteArray, camera: Camera) -> Unit)? = null
    var changeSizeCallback: (() -> Unit)? = null

    fun setCameraDisplayOrientation(activity: Activity, cameraId: Int, camera: Camera) {
        val info = Camera.CameraInfo()
        Camera.getCameraInfo(cameraId, info)
        val rotation = activity.windowManager.defaultDisplay.rotation
        var degrees = 0
        when (rotation) {
            Surface.ROTATION_0 -> degrees = 0
            Surface.ROTATION_90 -> degrees = 90
            Surface.ROTATION_180 -> degrees = 180
            Surface.ROTATION_270 -> degrees = 270
        }
        var result = 0
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360
            result = (360 - result) % 360
        } else {
            result = (info.orientation - degrees + 360) % 360
        }
        camera.setDisplayOrientation(result)
    }

    private fun setCameraParameters(){

    }

    private fun startPreview() {
    }

    private fun stopPreview() {
    }


    override fun surfaceCreated(holder: SurfaceHolder) {

    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        stopPreview()
        startPreview()
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        stopPreview()
    }

    override fun onPreviewFrame(data: ByteArray?, camera: Camera?) {
        if (data != null && camera != null) {
            previewCallback?.invoke(data, camera)
        }

    }


}