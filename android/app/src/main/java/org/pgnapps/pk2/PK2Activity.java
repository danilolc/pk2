//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.os.Build;
import android.util.Log;

import java.io.IOException;

public class PK2Activity extends SDLActivity {

    private static final String TAG = "PJava";

    private String loadZip() {

        String file = "";

        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        try{
            startActivityForResult(intent, 2);
//            file = intent.getDataString();
        } catch (ActivityNotFoundException e) {
            Log.i(TAG, "No file explorer clients installed.");
        }

        Log.i(TAG, "batatada");
        Log.i(TAG, file);

        return "batata";
    }

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
                "c++_shared",
                "zip",
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