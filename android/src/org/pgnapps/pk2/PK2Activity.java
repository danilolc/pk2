package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.os.Build;
import android.util.Log;
import android.view.View;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;
import android.content.res.AssetManager;
import android.view.WindowManager;
import java.io.File;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.*;
//import org.apache.commons.io.FileUtils;

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

	/*https://developer.android.com/training/data-storage/files.html#java*/
	/* Checks if external storage is available for read and write */
	public boolean isExternalStorageWritable() {
		String state = Environment.getExternalStorageState();
		if (Environment.MEDIA_MOUNTED.equals(state)) {
		    return true;
		}
		return false;
	}

	private List<String> assets_list = new ArrayList<String>();
	/*https://stackoverflow.com/questions/16234529/list-of-files-in-assets-folder-and-its-subfolders*/
	private boolean listAssetFiles(String path) {
		String[] list;
		try {
		    list = getAssets().list(path);
		    if (list.length > 0) {
		        // This is a folder
		        for (String file : list) {
		            if (!listAssetFiles(path + "/" + file))
		                return false;
		            else {
		                assets_list.add(file);
		            }
		        }
		    }
		} catch (IOException e) {
		    return false;
		}

		return true;
	}
	/*https://stackoverflow.com/questions/4447477/how-to-copy-files-from-assets-folder-to-sdcard*/
	private void copyAssets(String to) {
		AssetManager assetManager = getAssets();
		String[] files = null;
		listAssetFiles("");
		for (String filename : assets_list) {
			Log.e(LOG_TAG, filename);
		    InputStream in = null;
		    OutputStream out = null;
		    try {
		      in = assetManager.open(filename);
		      File outFile = new File(to, filename);
			  //if(!outFile.exists())
			  //    outFile.mkdirs()
		      out = new FileOutputStream(outFile);
		      copyFile(in, out);
		    } catch(IOException e) {
		        Log.e("tag", "Failed to copy asset file: " + filename, e);
		    }
		    finally {
		        if (in != null) {
		            try {
		                in.close();
		            } catch (IOException e) {
		                // NOOP
		            }
		        }
		        if (out != null) {
		            try {
		                out.close();
		            } catch (IOException e) {
		                // NOOP
		            }
		        }
		    }
		}
	}
	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1){
		  out.write(buffer, 0, read);
		}
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        makeFullScreen();
		if (!isExternalStorageWritable())
			Log.e(LOG_TAG, "Can't read external storage");
        File file = new File(Environment.getExternalStorageDirectory(), "Pekka Kana 2");
		if (!file.exists()){
			file.mkdirs();
			copyAssets(file.getAbsolutePath());
		}
        System.setProperty("user.dir", file.getAbsolutePath());

		super.onCreate(savedInstanceState);
	}
    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        makeFullScreen();
        super.onResume();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
		return super.dispatchKeyEvent(event);
	}

	//@Override
    //protected Dialog onCreateDialog(int ignore, Bundle args) {
	//	return super.onCreateDialog(ignore, args);
	//}
}
