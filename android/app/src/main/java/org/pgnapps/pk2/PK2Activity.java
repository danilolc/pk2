package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDL;

import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.util.Log;

import java.io.IOException;
import java.lang.Object;
import java.lang.reflect.Method;

public class PK2Activity extends SDLActivity {
    private static final String LOG_TAG = "PK2Activity";

    public void makeFullScreen() {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if(Build.VERSION.SDK_INT < 19){
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        } else {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

    private static Method getEntriesMethod;
    private static Method expansionFileMethod;
    private static Object expansionFile;

    // This method is called by SDL using JNI.
    public static String[] Utils_ScanAPKdir(String dir) throws IOException {
        
        if (expansionFile == null) {
            String mainHint = nativeGetHint("SDL_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION");
            if (mainHint == null) {
                return null; // no expansion use if no main version was set
            }
            String patchHint = nativeGetHint("SDL_ANDROID_APK_EXPANSION_PATCH_FILE_VERSION");
            if (patchHint == null) {
                return null; // no expansion use if no patch version was set
            }

            Integer mainVersion;
            Integer patchVersion;
            try {
                mainVersion = Integer.valueOf(mainHint);
                patchVersion = Integer.valueOf(patchHint);
            } catch (NumberFormatException ex) {
                ex.printStackTrace();
                throw new IOException("No valid file versions set for APK expansion files", ex);
            }

            try {
                // To avoid direct dependency on Google APK expansion library that is
                // not a part of Android SDK we access it using reflection
                expansionFile = Class.forName("com.android.vending.expansion.zipfile.APKExpansionSupport")
                    .getMethod("getAPKExpansionZipFile", Context.class, int.class, int.class)
                    .invoke(null, SDL.getContext(), mainVersion, patchVersion);

                expansionFileMethod = expansionFile.getClass()
                    .getMethod("getInputStream", String.class);
            } catch (Exception ex) {
                ex.printStackTrace();
                expansionFile = null;
                expansionFileMethod = null;
                throw new IOException("Could not access APK expansion support library1", ex);
            }
        }

        if (getEntriesMethod == null) {
            try {
                getEntriesMethod = expansionFile.getClass()
                    .getMethod("getEntriesAt", String.class);
            } catch (Exception ex) {
                getEntriesMethod = null;
                throw new IOException("Could not access APK expansion support library2", ex);
            }
        }


        Object[] list = null;

        try {
            list = expansionFileMethod.invoke(expansionFile, dir);
        } catch (Exception ex) {
            ex.printStackTrace();
            throw new IOException("Could not open stream from APK expansion file3", ex);
        }

        if (list == null) {
            // calling "getEntriesAt" was successful but null was returned
            throw new IOException("Could not find path in APK expansion file4");
        }

        for (Object n : list) {

            //Log.v("PK2", "Found: " + n.mFileName);

        }

        return new String[] {"a", "b"}; //list;

    }

    @Override
    protected String[] getArguments() {
        return new String[0];
        //return new String[] { "dev", "test", "rooster island 1/map001.map" };
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

    //@Override
    //protected void onCreate(Bundle savedInstanceState) {
    //    makeFullScreen();
    //    super.onCreate(savedInstanceState);
    //}

    @Override
    protected void onStart() {
        Utils_ScanAPKdir("sprites/");
        //makeFullScreen();
        super.onStart();
        setWindowStyle(true);
    }

    @Override
    protected void onResume() {
        //makeFullScreen();    
        super.onResume();
        setWindowStyle(true);
    }
}