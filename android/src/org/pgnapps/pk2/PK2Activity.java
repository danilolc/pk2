package org.pgnapps.pk2;

import org.libsdl.app.SDLActivity;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;
import java.io.File;

/*
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.lang.reflect.Method;
import java.util.Objects;

import android.app.*;
import android.content.*;
import android.text.InputType;
import android.view.*;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.os.*;
import android.util.Log;
import android.util.SparseArray;
import android.graphics.*;
import android.graphics.drawable.Drawable;
import android.hardware.*;
import android.content.pm.ActivityInfo;
*/
public class PK2Activity extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}
    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
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
