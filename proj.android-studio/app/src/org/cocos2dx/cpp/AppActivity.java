/****************************************************************************
Copyright (c) 2015-2017 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import java.util.Calendar;

import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import android.util.Log;
import android.view.Display;
import android.view.Menu;
import android.view.WindowManager;
import android.widget.Toast;

import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    static Context ctx;
    private static Context mContext;
    private static AlarmManager mAlarmManager;
    public String android_id = "";
    //    GoogleCloudMessaging gcm;
    Intent signInIntent;
    Context context;
    String regId;
    private static GoogleApiClient mGoogleApiClient;
    private static final String TAG = Cocos2dxActivity.class.getSimpleName();
    private static Activity sActivity;
    private static final int RC_SIGN_IN = 007;

    // DNGameUtility::checkIsNetworkAvailable()
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d("Notificato","=====================");
        fireNotification();
        Log.d("Notificato","=====================1");

        sActivity = this;


        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestEmail()
                .build();

        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Auth.GOOGLE_SIGN_IN_API, gso)
                .build();
    }



    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        // Checks the orientation of the screen for landscape and portrait
        Display display = ((WindowManager) getSystemService(WINDOW_SERVICE)).getDefaultDisplay();
        int orientation = display.getOrientation();
        switch(orientation) {
            case Configuration.ORIENTATION_PORTRAIT:

                setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                break;
            case Configuration.ORIENTATION_LANDSCAPE:
                setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                break;
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
       // getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }



    public void signin()
    {
        signInIntent = Auth.GoogleSignInApi.getSignInIntent(mGoogleApiClient);
        sActivity.startActivityForResult(signInIntent, RC_SIGN_IN);
    }

    @Override
    protected void onStart() {
        super.onStart();

        //startActivityForResult(signInIntent, RC_SIGN_IN);
    }

    public static boolean checkIsNetworkAvailable() {
        ConnectivityManager connectivityManager = (ConnectivityManager) Cocos2dxActivity.
                getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        //System.out.println("in side Connectivity.....");
        NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
        //System.out.println("in side Network info.....");
        if (networkInfo != null && networkInfo.isConnected()) {
            //System.out.println("internet.....");

            return true;
        } else

            //System.out.println(" no   internet.....");
            return false;
    }



//    private void handleSignInResult(GoogleSignInResult result) {
//        Log.d(TAG, "handleSignInResult:" + result.isSuccess());
//        if (result.isSuccess()) {
//            // Signed in successfolly, show authenticated UI.
//            GoogleSignInAccount acct = result.getSignInAccount();
//          Log.e("Account========", String.valueOf(acct));
//
//        } else {
//            // Signed out, show unauthenticated UI.
//            Log.d("Account========", "not accessed");
//        }
//    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        // Result returned from launching the Intent from GoogleSignInApi.getSignInIntent(...);
        if (requestCode == RC_SIGN_IN) {
            GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
          //  handleSignInResult(result);
        }
    }



public void fireNotification()
{
    Calendar calender = Calendar.getInstance();

    calender.set(Calendar.HOUR_OF_DAY, 24);
    calender.set(Calendar.MINUTE,0);
    calender.set(Calendar.SECOND,5);

    Intent intent = new Intent(getApplicationContext(), Notification_receiver.class);
    PendingIntent pendingIntent = PendingIntent.getBroadcast(getApplicationContext(), 100, intent, PendingIntent.FLAG_UPDATE_CURRENT);
    AlarmManager alarmManager = (AlarmManager)getSystemService(ALARM_SERVICE);
    alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, calender.getTimeInMillis(), AlarmManager.INTERVAL_DAY, pendingIntent);

}

    @Override
    public void onConnected(@Nullable Bundle bundle) {

    }

    @Override
    public void onConnectionSuspended(int i) {

    }

    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {

    }


    public void IAPPurchased()
    {

        Log.d("Previously purchase","IAP+++++++++++++");
       // Toast.makeText(AppActivity.this,"Previously purchased, please click on Restore button.", Toast.LENGTH_LONG).show();
        sActivity.runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(sActivity, "Previously purchased, please click on Restore button.", Toast.LENGTH_LONG).show();
            }
        });
    }
}