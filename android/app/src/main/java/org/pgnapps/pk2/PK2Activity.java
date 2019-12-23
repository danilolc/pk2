package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.content.pm.ActivityInfo;
import android.os.Build;
import android.view.View;
import android.os.Bundle;
import android.view.WindowManager;

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

    @Override
    protected String[] getArguments() {
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

    //@Override
    //protected void onCreate(Bundle savedInstanceState) {
    //    makeFullScreen();
    //    super.onCreate(savedInstanceState);
    //}

    @Override
    protected void onStart() {
        makeFullScreen();
        super.onStart();
    }

    @Override
    protected void onResume() {
        makeFullScreen();
        super.onResume();
    }
}