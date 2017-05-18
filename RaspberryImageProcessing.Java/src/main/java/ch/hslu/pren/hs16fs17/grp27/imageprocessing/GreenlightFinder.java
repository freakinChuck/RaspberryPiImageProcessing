package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.Application;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by silvio on 23.03.17.
 */
public class GreenlightFinder {
    public boolean ImageContainsGreenLight(Mat frame){
        try {
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
            if (circles.cols() > 0) {
                for (int x = 0; x < circles.cols(); x++) {
                    double vCircle[] = circles.get(0, x);

                    if (vCircle == null)
                        break;


                    if (Configuration.DOTAKEIMAGES) {

                        MatToBufImg matToBufImg = new MatToBufImg();
                        matToBufImg.setMatrix(frame, ".png");

                        File outputfile = new File(Application.getFolderBaseBath() + "/" + "GreenOriginal.png");
                        try {
                            ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                            System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                        }
                        catch (IOException e) {
                            e.printStackTrace();
                        }

                        matToBufImg = new MatToBufImg();
                        matToBufImg.setMatrix(green_hue_range, ".png");
                        outputfile = new File(Application.getFolderBaseBath() + "/" + "GreenHue.png");
                        try {
                            ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                            System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                        }
                        catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

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
