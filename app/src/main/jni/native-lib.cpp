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

/* This function is taken from the  function found in the native pose estimation opencv tutorial. This tests
 * the distance ratios between matches found using the knnmatcher to determine if the match is valid or not.
 * If the match is not valid (compared to threshold), then the match will be removed from the list of matches.
 */
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
            __android_log_print(ANDROID_LOG_INFO, "Keypoints", "dist1/dist2: %5.2f/%5.2f = %5.2f compared to %5.2f",
                                (*matchIterator)[0].distance,(*matchIterator)[1].distance,
                                (*matchIterator)[0].distance / (*matchIterator)[1].distance,ratio);

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

// Declaring Mat objects for descriptors and key points in order to have them accessible by multiple methods
// reference image descriptors
Mat descRef;
// camera frame descriptors
Mat descImage;
// reference image key points
vector<KeyPoint> kp_ref;
// camera frame key points
vector<KeyPoint> kp_image;


/*
 * This method is used to find the features in the camera frame being passed in by onImageAvailable().
 * onImageAvailable() passes the width and height of the TextureView in order to create the Mat object
 * correctly, as well as the camera frame in the forma of a byte buffer. The byte buffer is converted
 * into a Mat object, in YUV_420_888 format. This in then converted to gray scale for the features to
 * be read. Once the features of the camera frame are collected, they are compared to the features found
 * in the reference image. The matches are sent through the ratioTest to remove any outliers, and then
 * passed into the SolvePnP method to get the rotation and translation vectors. These r and t vectors
 * are then returned in a single array, so they can be split in Java after being returned through the JNI.
 */

JNIEXPORT jfloatArray JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativePoseEstimation(JNIEnv *env, jobject instance,
                                                            jint height, jint width,
                                                            jobject byteBuffer) {

    // pass camera image into Mat object
    Mat _yuv(height + height / 2, width, CV_8UC1, (uchar *) byteBuffer);
    // initialize camera frame Mat object
    Mat frame;
    // Fill camera frame Mat object with gray scale colored image after converting from YUV_420_888
    cvtColor(_yuv, frame, CV_YUV2GRAY_420);

    //FEATURE DETECTION

    // Initialize detector for ORB feature detection
    Ptr<Feature2D> detector = ORB::create();
    // Initialize extractor for ORB feature detection
    Ptr<DescriptorExtractor> extractor = ORB::create();

    // detect features in the camera frame and store the key points
    detector->detect(frame, kp_image);

    // extract the descriptors from the keypoints in the camera frame and store them
    extractor->compute(frame, kp_image, descImage);
    // Log the number of keypoints and the number of descriptors in the reference and the camera frame to test
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of camera frame keypoints: %i", kp_image.size());
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc image size: %i", descImage.rows);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc ref size: %i", descRef.rows);

    //FEATURE MATCHING

    // Initialize FlannMatcher objects used to match the features between the reference and frame
    Ptr<flann::IndexParams> indexParams = makePtr<flann::LshIndexParams>(6,12,1);
    Ptr<flann::SearchParams> searchParams = makePtr<flann::SearchParams>(50);
    // Create matcher object to complete matching (secondary method tried as troubleshooting)
    DescriptorMatcher * matcher1 = new FlannBasedMatcher(indexParams, searchParams);

    // Create matcher object
    FlannBasedMatcher matcher; //(new flann::LshIndexParams(6, 12, 1));

    // Initializ vector to contain matches
    vector<vector<DMatch>>matches;
    // Convert to 32 bit float for use with knn match. Done as troubleshooting step with matcher based on
    // recommendations found online.
    descImage.convertTo(descImage, CV_32F);
    descRef.convertTo(descRef, CV_32F);
    // log the number of matches for testing. Should be 0 at this point, which it is.
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    // Generate matches
    matcher.knnMatch(descImage,descRef, matches,2);
    // log number of matches found
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    // find distance attribute of matches. If less than some threshold, ignore
    int removed = ratioTest(matches);
    // Log new number of matches after ratioTest filtering
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i removed: %i", matches.size(), removed);


    // use solvePnP

    // solvePnP parameters from OpenCV documentation
    Mat distCoeffs = Mat::zeros(4,1,CV_64FC1);
    Mat rvec = Mat::zeros(3,1, CV_64FC1);
    Mat tvec = Mat::zeros(3,1, CV_64FC1);
    bool useExtrinsicGuess = false;

    // Initialize camera matrix for SolvePnP
    const _InputArray cameraMatrix;

    // Execute pose estimation and return r and t vectors
