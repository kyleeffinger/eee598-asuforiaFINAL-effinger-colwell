#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

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




    
    Mat img = imread("C:\\Users\\Zachary\\AndroidStudioProjects\\eee598-asuforiaFINAL-effinger-colwell\\app\\src\\main\\res\\drawable");
    Mat descriptors_object;
    Mat descriptors_scene;

    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();
    vector<KeyPoint> kp;
    vector<KeyPoint> kp2;

    detector->detect(img,kp);
    extractor->compute(img,kp,descriptors_object);
    extractor->compute(img,kp2,descriptors_scene);

    FlannBasedMatcher matcher;

    vector<vector<DMatch>>matches;

    matcher.knnMatch(descriptors_object,descriptors_scene, matches,2);






    return env->NewStringUTF("");
}

}