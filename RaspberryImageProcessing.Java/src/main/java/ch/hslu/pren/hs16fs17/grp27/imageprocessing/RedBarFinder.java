package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.Application;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import javafx.beans.property.StringProperty;
import org.opencv.core.*;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by silvio on 21.03.17.
 */
public class RedBarFinder {

    public Image FindRedDoubleBar(Mat matToProcess, int index){
        Mat hsv_image = new Mat();
        Imgproc.cvtColor(matToProcess, hsv_image, Imgproc.COLOR_BGR2HSV);

        Mat lower_red_hue_range = new Mat();
        Mat upper_red_hue_range = new Mat();

        //Rotbereich festlegen. um die roten Balken zu erkenne/filtern
        Core.inRange(hsv_image, new Scalar(0,100,100), new Scalar(10, 227, 255), lower_red_hue_range);
        Core.inRange(hsv_image, new Scalar(160, 100, 100), new Scalar(179, 255, 255), upper_red_hue_range);

        //Rotfilter anwenden
        Mat red_hue_image = new Mat();
        Core.addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
        Imgproc.GaussianBlur(red_hue_image, red_hue_image, new Size(9, 9), 2, 2);

        //Show only red --- For Debugging
        //Imgproc.GaussianBlur(red_hue_image, webcam_image, new Size(9, 9), 2, 2);
        List<MatOfPoint> contours = new ArrayList<>();
        Imgproc.findContours(red_hue_image, contours, new Mat(), Imgproc.RETR_LIST,Imgproc.CHAIN_APPROX_SIMPLE);

        //---------- Find both red bars----------------
        MatOfPoint2f approxCurve = new MatOfPoint2f();
        int barCount = 0;
        List<Rect> foundRectangles = new ArrayList<>();
        for (int i=0; i < contours.size() && barCount<=2; i++)
        {
            //Convert contours(i) from MatOfPoint to MatOfPoint2f
            MatOfPoint2f contour2f = new MatOfPoint2f( contours.get(i).toArray() );
            //Processing on mMOP2f1 which is in type MatOfPoint2f
            double approxDistance = Imgproc.arcLength(contour2f, true)*0.02;
            Imgproc.approxPolyDP(contour2f, approxCurve, approxDistance, true);

            //Convert back to MatOfPoint
            MatOfPoint points = new MatOfPoint( approxCurve.toArray() );

            // Get bounding rect of contour
            Rect rect = Imgproc.boundingRect(points);
            if(rect.height > 150 ) {
                // draw enclosing rectangle (all same color, but you could use variable i to make them unique)
                barCount++;
                //System.out.print("x: " + rect.x + " y: "+rect.y+ " h: " + rect.height + " w: " + rect.width + "\n");
                //Imgproc.rectangle(matToProcess, new Point(rect.x, rect.y), new Point(rect.x + rect.width, rect.y + rect.height), new Scalar(255, 0, 0, 255), 3);
                foundRectangles.add(rect);
            }

        }

        if(barCount == 2){

            if (Configuration.DOTAKEIMAGES) {

                MatToBufImg matToBufImg = new MatToBufImg();
                matToBufImg.setMatrix(red_hue_image, ".png");

                String folderBasePath = Application.getFolderBaseBath();

                File outputfile = new File(folderBasePath + "/" + "RedBar" + String.format("%03d", index) + ".png");
                try {
                    ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                    System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }

            return new Image(matToProcess, foundRectangles);
        }
        return null;
    }
}
