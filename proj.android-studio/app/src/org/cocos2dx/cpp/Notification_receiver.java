package org.cocos2dx.cpp;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import androidx.core.app.NotificationCompat;

import com.ghostchillies.superswitchrun.R;

/**
 * Created by Alok on 9/19/17.
 */

public class Notification_receiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
        Intent repeatingIntent = new Intent(context, Repeating_activity.class);
        repeatingIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(context, 100, repeatingIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(context)
                .setContentIntent(pendingIntent)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentText("We have missed your craziness.")
                .setAutoCancel(true);

       // Intent inte

        PendingIntent contentIntent = PendingIntent.getActivity(context, 0,
                new Intent(context, AppActivity.class), PendingIntent.FLAG_UPDATE_CURRENT);

        builder.setContentIntent(contentIntent);
        notificationManager.notify(100,builder.build());
    }
}
