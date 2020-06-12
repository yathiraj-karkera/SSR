package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import android.util.Log;
import androidx.fragment.app.FragmentActivity;

import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;

/**
 * Created by Alok on 9/21/17.
 */

public class GoogleSignInClass extends FragmentActivity implements GoogleApiClient.OnConnectionFailedListener {

    private static GoogleApiClient mGoogleApiClient;
    private static final int RC_SIGN_IN = 007;
    private static Activity sActivity;
    static Context ctx;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ctx= this;
        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestEmail()
                .build();



    }


public static void signin()
{
   // Intent signInIntent = Auth.GoogleSignInApi.getSignInIntent(mGoogleApiClient);
    //startActivityForResult(signInIntent, RC_SIGN_IN);
    Log.d("Google is connected", "=============");
    mGoogleApiClient.connect();
    Log.d("Google is connected", "=============1111");
}


    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        // Result returned from launching the Intent from GoogleSignInApi.getSignInIntent(...);
        if (requestCode == RC_SIGN_IN) {
            GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
            //  handleSignInResult(result);
        }
    }



    @Override
    public void onConnectionFailed(@NonNull ConnectionResult connectionResult) {

    }
}
