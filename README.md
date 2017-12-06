# eee598-asuforiaFINAL-effinger-colwell
Project 2 for EEE 598 Mobile Systems Architecture, correctly configured to use OpenCV with C++.


## Getting Started

This project set out to make a library using OpenCV to draw a cube in real time on a reference image within a camera frame using the NVIDIA Shield tablet. The app used to demonstrate this library is called Asuforia. The library is contained in /app/src/main/java/asuforia/group2/asuforia/ASUForia.java and contains the Camera2 API methods used, as well as the calls to the native OpenCV methods. The OpenCV tasks are located in /app/src/main/jni/native-lib.cpp. 

### Prerequisites

This app uses OpenCV 3.3.1 for Android, which is implemented through the JNI to C++. In order to use OpenCV in a native Android environment, the Android NDK and OpenCV SDK local paths must be linked in the Android.mk file. 

### Installing and Running

In order to run the app on another machine, the local path variables must be changed in the Android.mk file. The `OPENCV_ROOT:` must point to the local location of the `OpenCV-android-sdk` file, which is the Android Pack downloaded from [OpenCV Releases](https://opencv.org/releases.html). Furthermore, the `NDK_MODULE_PATH:` needs to point to the Android NDK bundle, which can be installed from the SDK Manager within Android Studio. These path changes can be seen in the Android.mk file below. 

The Android.mk file

```
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_ROOT:=/Users/kyleeffinger/OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCV_ROOT}/sdk/native/jni/OpenCV.mk

NDK_MODULE_PATH=/Users/kyleeffinger/Library/Android/sdk/ndk-bundle
LOCAL_ARM_NEON := true
LOCAL_SRC_FILES := native-lib.cpp
LOCAL_CPPFLAGS := -std=gnu++0x
LOCAL_CFLAGS += -O2
LOCAL_LDLIBS += -llog -ldl
LOCAL_MODULE := group2

include $(BUILD_SHARED_LIBRARY)
```

## Testing the functionality

While the application was not completely finished, several major milestones were met and confirmed through various testing, outlined below. The testing has also outlined some key short comings, which are explained in their respective sections.

#### Milestone 1: Camera2 API

The [Camera2 API](https://developer.android.com/reference/android/hardware/camera2/package-summary.html) was used to setup the camera preview for the app. This is done in both the `MainActivity` and `ASUForia`. While testing the camera preview being displayed in the TextureView is unnecessary, the use of the camera requires user permission, which does require testing to ensure the function is understood exactly. In this case, the developers are not experienced enough with Android to handle this challenge, so **upon the first install** and open, **the application will crash, since there is no pop up to ask for permissions.** In order to get around this, first run the app in Android Studio. The app will crash, and just tap to close the app. Then head to Settings > Apps > Asuforia > Permissions and grant camera and storage permissions. Then reopen the app and the camera preview will be visable.

**Another point of interest** with the camera is that **there is no handling of device rotation in the camera preview** (although we tried). For this we recommend placing the device in portrait (with the front camera at the top of the device) and then lock the rotation by swiping down from the top and selecting *Portrait*. 

#### Milestone 2: Feature Detection of Reference Image

Once the camera has been setup and the preview is available, the next step is to read the reference image, and detect the key features in the image using OpenCV. While getting OpenCV setup correctly could also be considered a milestone, it will be passed over here to focus on the project objectives instead. The reference image features are detected in the native method `nativeFeatureDetection`, which is called by `onCreate()`. Several tests were used during development to check that this process is working correctly. 

First, the reference image path must be determined so that it can be passed to the OpenCV method `imread()`. This is done using the `getResources()` method of our Activity, and then passing the path of our File object (`myReferenceImage.getAbsolutePath()`) into `nativeFeatureDetection`. The code to do this is written in the ASUFria constructor so that it is called during the `onCreate()` process. This is tested by logging the path, and then checking if the `Mat` object is empty, using the OpenCV `empty()` method of the Mat class. This can can be seen in the code snippet below.

```
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
```
Once the image is loaded, the features are detected, and to test that the features are being detected, they are also logged:

```
detector->detect(img, kp_ref);

    extractor->compute(img, kp_ref, descRef);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of reference image keypoints: " "%i", kp_ref.size());
    // __android_log_print(ANDROID_LOG_INFO, "Mat", "%s", descriptors11);
```
This confirms that the reference image is being read properly in the Java code, being passed through the JNI, and being processed by OpenCV.

#### Milestone 3: Feature Detection of Camera Frame

Once the reference image is passed and features are detected, the camera frame is passed using the `onImageAvailable()` callback function. Using an `ImageReader` object, the cameraframe is passed to `nativePoseEstimation()` where it is converted from its `YUV_420_888` format to a `Mat` object. The features are tested the same way as the reference image.

```
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
//    vector<KeyPoint> kp_image;

    detector->detect(frame, kp_image);

    extractor->compute(frame, kp_image, descImage);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of camera frame keypoints: %i", kp_image.size());
```

#### Milestone 4: Matching of Features In Reference and Camera Frame

Once the features are detected in both the reference image and the camera frame, the next step is to compare the features and find matches so that they can be used to estimate the camera pose. In order to check this, the number of features in each are compared to ensure they are the same (set to the default value of 500).

```
__android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc image size: %i", descImage.rows);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "Desc ref size: %i", descRef.rows);
```

The number of features in both is found to be the same, and the `FlannBasedMatcher` is then used to match the features. There appears to be an issue with this portion of the code. The number of matches is checked before the matcher is used, and confirmed to be 0, but then after the match is performed, a high number of matches is found even when the reference image is not within the camera view finder. Using the `ratioTest()` method to attempt to remove the potential outliers, only a few matches are removed, confirming that there is an issue with this portion of the code. 

```
//FEATURE MATCHING
    Ptr<flann::IndexParams> indexParams = makePtr<flann::LshIndexParams>(6,12,1);
    Ptr<flann::SearchParams> searchParams = makePtr<flann::SearchParams>(50);
    DescriptorMatcher * matcher1 = new FlannBasedMatcher(indexParams, searchParams);

    FlannBasedMatcher matcher; //(new flann::LshIndexParams(6, 12, 1));

    vector<vector<DMatch>>matches;
    descImage.convertTo(descImage, CV_32F);
    descRef.convertTo(descRef, CV_32F);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    matcher.knnMatch(descImage,descRef, matches,2);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i", matches.size());

    // find distance attribute of matches. If less than some threshold, ignore
    int removed = ratioTest(matches);
    __android_log_print(ANDROID_LOG_INFO, "Keypoints", "# of matches: ""%i removed: %i", matches.size(), removed);
```

#### Unfinished Milestones and Their Progress

##### Camera Pose Estimation

Due to the unknown issue with the matcher, no further OpenCV-related milestones could be met. However, the structure of the code that would be needed for those milestones has been constructed. As an example, the `solvePnP` method has been setup, and passed the correct arguments, such that is the matcher could be fixed, the pose could be estimated using the rotation and translation (`rvec` and `tvec`). 

```
Mat distCoeffs = Mat::zeros(4,1,CV_64FC1);
    Mat rvec = Mat::zeros(3,1, CV_64FC1);
    Mat tvec = Mat::zeros(3,1, CV_64FC1);

    bool useExtrinsicGuess = false;

    const _InputArray cameraMatrix;

    solvePnP(kp_ref, kp_image, cameraMatrix ,distCoeffs, rvec, tvec, useExtrinsicGuess);
```

Furthermore, the ASUForia `onImageAvailable()` callback is expecting the float array output that will be passed to `onPose`, which is where the cube would be drawn.

##### Camera Cube Drawing

Again due to the matcher issue, the cube cannot be drawn on the reference image. However, in `onPose()` a call to the native method `nativeCubeDraw()` is present, such that the framework is present to complete the project if the matcher issues were resolved.

## Authors

* **Kyle Effinger** 
* **Zach Colwell**
