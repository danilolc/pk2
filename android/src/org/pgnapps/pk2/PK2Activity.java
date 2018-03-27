package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.content.res.AssetManager;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class PK2Activity extends SDLActivity {
	private static final String LOG_TAG = "PK2Activity";
    private final String external_dir = Environment.getExternalStorageDirectory().getAbsolutePath();

    public void makeFullScreen() {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if(Build.VERSION.SDK_INT < 19){
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        }else {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    // https://stackoverflow.com/questions/3382996/how-to-unzip-files-programmatically-in-android
    private boolean unpackZip(String path, String zipname) {
        InputStream is;
        ZipInputStream zis;
        try {
            String filename;
            is = new FileInputStream(path + zipname);
            zis = new ZipInputStream(new BufferedInputStream(is));
            ZipEntry ze;
            byte[] buffer = new byte[1024];
            int count;

            while ((ze = zis.getNextEntry()) != null) {
                // write to file
                filename = ze.getName();

                // Need to create directories if not exists, or
                // it will generate an Exception...
                if (ze.isDirectory()) {
                    File fmd = new File(path + filename);
                    fmd.mkdirs();
                    continue;
                }

                FileOutputStream fout = new FileOutputStream(path + filename);

                // read zip and write
                while ((count = zis.read(buffer)) != -1)
                    fout.write(buffer, 0, count);

                fout.close();
                zis.closeEntry();
            }

            zis.close();
        }
        catch(IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

	boolean Check_Assets(){
        if (!isExternalStorageWritable()) {
            Log.e(LOG_TAG, "Can't read external storage");
            return false;
        }

        File directory = new File(external_dir, "Pekka Kana 2");
        if (directory.exists()) return true;

        InputStream is;
        OutputStream os;

        try {
            AssetManager assetManager = getAssets();
            is = assetManager.open("res.zip");
            os = new FileOutputStream(external_dir + "/res.zip");
            os.flush();
            copyFile(is,os);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        unpackZip(external_dir + "/", "res.zip");
        new File(external_dir,"res.zip").delete();

        return directory.exists();
	}

	@Override
    protected String[] getArguments() {
	    if(Check_Assets()) { // This is here just to call on SDL_main thread before call SDL_main
            String[] args = new String[2];
            args[0] = "path";
            args[1] = external_dir + "/Pekka Kana 2";
            return args;
        }
        return new String[0];
    }

	@Override
	protected String[] getLibraries() {
		return new String[] {
			"PK2"
		};
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
	    makeFullScreen();
		super.onCreate(savedInstanceState);
	}

    @Override
    protected void onResume() {
        makeFullScreen();
        super.onResume();
    }
}
