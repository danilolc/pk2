package jogo.danilolc.pekkakana2;

import android.os.Environment;

import java.io.File;

/**
 * Created by danilo on 23/01/18.
 */

public class PekkaKana2 {
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
}
