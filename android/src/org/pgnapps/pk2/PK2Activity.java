package org.pgnapps.pk2;

import org.apache.commons.io.FileUtils;
import org.libsdl.app.SDLActivity;

import android.os.Build;
import android.util.Log;
import android.view.View;
import android.os.Bundle;
import android.os.Environment;
import android.view.WindowManager;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javax.net.ssl.HttpsURLConnection;

public class PK2Activity extends SDLActivity {
	private static final String LOG_TAG = "PK2Activity";


    public void makeFullScreen() {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if(Build.VERSION.SDK_INT < 19){
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        }else {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

    // https://stackoverflow.com/questions/3382996/how-to-unzip-files-programmatically-in-android
    private boolean unpackZip(String path, String zipname) {
        InputStream is;
        ZipInputStream zis;
        try
        {
            String filename;
            is = new FileInputStream(path + zipname);
            zis = new ZipInputStream(new BufferedInputStream(is));
            ZipEntry ze;
            byte[] buffer = new byte[1024];
            int count;

            while ((ze = zis.getNextEntry()) != null)
            {
                // zapis do souboru
                filename = ze.getName();

                // Need to create directories if not exists, or
                // it will generate an Exception...
                if (ze.isDirectory()) {
                    File fmd = new File(path + filename);
                    fmd.mkdirs();
                    continue;
                }

                FileOutputStream fout = new FileOutputStream(path + filename);

                // cteni zipu a zapis
                while ((count = zis.read(buffer)) != -1)
                {
                    fout.write(buffer, 0, count);
                }

                fout.close();
                zis.closeEntry();
            }

            zis.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    /*int Download_All(File directory, BufferedReader br){
        ArrayList<String> files_to_add = new ArrayList<String>();
        ArrayList<String> files_to_change = new ArrayList<String>();
        ArrayList<String> files_to_remove = new ArrayList<String>();
        String command;

        while(true) {
            try{
                command = new String(br.readLine().getBytes());
                if (command.startsWith("PK2")) break;
            } catch (IOException e) {
                e.printStackTrace();
                break;
            } catch (NullPointerException e) {
                break;
            }
            Log.d(LOG_TAG,command);
            String[] line = command.split(" ");

            if(line[0].equals("add"))
                files_to_add.add(line[1]);

            if (line[0].equals("change"))
                files_to_change.add(line[1]);

            if (line[0].equals("remove"))
                files_to_remove.add(line[1]);

        }

        for(int i = 0; i < files_to_add.size(); i++){
            String filename = files_to_add.get(i);
            try {
                URL url = new URL("https://raw.githubusercontent.com/danilolc/pk2/master/res/" + filename);
                InputStream is = url.openStream();
                byte[] buffer = new byte[is.available()];
                is.read(buffer);
                OutputStream os = new FileOutputStream(directory.getPath() + filename);
                os.write(buffer);
                Log.d(LOG_TAG, "File " + filename + " downlaoded");
            } catch (IOException e) {
                Log.e(LOG_TAG, "Can't get " + filename + " from server");
            }
        }

        return 0;
    }*/

    int Download_All(File directory, BufferedReader br){

        File file = new File(Environment.getExternalStorageDirectory() + "/0.1.zip");
        if(file.exists()) file.delete();

        try {
            URL url = new URL("https://raw.githubusercontent.com/danilolc/pk2/master/res/pack/0.1.zip");
            FileUtils.copyURLToFile(url, file, 10000, 10000);
        } catch (IOException e) {
            Log.e(LOG_TAG, "Can't get zip from server");
        }
        Log.e(LOG_TAG, file.getParent());

        unpackZip(file.getParent() + "/", file.getName());
        file.delete();

        return 0;
    }

	int Check_Assets(){
        if (!isExternalStorageWritable()) {
            Log.e(LOG_TAG, "Can't read external storage");
            return -1;
        }

        File directory = new File(Environment.getExternalStorageDirectory(), "Pekka Kana 2");

        String ver;
        BufferedReader br_loc;
        BufferedReader br_rem;

        try {
            URL url = new URL("https://raw.githubusercontent.com/danilolc/pk2/master/res/version.txt");
            URLConnection con = url.openConnection();
            InputStream is = con.getInputStream();
            br_rem = new BufferedReader(new InputStreamReader(is));
            ver = br_rem.readLine();
            ver = ver.split("-")[2];
            Log.d(LOG_TAG, "The upstream version is " + ver);
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }

        String remote_ver = ver;

		if(!directory.exists())
			return Download_All(directory, br_rem);

        try {
            br_loc = new BufferedReader(new FileReader(directory.getPath() + "/version.txt"));
            ver = ver.split("-")[2];
            Log.d(LOG_TAG, "The local version is " + ver);
        } catch (IOException e) {
            e.printStackTrace();
        }

        if(ver.isEmpty()) return Download_All(directory, br_rem);

        String local_ver = ver;

        if(Float.parseFloat(remote_ver) > Float.parseFloat(local_ver)){
            Log.w(LOG_TAG,"Have to update.");
        }
		return 0;
	}

	public boolean isExternalStorageWritable() {
		String state = Environment.getExternalStorageState();
		return Environment.MEDIA_MOUNTED.equals(state);
	}

	@Override
    protected String[] getArguments() {
	    Check_Assets(); // This is here just to call on SDL_main thread before call SDL_main
	    String[] args = new String[1];
	    args[0] = "dev";
        return args;
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

	//@Override
    //protected void onPause() {
    //    super.onPause();
    //}

    //@Override
    //public void onWindowFocusChanged(boolean hasFocus) {
    //    super.onWindowFocusChanged(hasFocus);
    //}

    //@Override
    //public void onLowMemory() {
    //    super.onLowMemory();
    //}

    //@Override
    //protected void onDestroy() {
    //    super.onDestroy();
    //}

    //@Override
    //public boolean dispatchKeyEvent(KeyEvent event) {
	//	return super.dispatchKeyEvent(event);
	//}

	//@Override
    //protected Dialog onCreateDialog(int ignore, Bundle args) {
	//	return super.onCreateDialog(ignore, args);
	//}
}
