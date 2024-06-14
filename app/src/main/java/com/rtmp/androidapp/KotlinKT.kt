package com.rtmp.androidapp

import java.lang.IllegalArgumentException
import java.nio.ByteBuffer

fun ByteBuffer.toByteArray(): ByteArray {
    if (!hasRemaining()) {
        throw IllegalArgumentException("is not!!")
    }

    return if (hasArray()) {
        return array()
    } else {
        val buffer = ByteArray(remaining())
        get(buffer)
        buffer
    }
}