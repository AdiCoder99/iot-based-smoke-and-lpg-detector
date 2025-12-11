package com.company.smokealertapp

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.drawable.AnimationDrawable
import android.graphics.drawable.ColorDrawable
import android.os.Build
import android.os.Bundle
import android.view.animation.Animation
import android.view.animation.AnimationUtils
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.lifecycle.lifecycleScope
import com.company.smokealertapp.databinding.ActivityMainBinding
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val requestPermissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestPermission()
    ) { isGranted: Boolean ->
        if (isGranted) {
            startSmokeDetectorService()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        requestPermissions()
        observeSmokeDetectorStatus()
    }

    private fun requestPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.POST_NOTIFICATIONS) !=
                PackageManager.PERMISSION_GRANTED
            ) {
                requestPermissionLauncher.launch(Manifest.permission.POST_NOTIFICATIONS)
            } else {
                startSmokeDetectorService()
            }
        } else {
            startSmokeDetectorService()
        }
    }

    private fun startSmokeDetectorService() {
        val intent = Intent(this, SmokeDetectorService::class.java)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForegroundService(intent)
        } else {
            startService(intent)
        }
    }

    private fun observeSmokeDetectorStatus() {
        lifecycleScope.launch {
            SmokeDetectorStatus.isSmokeDetected.combine(SmokeDetectorStatus.lastUpdated) { isDetected, timestamp ->
                Pair(isDetected, timestamp)
            }.collectLatest { (isDetected, timestamp) ->
                updateUi(isDetected, timestamp)
            }
        }
    }

    private fun updateUi(isSmokeDetected: Boolean, timestamp: Long) {
        if (isSmokeDetected) {
            binding.rootLayout.setBackgroundResource(R.drawable.animated_warning_background)
            val backgroundAnimation = binding.rootLayout.background as AnimationDrawable
            backgroundAnimation.start()

            binding.statusIcon.setImageResource(R.drawable.ic_status_warning)
            binding.statusText.text = "Smoke Detected!"
            binding.subtitleText.text = "High levels of smoke detected. Please check immediately."
        } else {
            binding.rootLayout.background = ColorDrawable(ContextCompat.getColor(this, R.color.colorSafeBackground))
            binding.statusIcon.setImageResource(R.drawable.ic_status_safe)
            binding.statusText.text = "All Clear"
            binding.subtitleText.text = "Your smoke detector is online and monitoring."
        }

        if (timestamp > 0) {
            val sdf = SimpleDateFormat("hh:mm:ss a, MMM dd", Locale.getDefault())
            binding.lastUpdatedText.text = "Last update: ${sdf.format(Date(timestamp))}"
        } else {
            binding.lastUpdatedText.text = "Last updated: --"
        }
    }
}
