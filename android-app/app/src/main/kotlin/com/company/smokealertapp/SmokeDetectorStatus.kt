package com.company.smokealertapp

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow

object SmokeDetectorStatus {
    private val _isSmokeDetected = MutableStateFlow(false)
    val isSmokeDetected = _isSmokeDetected.asStateFlow()

    private val _lastUpdated = MutableStateFlow<Long>(0)
    val lastUpdated = _lastUpdated.asStateFlow()

    fun setStatus(isDetected: Boolean, timestamp: Long) {
        _isSmokeDetected.value = isDetected
        _lastUpdated.value = timestamp
    }
}
