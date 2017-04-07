package ch.hslu.pren.hs16fs17.grp27.io;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.videoio.VideoCapture;

/**
 * Created by silvio on 20.03.17.
 */
public class Camera {
    VideoCapture capture = null;

    public Camera(int index){
        capture = new VideoCapture(index);
    }

    public boolean Available(){
        return capture.isOpened();
    }

    public Mat Capture(){
        Mat captureImageMat = new Mat();
        capture.read(captureImageMat);

        return captureImageMat;
    }

    public void Shutdown() {
        capture.release();
    }
}

