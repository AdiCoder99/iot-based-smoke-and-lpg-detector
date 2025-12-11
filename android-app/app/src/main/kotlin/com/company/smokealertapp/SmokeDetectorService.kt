package com.company.smokealertapp

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.IBinder
import android.util.Log
import androidx.core.app.NotificationCompat
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener

class SmokeDetectorService : Service() {

    private val database by lazy { FirebaseDatabase.getInstance() }
    private val sensorRef by lazy { database.getReference("mq2_readings") }

    private val listener = object : ValueEventListener {
        override fun onDataChange(snapshot: DataSnapshot) {
            Log.d(TAG, "Service onDataChange: ${snapshot.value}")
            val gasDigital = snapshot.child("gasDigital").getValue(Long::class.java)
            val timestamp = snapshot.child("timestamp").getValue(Long::class.java) ?: System.currentTimeMillis()
            val isSmokeDetected = gasDigital == 1L
            SmokeDetectorStatus.setStatus(isSmokeDetected, timestamp)
            if (isSmokeDetected) {
                sendSmokeNotification()
            }
        }

        override fun onCancelled(error: DatabaseError) {
            Log.e(TAG, "Database error in service: ", error.toException())
        }
    }

    override fun onCreate() {
        super.onCreate()
        startForeground(SERVICE_ID, createForegroundNotification())
        sensorRef.addValueEventListener(listener)
        Log.d(TAG, "SmokeDetectorService created and listener attached.")
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return START_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()
        sensorRef.removeEventListener(listener)
        SmokeDetectorStatus.setStatus(false, 0)
        Log.d(TAG, "SmokeDetectorService destroyed and listener removed.")
    }

    private fun sendSmokeNotification() {
        val notificationManager = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        val channelId = "smoke_alert_channel"

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(channelId, "Smoke Alerts", NotificationManager.IMPORTANCE_HIGH)
            notificationManager.createNotificationChannel(channel)
        }

        val notification = NotificationCompat.Builder(this, channelId)
            .setSmallIcon(R.drawable.ic_status_warning)
            .setContentTitle("SMOKE DETECTED!")
            .setContentText("High levels of smoke detected. Please check immediately.")
            .setPriority(NotificationCompat.PRIORITY_HIGH)
            .build()

        notificationManager.notify(SMOKE_NOTIFICATION_ID, notification)
        Log.d(TAG, "Smoke notification sent.")
    }

    private fun createForegroundNotification(): Notification {
        val channelId = "foreground_service_channel"
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(channelId, "App Status", NotificationManager.IMPORTANCE_LOW)
            getSystemService(NotificationManager::class.java).createNotificationChannel(channel)
        }

        return NotificationCompat.Builder(this, channelId)
            .setContentTitle("Smoke Detector Active")
            .setContentText("Monitoring your smoke sensor in the background.")
            .setSmallIcon(R.drawable.ic_status_safe)
            .build()
    }

    override fun onBind(intent: Intent?): IBinder? = null

    companion object {
        private const val TAG = "SmokeDetectorService"
        private const val SERVICE_ID = 1
        private const val SMOKE_NOTIFICATION_ID = 2
    }
}