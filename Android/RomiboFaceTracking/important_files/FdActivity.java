package org.opencv.samples.facedetect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;

public class FdActivity extends Activity implements CvCameraViewListener2 {

    private static final String    TAG                 = "OCVSample::Activity";
    
    // Color of the box surrounding the detected face.
    private static final Scalar    FACE_RECT_COLOR     = new Scalar(0, 255, 0, 255);
    
    // Which detector to use
    public static final int        JAVA_DETECTOR       = 0;
    public static final int        NATIVE_DETECTOR     = 1;
    
    // Radii of the eye circles.
    public static final int        EYE_WHITE_RADIUS    = 160;
    public static final int        EYE_PUPIL_RADIUS    = 40;
    
    // The ratio that the limited frame will be to the previously detected face.
    public static final double	   BOUND_RATIO		   = 0.8;
    
    // How often the detection is to be run.
    private static final int	   SAMPLE_RATE         = 1;

    private MenuItem               mItemFace50;
    private MenuItem               mItemFace40;
    private MenuItem               mItemFace30;
    private MenuItem               mItemFace20;
    private MenuItem               mItemType;

    private Mat                    mRgba;
    private Mat                    mGray;
    private File                   mCascadeFile;
    private CascadeClassifier      mJavaDetector;
    private DetectionBasedTracker  mNativeDetector;

    private int                    mDetectorType       = JAVA_DETECTOR;
    private String[]               mDetectorName;

    private float                  mRelativeFaceSize   = 0.2f;
    private int                    mAbsoluteFaceSize   = 0;

    private CameraBridgeViewBase   mOpenCvCameraView;
    

    private static int	           current_frame       = -1;
    private static Mat             saved_frame;
	private static Rect 		   lastFaceFrame	   = null;
    
    
    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");

                    // Load native library after(!) OpenCV initialization
                    System.loadLibrary("detection_based_tracker");

                    try {
                        // load cascade file from application resources
                        InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
                        File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
                        mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
                        FileOutputStream os = new FileOutputStream(mCascadeFile);

                        byte[] buffer = new byte[4096];
                        int bytesRead;
                        while ((bytesRead = is.read(buffer)) != -1) {
                            os.write(buffer, 0, bytesRead);
                        }
                        is.close();
                        os.close();

                        mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
                        if (mJavaDetector.empty()) {
                            Log.e(TAG, "Failed to load cascade classifier");
                            mJavaDetector = null;
                        } else
                            Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

                        mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);
                        
