//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.Manifest;
import android.content.res.AssetManager;
import android.util.Log;
//import android.os.Environment;
//import android.os.Build;
//import android.content.pm.PackageManager;

//import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.util.Locale;

public class PK2Activity extends SDLActivity {

    private static final String TAG = "PJava";

    //@Keep
    private String[] listDir(String dir) {

        String[] ret;
        Log.i(TAG, "Scanning \"" + dir + "\" from APK");
        
        try {

            ret = getAssets().list(dir);

        } catch(IOException e) {

            Log.e(TAG, "Error scaning");
            e.printStackTrace();
            ret = new String[] { "" };

        }

        return ret;

    }

    @Override
    protected String[] getArguments() {

        if (BuildConfig.DEBUG)
            return new String[] {"dev", "fps"};

        return new String[0];

    }

    @Override
    protected String[] getLibraries() {
        return new String[] {
                "hidapi",
                "SDL2",
                "SDL2_image",
                "mpg123",
                "SDL2_mixer",
                "PK2"};
    }

    @Override
    protected void onStart() {
        
        super.onStart();
        setWindowStyle(true);

    }

    @Override
    protected void onResume() {
        
        super.onResume();
        setWindowStyle(true);

    }

}