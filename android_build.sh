rm -rf libs
rm -rf obj

mv RedEye jni
ndk-build
mv jni RedEye

rm -rf RedEyeAndroid/app/src/main/jniLibs
cp -r libs RedEyeAndroid/app/src/main/

mv RedEyeAndroid/app/src/main/libs RedEyeAndroid/app/src/main/jniLibs

rm -rf libs
rm -rf obj