                        cascadeDir.delete();

                    } catch (IOException e) {
                        e.printStackTrace();
                        Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
                    }

                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public FdActivity() {
        mDetectorName = new String[2];
        mDetectorName[JAVA_DETECTOR] = "Java";
        mDetectorName[NATIVE_DETECTOR] = "Native (tracking)";

        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.face_detect_surface_view);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.fd_activity_surface_view);
        //mOpenCvCameraView.
        mOpenCvCameraView.setCvCameraViewListener(this);
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        mGray = new Mat();
        mRgba = new Mat();
    }

    public void onCameraViewStopped() {
        mGray.release();
        mRgba.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {

    	// Increment current frame counter
		current_frame = (current_frame + 1) % SAMPLE_RATE;
		// And only run algorithm once every "current_frame" times.
    	if(current_frame != 0)
    	{
    		return saved_frame;
    	}
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();

        if (mAbsoluteFaceSize == 0) {
            int height = mGray.rows();
            if (Math.round(height * mRelativeFaceSize) > 0) {
                mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
            }
            mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);
        }

        MatOfRect faces = new MatOfRect();
        
        if (mDetectorType == JAVA_DETECTOR) {
            if (mJavaDetector != null) {
            	if(lastFaceFrame != null) {
            		// If a face was detected in the previous frame:
            		// Limit the size of the matrix being searched for the next face
            		
            		int yMin = Math.max(0, (int)lastFaceFrame.tl().y -
            				(int)(lastFaceFrame.height * BOUND_RATIO));
            		int yMax = Math.min(mGray.height(), (int)lastFaceFrame.br().y +
            				(int)(lastFaceFrame.height * BOUND_RATIO));
            		int xMin = Math.max(0, (int)lastFaceFrame.tl().x -
            				(int)(lastFaceFrame.width * BOUND_RATIO));
            		int xMax = Math.min(mGray.width(), (int)lastFaceFrame.br().x +
            				(int)(lastFaceFrame.width * BOUND_RATIO));
            		
            		mGray = mGray.submat(yMin, yMax, xMin, xMax);
            		
            	}
            	mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, // TODO: objdetect.CV_HAAR_SCALE_IMAGE
            			new Size(mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
            }
        }
        else if (mDetectorType == NATIVE_DETECTOR) {
        	// NATIVE_DETECTOR is the face tracking built in to OpenCVJava.
            if (mNativeDetector != null)
                mNativeDetector.detect(mGray, faces);
        }
        else {
            Log.e(TAG, "Detection method is not selected!");
        }

        Rect targetFace = null;
        Rect[] facesArray = faces.toArray();
        
        // Get the largest detected face
        for (Rect r: facesArray) {
        	if(targetFace == null ||  r.area() > targetFace.area())
        		targetFace = r;
        }
       
        // Draw the irises
    	Core.circle(mRgba, new Point(mRgba.width()/4 + 100, mRgba.height()/2), EYE_WHITE_RADIUS, new Scalar(255,255,255,255), -1);
    	Core.circle(mRgba, new Point(3*mRgba.width()/4 - 100, mRgba.height()/2), EYE_WHITE_RADIUS, new Scalar(255,255,255,255), -1);
    	
        if(targetFace != null)
        {
        	// If  face is found
        	Core.rectangle(mRgba, targetFace.tl(), targetFace.br(), FACE_RECT_COLOR, 3);

            // Location of pupils based on face location
        	double xCenter = (targetFace.tl().x + targetFace.br().x) / 2.0;
        	double yCenter = (targetFace.tl().y + targetFace.br().y) / 2.0;
        	
        	int xCoord = (int)((xCenter / mRgba.width())*EYE_WHITE_RADIUS) - EYE_WHITE_RADIUS / 2;
        	int yCoord = (int)((yCenter / mRgba.height())*EYE_WHITE_RADIUS) - EYE_WHITE_RADIUS / 2;


        	// Draw pupils
        	Core.circle(mRgba, new Point((3 * mRgba.width() / 4 - 100) - xCoord, yCoord + (mRgba.height() / 2)), EYE_PUPIL_RADIUS, new Scalar(0,0,0), -1);
        	Core.circle(mRgba, new Point((mRgba.width() / 4 + 100) - xCoord, yCoord + (mRgba.height() / 2)), EYE_PUPIL_RADIUS, new Scalar(0,0,0), -1);
        	
        	// Save the location of the detected face.
        	lastFaceFrame = new Rect((int)targetFace.tl().x, (int)targetFace.tl().y,
        							 (int)targetFace.br().x, (int)targetFace.br().y);
        	
        }
        else
        {
        	// No face detected
        	lastFaceFrame = null;
        }
        
        saved_frame = mRgba;
        
        return mRgba;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");
        mItemFace50 = menu.add("Face size 50%");
        mItemFace40 = menu.add("Face size 40%");
        mItemFace30 = menu.add("Face size 30%");
        mItemFace20 = menu.add("Face size 20%");
        mItemType   = menu.add(mDetectorName[mDetectorType]);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
        if (item == mItemFace50)
            setMinFaceSize(0.5f);
        else if (item == mItemFace40)
            setMinFaceSize(0.4f);
        else if (item == mItemFace30)
            setMinFaceSize(0.3f);
        else if (item == mItemFace20)
            setMinFaceSize(0.2f);
        else if (item == mItemType) {
            mDetectorType = (mDetectorType + 1) % mDetectorName.length;
            item.setTitle(mDetectorName[mDetectorType]);
            setDetectorType(mDetectorType);
        }
        return true;
    }

    private void setMinFaceSize(float faceSize) {
        mRelativeFaceSize = faceSize;
        mAbsoluteFaceSize = 0;
    }

    private void setDetectorType(int type) {
        if (mDetectorType != type) {
            mDetectorType = type;

            if (type == NATIVE_DETECTOR) {
                Log.i(TAG, "Detection Based Tracker enabled");
                mNativeDetector.start();
            } else {
                Log.i(TAG, "Cascade detector enabled");
                mNativeDetector.stop();
            }
        }
    }
}
