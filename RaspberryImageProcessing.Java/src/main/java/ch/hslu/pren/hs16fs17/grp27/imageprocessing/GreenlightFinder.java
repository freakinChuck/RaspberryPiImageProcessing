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
public class GreenlightFinder {
    public boolean ImageContainsGreenLight(Mat frame){
        try {
            MatOfByte mem = new MatOfByte();
            Imgcodecs.imencode(".bmp", frame, mem);
            Image im = ImageIO.read(new ByteArrayInputStream(mem.toArray()));
            BufferedImage buff = (BufferedImage) im;
            // Convert image to HSV
            Mat hsv_image = new Mat();
            Imgproc.cvtColor(frame, hsv_image, Imgproc.COLOR_BGR2HSV);
            Mat green_hue_range = new Mat();
            //Set Green range
            int sensitivity = 15;
            Core.inRange(hsv_image, new Scalar(60 - sensitivity, 80, 60), new Scalar(60 + sensitivity, 255, 255), green_hue_range);
            //Slightly blur the image. Improves false-positiv circle detection
            Imgproc.GaussianBlur(green_hue_range, green_hue_range, new Size(9, 9), 2, 2);

            // Find Cirlces
            Mat color_image = frame;
            // -Settings
            Mat circles = new Mat();
            int iCannyUpperThreshold = 10;
            int iMinRadius = 10;
            int iMaxRadius = 20;
            int iAccumulator = 10;

            //Find Circles with Hough algo.
            Imgproc.HoughCircles(green_hue_range, circles, Imgproc.CV_HOUGH_GRADIENT,
                    1.0, green_hue_range.rows(), iCannyUpperThreshold, iAccumulator,
                    iMinRadius, iMaxRadius);

            // Paint Circles in Picture and print msg
            if (circles.cols() > 0) {
                for (int x = 0; x < circles.cols(); x++) {
                    double vCircle[] = circles.get(0, x);

                    if (vCircle == null)
                        break;
                    org.opencv.core.Point pt = new org.opencv.core.Point(Math.round(vCircle[0]), Math.round(vCircle[1]));
                    int radius = (int) Math.round(vCircle[2]);

                    //ImageIO.write(new MatToBufImg(green_hue_range, ".jpg").getBufferedImage(), "JPG", new FileOutputStream("/home/pi/Images/Redlight/" + "green_" + System.currentTimeMillis() + "_hue.jpg"));
                    //ImageIO.write(new MatToBufImg(frame, ".jpg").getBufferedImage(), "JPG", new FileOutputStream("/home/pi/Images/Redlight/" + "green_" + System.currentTimeMillis() + "_orig.jpg"));

                    return true;
                }
            }
            return false;
            // for color debuggin
            //Mat res = new Mat();
            //Core.bitwise_and( color_image,color_image, res ,green_hue_range);

        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
            ex.printStackTrace();
            return false;
        }
    }
}