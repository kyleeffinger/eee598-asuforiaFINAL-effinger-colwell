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

The [Camera2 API](https://developer.android.com/reference/android/hardware/camera2/package-summary.html) was used to setup the camera preview for the app. This is done in both the `MainActivity` and `ASUForia`. While testing the camera preview being displayed in the TextureView is unnecessary, the use of the camera requires user permission, which does require testing to ensure the function is understood exactly. In this case, the developers are not experienced enough with Android to handle this challenge, so upon the first install and open, the application will crash, since there is no pop up to ask for permissions. In order to get around this, first run the app in Android Studio. The app will crash, and just tap to close the app. Then head to Settings > Apps > Asuforia > Permissions and grant camera and storage permissions. Then reopen the app and the camera preview will be visable.

Another point of interest with the camera is that there is no handling of device rotation in the camera preview (although we tried). For this we recommend placing the device in portrait (with the front camera at the top of the device) and then lock the rotation by swiping down from the top and selecting *Portrait*. 

#### Milestone 2: Feature Detection of Reference Image

Once the camera has been setup and the preview is available, the next step is to read the reference image, and detect the key features in the image using OpenCV. While getting OpenCV setup correctly could also be considered a milestone, it will be passed over here to focus on the project objectives instead. The reference image features are detected in the native method `nativeFeatureDetection`, which is called by `onCreate()`. Several tests were used during development to check that this process is working correctly.


## Authors

* **Kyle Effinger** 
* **Zach Colwell**
