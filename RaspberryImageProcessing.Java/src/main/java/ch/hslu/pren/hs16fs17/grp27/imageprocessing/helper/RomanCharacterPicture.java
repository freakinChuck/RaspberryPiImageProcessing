package ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper;


import ch.hslu.pren.hs16fs17.grp27.Application;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import net.sourceforge.tess4j.ITesseract;
import net.sourceforge.tess4j.Tesseract;
import org.opencv.core.*;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.List;

/**
 *
 * @author Tenga
 * @version 1.0
 */
public class RomanCharacterPicture {
    private Mat webcam_image;
    private RectangleCoordinates leftRectangle;
    private RectangleCoordinates rightRectangle;
    private TesseractInstance tessIns = new TesseractInstance();
    private ITesseract tesseract = tessIns.getSingletonTesseract();

    private int index;

    public RomanCharacterPicture(Mat webcam_image, List<Rect> foundRectangles, int index){
        this.webcam_image = webcam_image;
        this.index = index;
        if(foundRectangles.get(0).x < foundRectangles.get(1).x){
            leftRectangle = new RectangleCoordinates(foundRectangles.get(0).x + foundRectangles.get(0).width, foundRectangles.get(0).y);
            rightRectangle = new RectangleCoordinates(foundRectangles.get(1).x,foundRectangles.get(1).y + foundRectangles.get(1).height);
        }else{
            leftRectangle = new RectangleCoordinates(foundRectangles.get(1).x + foundRectangles.get(1).width, foundRectangles.get(1).y);
            rightRectangle = new RectangleCoordinates(foundRectangles.get(0).x,foundRectangles.get(0).y + foundRectangles.get(0).height);
        }
    }

    public int evaluatePicture(){
        try{

            Rect rect = new Rect(leftRectangle.getxPos(),leftRectangle.getyPos(),(rightRectangle.getxPos() - leftRectangle.getxPos()),(rightRectangle.getyPos() - leftRectangle.getyPos()));
            Mat subImageMat = webcam_image.submat(rect);

            Mat hsv_image = new Mat();
            Imgproc.cvtColor(subImageMat, hsv_image, Imgproc.COLOR_BGR2HSV);
            //--------------Covert picture to Black and With. // not a grayscale
            Mat lower_black_hue_range = new Mat();
            Mat upper_black_hue_range = new Mat();

            Core.inRange(hsv_image, new Scalar(0,0,0), new Scalar(180, 255, 50), lower_black_hue_range);
            Core.inRange(hsv_image, new Scalar(0, 0, 20), new Scalar(180, 255, 100), upper_black_hue_range);

            Mat black_hue_image = new Mat();
            Core.addWeighted(lower_black_hue_range, 1.0, upper_black_hue_range, 1.0, 0.0, black_hue_image);
            Imgproc.GaussianBlur(black_hue_image, black_hue_image, new Size(9, 9), 2, 2);

            //Invert black to white
            Mat white_hue_image = new Mat();
            Core.bitwise_not(black_hue_image, white_hue_image);
            MatToBufImg whiteImageBuff = new MatToBufImg();

            whiteImageBuff.setMatrix(white_hue_image, ".jpg");
            BufferedImage inputOCRimage = whiteImageBuff.getBufferedImage();
            String result = tesseract.doOCR(inputOCRimage).trim();
            //--> Convert the OCR string to roman chars.
            boolean halfV = false;
            int forEachCharI = 0;
            int forEachCharV = 0;
            for (char ch: result.toCharArray()) {
                switch (ch) {
                    case 'i':
                    case 'I':
                    case 'l':
                    case 'L':
                    case '1':
                    case 'T':
                    case 't':
                    case '!':
                    case '{':
                    case '}':
                    case 'f':
                    case '[':
                    case ']':
                    case '|':
                        forEachCharI++;
                        break;
                    case 'H':
                    case '"':
                    case '”':
                    case 'h':
                    case 'u':
                        forEachCharI += 2;
                        break;
                    case 'v':
                    case 'V':
                        forEachCharV = 5;
                        break;
                    case 'W':
                    case 'w':
                        forEachCharV = 5;
                        forEachCharI = 1;
                        break;
                    case '\\':
                        forEachCharI++;
                        halfV = true;
                        break;
                    case '/':
                        if(halfV){
                            forEachCharV = 5;
                            if(forEachCharI == 1){
                                forEachCharI = 0;
                            }else{
                                forEachCharI=1;
                            }
                        }else
                            forEachCharI++;
                        break;
                    default:
                        break;
                }
            }
            int counter = 0;
            counter = (forEachCharI+forEachCharV);
            //System.out.println("All chars:" + result + " --> " + counter + "\n");
            if(Configuration.DOTAKEIMAGES) {
                MatToBufImg matToBufImg = new MatToBufImg();
                matToBufImg.setMatrix(white_hue_image, ".png");

                String folderBasePath = Application.getFolderBaseBath();

                File outputfile = new File(folderBasePath + "/" + "Number" + String.format("%03d", index) + ".png");
                try {
                    ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                    System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return(counter);
        }catch(Exception ex){
            System.err.println(ex.toString());
            return 0;
        }

    }

}

