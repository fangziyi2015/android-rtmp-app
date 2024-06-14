package com.rtmp.androidapp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.rtmp.androidapp.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    var pushHelper: PushHelper? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        pushHelper = PushHelper(this, binding.previewView)
        binding.startLive.setOnClickListener {
            pushHelper?.startLive("")
        }

        binding.stopLive.setOnClickListener {
            pushHelper?.stopLive()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        pushHelper?.release()
    }
}