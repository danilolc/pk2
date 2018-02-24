package jogo.danilolc.pekkakana2;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;

import org.libsdl.app.SDLActivity;

import java.io.File;

/**
 * Created by danilo on 23/01/18.
 */



public class PekkaKana2 extends SDLActivity {
    private static final String dirPath = Environment.getExternalStorageDirectory()+"/PekkaKana2/";


    public boolean isSet(){
        File f = new File(dirPath);
        return f.isDirectory();
    }

    public void CopyContent(){
        File dir = new File(dirPath);
        if(dir.isDirectory()) dir.delete();
        dir.mkdir();
        //Copy content of res
    }

/*
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }
    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
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
*/


}
