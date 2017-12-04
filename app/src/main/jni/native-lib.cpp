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

JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativePoseEstimation(JNIEnv *env, jobject instance) {

    //TODO: Implement nativePoseEstimation with OpenCV methods


    return env->NewStringUTF("");
}


JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeFeatureDetection(JNIEnv *env, jobject This, jobject referenceImage) {

    // TODO: Implement nativeFeatureDetection to extract ORB features from reference image

    //Read the reference image
    Mat img  = imread("app\\src\\main\\res\\drawable\\referenceimage.png",CV_LOAD_IMAGE_GRAYSCALE);
    Mat img2 = imread("app\\src\\main\\res\\drawable\\referenceimage.png",CV_LOAD_IMAGE_GRAYSCALE);


    //wait this is java tho
    try{
        InputStream inStream = getResources().openRawResource(R.raw.reference);


        File cascadeDir = getDir("ref", Context.MODE_PRIVATE);


        File mReferenceImage = new File(cascadeDir, "referenceImage.jpg");
        FileOutputStream outStream = new FileOutputStream(mReferenceImage);


        byte[] buffer = new byte[4096];
        int bytesRead;
        while ((bytesRead = inStream.read(buffer)) != -1) {
            outStream.write(buffer, 0, bytesRead);
        }


        inStream.close();
        outStream.close();


    }
    catch (Exception e) {
        e.printStackTrace();
    }

    imread("Testing",mReferenceImage.getAbsolutePath());




        imshow("Example", img);
    Mat outputImage;


    if(&img != NULL)
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image Loaded");
    else
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image NOT Loaded");

    if(&img2 != NULL)
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image Loaded");
    else
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image NOT Loaded");


    //FEATURE DESCRIPTORS IMAGE 1
    Mat descriptors11;
    Mat descriptors12;

    //FEATURE DESCRIPTORS IMAGE 2
    Mat descriptors21;
    Mat descriptors22;


    //Ptr<FeatureDetector> detector = ORB::create();
    Ptr<Feature2D> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();
    vector<KeyPoint> kp;
    vector<KeyPoint> kp2;

    detector->detect(img,kp);

    extractor->compute(img,kp,descriptors11);
    extractor->compute(img,kp2,descriptors12);

    //FEATURE MATCHING
    FlannBasedMatcher matcher;

    vector<vector<DMatch>>matches;

    matcher.knnMatch(img,img2, matches,2);

    return env->NewStringUTF("");
}

}