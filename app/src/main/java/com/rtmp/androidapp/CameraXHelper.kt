package com.rtmp.androidapp

import android.annotation.SuppressLint
import android.util.Log
import android.util.Size
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.Camera
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import java.util.concurrent.Executors

@SuppressLint("RestrictedApi")
class CameraXHelper(
    private val activity: AppCompatActivity,
    private val previewView: PreviewView,
    width: Int,
    height: Int
) {

    private var camera: Camera? = null
    private var preview: Preview? = null
    private var imageAnalysis: ImageAnalysis? = null
    private var isBackCamera = true
    private var cameraProvider: ProcessCameraProvider? = null

    var onPreviewCallback: ((data: ByteArray) -> Unit)? = null
    var onChangeCallback: (() -> Unit)? = null

    companion object {
        private const val TAG = "CameraXHelper"
    }

    init {
        val listenableFuture = ProcessCameraProvider.getInstance(activity)
        listenableFuture.addListener({
            cameraProvider = listenableFuture.get()
            preview = Preview.Builder()
                .build()
                .also {
                    it.setSurfaceProvider(previewView.surfaceProvider)
                }

            imageAnalysis = ImageAnalysis.Builder()
                .setDefaultResolution(Size(width, height))
                .setBackpressureStrategy(STRATEGY_KEEP_ONLY_LATEST)
                .build().also {
                    it.setAnalyzer(
                        Executors.newSingleThreadExecutor()
                    ) { image ->
                        val buffer = image.planes[0].buffer
                        val data = buffer.toByteArray()
                        onPreviewCallback?.invoke(data)
                        image.close()
                    }
                }

            cameraProvider?.let {
                startCamera(false)
            }
        }, ContextCompat.getMainExecutor(activity))
    }

    fun switchCamera(isBackCamera: Boolean) {
        if (isBackCamera != this.isBackCamera) {
            startCamera(isBackCamera)
        }
    }

    private fun startCamera(isBackCamera: Boolean = true) {
        this.isBackCamera = isBackCamera
        // Select back camera as a default
        val cameraSelector = if (isBackCamera) {
            CameraSelector.DEFAULT_BACK_CAMERA
        } else {
            CameraSelector.DEFAULT_FRONT_CAMERA
        }

        try {
            // Unbind use cases before rebinding
            cameraProvider?.unbindAll()
            // Bind use cases to camera
            camera = cameraProvider?.bindToLifecycle(
                activity, cameraSelector, preview, imageAnalysis
            )
            camera?.let {
                onChangeCallback?.invoke()
            }

        } catch (exc: Exception) {
            Log.e(TAG, "Use case binding failed", exc)
        }
    }


}