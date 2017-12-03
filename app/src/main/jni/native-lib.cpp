#include <jni.h>
#include "opencv2/opencv.hpp"

//
JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativePoseEstimation(JNIEnv *env, jobject instance) {

    //TODO: Implement nativePoseEstimation with OpenCV methods


    return env->NewStringUTF("");
}


JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeFeatureDetection(JNIEnv *env, jobject instance,
                                                              jobject referenceImage) {

    // TODO: Implement nativeFeatureDetection to extract ORB features from reference image


    return env->NewStringUTF("");
}