package com.redknot.redeyeandroid;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by redknot on 09/11/2017.
 */

public class MainRenderer implements GLSurfaceView.Renderer{

    private Context context;
    private int width = 0;
    private int height = 0;

    public MainRenderer(Context context)
    {
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        this.width = width;
        this.height = height;

        NativeMethod.Start(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeMethod.Update();
    }
}
