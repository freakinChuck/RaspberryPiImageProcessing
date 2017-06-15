package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.Application;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

/**
 * Created by silvio on 23.03.17.
 */
public class RedLightHeight {
    private int redYPos= 0;
    private int redXPos= 0;

    public int getRedYPos(){
        return redYPos;
    }
    public int getRedXPos(){
        return redXPos;
    }
    public boolean FindRedLightHeight(Mat frame){
        try {
            // Convert image to HSV
            Mat hsv_image_red = new Mat();
            Imgproc.cvtColor(frame, hsv_image_red, Imgproc.COLOR_BGR2HSV);
            Mat lower_red_hue_range = new Mat();
            Mat upper_red_hue_range = new Mat();
            //Rotbereich festlegen. um die roten Balken zu erkenne/filtern
            Core.inRange(hsv_image_red, new Scalar(0,95,75), new Scalar(15, 255, 255), lower_red_hue_range);
            Core.inRange(hsv_image_red, new Scalar(165, 95, 75), new Scalar(180, 255, 255), upper_red_hue_range);
            //Rotfilter anwenden
            Mat red_hue_image = new Mat();
            Core.addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
            Imgproc.GaussianBlur(red_hue_image, red_hue_image, new Size(9, 9), 2, 2);

            // Find Cirlces
            // -Settings
            Mat circles = new Mat();
            int iCannyUpperThreshold = 10;
            int iMinRadius = 10;
            int iMaxRadius = 20;
            int iAccumulator = 10;

            //Find Circles with Hough algo.
            Imgproc.HoughCircles(red_hue_image, circles, Imgproc.CV_HOUGH_GRADIENT,
                    1.0, red_hue_image.rows()/8, iCannyUpperThreshold, iAccumulator,
                    iMinRadius, iMaxRadius);

            // Paint Circles in Picture and print msg
            if (circles.cols() > 0) {
                for (int x = 0; x < circles.cols(); x++) {
                    double vCircle[] = circles.get(0, x);

                    if (vCircle == null)
                        break;
                    org.opencv.core.Point pt = new org.opencv.core.Point(Math.round(vCircle[0]), Math.round(vCircle[1]));
                    this.redYPos = (int)pt.y;
                    this.redXPos = (int)pt.x;

                    if (Configuration.DOTAKEIMAGES) {

                        MatToBufImg matToBufImg = new MatToBufImg();
                        matToBufImg.setMatrix(frame, ".png");

                        File outputfile = new File(Application.getFolderBaseBath() + "/" + "RedOriginal.png");
                        try {
                            ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                            System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                        }
                        catch (IOException e) {
                            e.printStackTrace();
                        }

                        matToBufImg = new MatToBufImg();
                        matToBufImg.setMatrix(red_hue_image, ".png");
                        outputfile = new File(Application.getFolderBaseBath() + "/" + "RedHue.png");
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
