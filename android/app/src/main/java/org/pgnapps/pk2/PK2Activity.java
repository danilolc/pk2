package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.content.res.AssetManager;

import java.io.IOException;
import java.util.Locale;

public class PK2Activity extends SDLActivity {
    private static final String LOG_TAG = "PK2";

    private String[] listDir(String dir) {

        AssetManager am = getAssets();
        String[] ret;
        
        try {

            ret = am.list(dir);

        } catch(IOException e) {

            e.printStackTrace();
            ret = new String[] { "" };

        }

        return ret;

    }

    private String getLocale() {

        return Locale.getDefault().getLanguage();

    }

    @Override
    protected String[] getArguments() {
        //return new String[0];
        return new String[] { "dev", "fps" };
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
        super.onStart();
        setWindowStyle(true);

    }

    @Override
    protected void onResume() {
        super.onResume();
        setWindowStyle(true);
    }
}