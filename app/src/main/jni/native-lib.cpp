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

int ratioTest(std::vector<std::vector<cv::DMatch> > &matches)
{
    float ratio = 0.8;
    int removed = 0;
    // for all matches
    for ( std::vector<std::vector<cv::DMatch> >::iterator
                  matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
    {
        // if 2 NN has been identified
        if (matchIterator->size() > 1)
        {
            // check distance ratio
            __android_log_print(ANDROID_LOG_INFO, "Keypoints", "dist1/dist2: %5.2f/%5.2f = %5.2f compared to %5.2f", (*matchIterator)[0].distance,(*matchIterator)[1].distance,(*matchIterator)[0].distance / (*matchIterator)[1].distance,ratio);
            if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio)
            {
                matchIterator->clear(); // remove match
                removed++;
            }
        }
        else
        { // does not have 2 neighbours
            matchIterator->clear(); // remove match
            removed++;
        }
    }
    return removed;
}

extern "C" {

Mat descRef;
Mat descImage;

JNIEXPORT jfloatArray JNICALL
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
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of camera frame keypoints: %i", kp_image.size());
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc image size: %i", descImage.rows);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc ref size: %i", descRef.rows);

    //FEATURE MATCHING
    FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2));

    vector<vector<DMatch>>matches;
    //descImage.convertTo(descImage, CV_32F);
    //descRef.convertTo(descRef, CV_32F);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    matcher.knnMatch(descImage,descRef, matches,2);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    // find distance attribute of matches. If less than some threshold, ignore
    int removed = ratioTest(matches);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i removed: %i", matches.size(), removed);


    // use solvePnP


    // return r and t vecs


    return env->NewFloatArray(1);
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