#include "com_redknot_redeyeandroid_NativeMethod.h"
#include "App/App.hpp"
#include "Engine/Engine.hpp"

App * app;

JNIEXPORT void JNICALL Java_com_redknot_redeyeandroid_NativeMethod_Start
  (JNIEnv *, jclass, jint width, jint height)
  {
    Screen * screen = Screen::getInstance();
  	screen->setWidthAndHeight(width, height);

    app = new App();
    app->Start();
  }

JNIEXPORT void JNICALL Java_com_redknot_redeyeandroid_NativeMethod_Update
  (JNIEnv *, jclass)
  {
    app->Update();
  }

JNIEXPORT void JNICALL Java_com_redknot_redeyeandroid_NativeMethod_End
  (JNIEnv *, jclass)
  {
    app->End();

  	delete app;
  }