//    solvePnP(kp_ref, kp_image, cameraMatrix ,distCoeffs, rvec, tvec, useExtrinsicGuess);
//
//      // POSE ESTIMATION TESTING
//
//    //instantiate Rvec, Tvec and RTvec
//    //Rvec and Tvec will always contain 3 elements there RTvec will contain 6
//    float * Rvec = new float[3];
//    float * Tvec = new float[3];
//    float * RTvec = new float[6];
//
//    //Rvec itself is returned from solvePnP
//    //This data is just for example
//    //The same goes for Tvec
//    Rvec[0] = 1;
//    Rvec[1] = 2;
//    Rvec[2] = 3;
//
//    Tvec[0] = 4;
//    Tvec[1] = 5;
//    Tvec[2] = 6;
//
//    //In order to return the R and T vecs from a C++ function we must
//    //bring them together in a single array which will be passed
//    //as a pointer to Java
//    RTvec[0] = Rvec[0];
//    RTvec[1] = Rvec[1];
//    RTvec[2] = Rvec[2];
//    RTvec[3] = Tvec[0];
//    RTvec[4] = Tvec[1];
//    RTvec[5] = Tvec[2];


    // return r and t vecs to onImageAvailable for cube drawing
    return env->NewFloatArray(1);
}


/*
 * This method is used for detecting the features in the reference image. The reference image is passed
 * from the ASUForia constructor oin the form of an absolute path so that imread() will be able to read
 * the image file and pass into a Mat object. The features will then be read using the ORB detector and
 * extractor, and the key points and descriptors will be saved in their respective global variables so
 * they can be accessed by the pose estimation function.
 */
JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeFeatureDetection(JNIEnv *env, jobject This,
                                                              jstring referenceImage) {


    // store the path to the reference image
    const char *refImg = env->GetStringUTFChars(referenceImage, NULL);

    // Read the reference image and store in Mat object
    Mat img = imread(refImg, CV_LOAD_IMAGE_GRAYSCALE);

    // Log the path to the reference image to make sure that it is correct
    __android_log_print(ANDROID_LOG_INFO, "Mat", "%s", refImg);

    // Perform check to see if the Mat object is empty. If it is, log the error.
    if (img.empty())
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image NOT Loaded");
    else
        __android_log_print(ANDROID_LOG_INFO, "Mat", "Image Loaded");

    // instantiate ORB detector and extractor
    Ptr<Feature2D> detector = ORB::create();
    Ptr<DescriptorExtractor> extractor = ORB::create();

    // detect features and store key points
    detector->detect(img, kp_ref);

    // extract and store descriptors
    extractor->compute(img, kp_ref, descRef);
    // Log the number of key points found in the reference image for testing
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of reference image keypoints: " "%i", kp_ref.size());

    // Outputs of this function are stored in global variables, no need to return anything through JNI
    return env->NewStringUTF("");
}


/*
 * This method is used to draw the cube on the camera frame that is to be displayed in the TextureView.
 * This will be called by the onPose() method, since the drawing cannot be done until the rotation and
 * translation vectors have been computed.
 */

JNIEXPORT jstring JNICALL
Java_asuforia_group2_asuforia_ASUForia_nativeCubeDraw(JNIEnv *env, jobject instance) {

    // TODO
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Testing ");


    return env->NewStringUTF("");
}

}