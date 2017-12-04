#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <android/log.h>
#include "features.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"


using namespace cv;
using namespace std;



extern "C" {


// NATIVE POSE ESTIMATION
JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativePoseEstimation(JNIEnv *env, jobject instance) {

    //TODO: Implement nativePoseEstimation with OpenCV methods


    return env->NewStringUTF("");
}


// NATIVE FEATURE DETECTION
JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeFeatureDetection(JNIEnv *env, jobject This,
                                                              jobject referenceImage) {

    // TODO: Implement nativeFeatureDetection to extract ORB features from reference image

    //Read the reference image
    Mat img = imread("C:\\Users\\Zachary\\AndroidStudioProjects\\eee598-asuforiaFINAL-effinger-colwell\\app\\src\\main\\res\\drawable",CV_LOAD_IMAGE_GRAYSCALE);
    Mat outputImage;

//    if(&img != NULL)
//        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image Loaded");
//    else
//        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image NOT Loaded");


     //cvtColor(img,grayImg,CV_BGR2GRAY,0);
    //FEATURE DETECTION & EXTRACTION
    Mat descriptors1;
    Mat descriptors2;

    //Ptr<FeatureDetector> detector = ORB::create();
    Ptr<Feature2D> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();
    vector<KeyPoint> kp;
    vector<KeyPoint> kp2;

    detector->detect(img,kp);

    extractor->compute(img,kp,descriptors1);
    extractor->compute(img,kp2,descriptors2);

    //FEATURE MATCHING
    FlannBasedMatcher matcher;

    vector<vector<DMatch>>matches;

    matcher.knnMatch(descriptors1,descriptors2, matches,2);
    matcher.knnMatch(descriptors_object, descriptors_scene, matches, 2);


    return env->NewStringUTF("");
}



// NATIVE CUBE DRAWING METHOD
JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeCubeDraw(JNIEnv *env, jobject instance) {

    // TODO: figure out how to draw the cube on the reference image


    return env->NewStringUTF("");
}

}