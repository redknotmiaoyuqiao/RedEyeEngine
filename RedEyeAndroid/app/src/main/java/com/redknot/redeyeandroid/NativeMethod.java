package com.redknot.redeyeandroid;

/**
 * Created by redknot on 09/11/2017.
 */

public class NativeMethod {
    static{
        System.loadLibrary("redeye");
    }

    public native static void Start(int width,int height);
    public native static void Update();
    public native static void End();
}
