package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;

/**
 * Created by silvio on 23.03.17.
 */
public class RedLightHeight {
    private int redPos= 0;

    public int getRedPos(){
        return redPos;
    }
    public boolean FindRedLightHeight(Mat frame){
        try {
            MatOfByte mem = new MatOfByte();
            Imgcodecs.imencode(".bmp", frame, mem);
            Image im = ImageIO.read(new ByteArrayInputStream(mem.toArray()));
            BufferedImage buff = (BufferedImage) im;
            // Convert image to HSV
            Mat hsv_image_red = new Mat();
            Imgproc.cvtColor(frame, hsv_image_red, Imgproc.COLOR_BGR2HSV);
            Mat lower_red_hue_range = new Mat();
            Mat upper_red_hue_range = new Mat();
            //Rotbereich festlegen. um die roten Balken zu erkenne/filtern
            Core.inRange(hsv_image_red, new Scalar(0,100,100), new Scalar(10, 227, 255), lower_red_hue_range);
            Core.inRange(hsv_image_red, new Scalar(160, 100, 100), new Scalar(179, 255, 255), upper_red_hue_range);
            //Rotfilter anwenden
            Mat red_hue_image = new Mat();
            Core.addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
            Imgproc.GaussianBlur(red_hue_image, red_hue_image, new Size(9, 9), 2, 2);

            // Find Cirlces
            Mat color_image = frame;
            // -Settings
            Mat circles = new Mat();
            int iCannyUpperThreshold = 10;
            int iMinRadius = 10;
            int iMaxRadius = 20;
            int iAccumulator = 10;

            //Find Circles with Hough algo.
            Imgproc.HoughCircles(red_hue_image, circles, Imgproc.CV_HOUGH_GRADIENT,
                    1.0, red_hue_image.rows(), iCannyUpperThreshold, iAccumulator,
                    iMinRadius, iMaxRadius);

            // Paint Circles in Picture and print msg
            if (circles.cols() > 0) {
                for (int x = 0; x < circles.cols(); x++) {
                    double vCircle[] = circles.get(0, x);

                    if (vCircle == null)
                        break;
                    org.opencv.core.Point pt = new org.opencv.core.Point(Math.round(vCircle[0]), Math.round(vCircle[1]));
                    this.redPos = (int)pt.y;
                    return true;
                }
            }
            return false;


        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
            ex.printStackTrace();
            return false;
        }
    }
}
