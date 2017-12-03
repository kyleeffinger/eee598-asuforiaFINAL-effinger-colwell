package asuforia.group2.asuforia;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.Image;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Surface;
import android.view.TextureView;

public class MainActivity extends AppCompatActivity {


    /******************************* Start of Activity Lifecycle *******************************************/


    /**
     * As a user opens, navigates through, leaves, and returns to our application, our Activity instance goes
     * through many different states in what Google calls the Activity Lifecycle. When the Activity transitions
     * into a new state, the Android system invokes the corresponding callback. There are 6 core callbacks,
     * referred to as onCreate(), onStart(), onResume(), onPause(), onStop(), and onDestroy(). Of particular
     * importance to us are onCreate(), onResume(), and onPause(). onCreate is called when the Activity is
     * first launched, meaning that it is not already in memory. This must be implemented, or the application
     * will not run. In this method, we want to perform the actions and logic that should only happen once
     * throughout the lifecycle of the Activity (like initialize objects and variables). The Activity does not
     * stay in this state after completing these tasks. The Activity interacts with the user in the onResume()
     * state. The app stays in this state until something takes the focus away from the app (such as opening
     * another app or going to the home screen). When the application goes to the background like this, the
     * app will transition into the onPause() state. In our case, the onCreate() state will be used to setup
     * the interface between the ASUForia library and the MainActivity, as well as define onPose() to perform
     * the cube drawing. onResume() and onPause() will be used to ensure that pose estimation starts when the
     * app comes to the foreground and ends when the app goes to the background.
     */

    ASUForia asuforia;
    TextureView myTextureView;
    Bitmap bmpIn;

    // Define what happens when the application is first opened
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /**
         Import reference jpg image as Bitmap (3 lines of code below code based on project 1). Can keep image as Bitmap
         until it reaches C++ world, then will be converted to Mat for use with OpenCV.
         */
        BitmapFactory.Options opts = new BitmapFactory.Options();
        opts.inPreferredConfig = Bitmap.Config.ARGB_8888; // Each pixel is 4 bytes: Alpha, Red, Green, Blue
        bmpIn = BitmapFactory.decodeResource(getResources(), R.drawable.referenceimage, opts);

        //TODO: Define PoseListener object to act as interface between ASUForia library and MainActivity
        final ASUForia.PoseListener myPoseListener = new ASUForia.PoseListener() {

            //TODO: Define PoseListener callback function, onPose() which will use OpenCV to draw cube on image
            public void onPose(Image cameraFrame, float[] rvec, float[] tvec) {

                //TODO: Paint cameraSurface with a cube as an overlay on the marker in the image using OpenCV.


            }

        };

        //TODO: Implement this in C++ - nativeFeatureDetection()

//        Mat mat = new Mat(bmpIn.getHeight(), bmpIn.getWidth(), CvType.CV_8UC3);
//        Utils.bitmapToMat(bmpIn, mat);

        // This will probably need to be Texture instead of Surface since we are using TextureView. "Whatever it needs
        // to be for it to work is fine" -Robert
        Surface cameraSurface = null; // can change to texture if needed

        //TODO: Create an ASUForia object
        asuforia = new ASUForia(myPoseListener, bmpIn, cameraSurface, this);
       /*
       This function is going to pass bitmap to ASUForia, then ASUForia is linked to native-lib through line at bottom
       "public native String nativeFeatureDetection();". Then, once in C++, Bitmap can be converted to Mat. OpenCV will
       need to use Mat data type for all methods.
        */



    }

    // Define what happens when our application enters and stays in the foreground
    @Override
    protected void onResume() {
        // call super constructor in order to apply onResume() to entire Activity
        super.onResume();

        //TODO: Call startEstimation() to setup camera and pass to onImageAvailable to nativePoseEstimation() to onPose()
        // startEstimation() only needs to be called here since onResume() will be called when the application
        // is first opened, and every time the app is brought into the foreground.
        myTextureView = (TextureView) findViewById(R.id.cameraPreview);

        asuforia.startEstimation(myTextureView);
    }


    // Define what happens when app is put in the background
    @Override
    protected void onPause() {

        // Call super constructor to apply to entire activity
        super.onPause();

        //TODO: Call endEstimation() to stop the camera and free resources when camera is not visible
        asuforia.endEstimation();

    }

    /********************************* End of Activity Lifecycle *******************************************/

}
