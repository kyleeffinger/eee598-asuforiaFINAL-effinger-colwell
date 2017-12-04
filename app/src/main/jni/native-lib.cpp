#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <android/log.h>
#include "features.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

extern "C" {

Mat descRef;
Mat descImage;

JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativePoseEstimation(JNIEnv *env, jobject instance,
                                                            jint height, jint width,
                                                            jobject byteBuffer) {

    Mat _yuv(height + height / 2, width, CV_8UC1, (uchar *) byteBuffer);
    Mat frame;
    cvtColor(_yuv, frame, CV_YUV2GRAY_420);
    //TODO: Implement nativePoseEstimation with OpenCV methods

    // detect features in frame
    Ptr<Feature2D> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();
    vector<KeyPoint> kp_image;

    detector->detect(frame, kp_image);

    extractor->compute(frame, kp_image, descImage);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of camera frame keypoints: ""%i", kp_image.size());

    //FEATURE MATCHING
    FlannBasedMatcher matcher;

    vector<vector<DMatch>>matches;

    matcher.knnMatch(descImage,descRef, matches,2);

    // use solvePnP


    // return r and t vecs


    return env->NewStringUTF("");
}


JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeFeatureDetection(JNIEnv *env, jobject This,
                                                              jstring referenceImage) {

    // TODO: Implement nativeFeatureDetection to extract ORB features from reference image

    const char *refImg = env->GetStringUTFChars(referenceImage, NULL);

    //Read the reference image
    Mat img = imread(refImg, CV_LOAD_IMAGE_GRAYSCALE);

    __android_log_print(ANDROID_LOG_INFO, "Mat", "%s", refImg);


    if (img.empty())
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image NOT Loaded");
    else
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image Loaded");

    Ptr<Feature2D> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();
    vector<KeyPoint> kp_ref;

    detector->detect(img, kp_ref);

    extractor->compute(img, kp_ref, descRef);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of reference image keypoints: " "%i", kp_ref.size());
    // __android_log_print(ANDROID_LOG_INFO, "Mat", "%s", descriptors11);

    return env->NewStringUTF("");
}



JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeCubeDraw(JNIEnv *env, jobject instance) {

    // TODO
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Testing ");


    return env->NewStringUTF("");
}

}