package ch.hslu.pren.hs16fs17.grp27;

import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.GreenlightFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedLightHeight;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedBarFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RomanCharacterFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import ch.hslu.pren.hs16fs17.grp27.intercomm.GpioCommunication;
import ch.hslu.pren.hs16fs17.grp27.io.Camera;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Rect;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

/**
 * Created by silvio on 17.03.17.
 */
public class Application {


    static Calendar calendar = Calendar.getInstance();
    static String currentDateAsString = String.format("%tY%te%td%tl%tm", calendar, calendar, calendar, calendar, calendar);
    public static String getFolderBaseBath(){
        return "/home/pi/Images/" + currentDateAsString;

    }

    public static void main(String[] args) throws InterruptedException {

        GpioCommunication communication;

        if (Configuration.DOCOMMUNICATION && Configuration.DOWAITFORARDUINO) {
            System.out.println("Waiting for Arduino");

            communication = new GpioCommunication();
            communication.ClearPinData();

            communication.DisplayTilt();
            while (!communication.IsArduinoReady()){
                Thread.sleep(100);
            }
            communication.ClearPinData();
            communication.InitiateWaitState();

            System.out.println("Arduino Ready");

        }
        else if (Configuration.DOCOMMUNICATION) {
            communication = new GpioCommunication();
            communication.ClearPinData();
            communication.InitiateWaitState();

            System.out.println("ArduinoWait Deactivated");

        }


        Application.SetUpEnvironment();

        Camera frontCamera = new Camera(Configuration.FRONTCAMERAINDEX);
        RedLightHeight redLightHeight = new RedLightHeight();
        GreenlightFinder greenlightFinder = new GreenlightFinder();
        RedBarFinder redBarFinder = new RedBarFinder();
        RomanCharacterFinder romanCharacterFinder = new RomanCharacterFinder();
        int frontCameraWidth;
        int frontCameraHeight;

        int count = 0;
        while (!frontCamera.Available() && count++ < 20){
            Thread.sleep(250);
            System.out.println("Camera not Available");

        }

        if (!frontCamera.Available()){
            System.out.println("TILT.....");
            if (Configuration.DOCOMMUNICATION)
                communication.DisplayTilt();
            Thread.sleep(2000);

            System.out.println("Shutdown initiated....");
            if (Configuration.DOCOMMUNICATION)
                communication.ClearPinData();
            return;
        }

        //Save Camera With and Height for further steps
        frontCameraWidth = frontCamera.Capture().width();
        frontCameraHeight = frontCamera.Capture().height();

        String folderBasePath = getFolderBaseBath();
        if (Files.notExists(Paths.get(folderBasePath))) {
            new File(folderBasePath).mkdirs();
        }

        Rect upperHalfOfImage = new Rect(0,frontCameraHeight/3,frontCameraWidth,frontCameraHeight/3*2);
        while(!redLightHeight.FindRedLightHeight(new Mat(frontCamera.Capture(),upperHalfOfImage))){
            System.out.println("waiting for Redlight");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
            }
        }

        System.out.println("Redlight found at: " + redLightHeight.getRedXPos());

        //Rect belowRedLight = new Rect(redLightHeight.getRedXPos(), 0, frontCameraWidth-redLightHeight.getRedXPos(),frontCameraHeight);
        Rect belowRedLight = new Rect(0,0,redLightHeight.getRedXPos(),frontCameraWidth);

        while (!greenlightFinder.ImageContainsGreenLight(frontCamera.Capture())){
            System.out.println("waiting for Greenlight");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {


            }
        }
        
        System.out.println("Green light Found");
        frontCamera.Shutdown();

        if (Configuration.DOCOMMUNICATION)
            communication.SendStartSignal();

        System.out.println("Start Signal sent to Arduino");



        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Camera sideCamera = new Camera(Configuration.SIDECAMERAINDEX);

        System.out.println("Starting Number Recognition");
        List<Future<Integer>> processList = new ArrayList<>();
        List<Integer> intList = new ArrayList<>();

        boolean redBarFound = false;

        final int numberOfImagesToTake = 12;

        Image[] redBarImages = new Image[numberOfImagesToTake];

        while (redBarImages[0] == null) {
            Mat capturedImage = sideCamera.Capture();
            Image foundImage = redBarFinder.FindRedDoubleBar(capturedImage, 0);
            if (foundImage != null) {
                System.out.println("Redbar found");
                redBarImages[0] = foundImage;
            }
            System.gc();
        }

        Mat[] matrixesToEvaluate = new Mat[numberOfImagesToTake -1];
        for (int i = 0; i < matrixesToEvaluate.length; i++) {
            System.out.println("Picture taken" + i);
            matrixesToEvaluate[i] = sideCamera.Capture();
        }

        for (int i = 0; i < matrixesToEvaluate.length; i++) {
            redBarImages[i+1] = redBarFinder.FindRedDoubleBar(matrixesToEvaluate[i], i);
            if (redBarImages[i+1] != null) {
                System.out.println("RedbarFound Picture" + i);
            }
            else {
                System.out.println("RedbarFailed Picture" + i);
            }
        }

        for (int x = 0; x < numberOfImagesToTake; x++) {

            final int index = x;
            Future<Integer> numberRecognitionFuture = CompletableFuture.completedFuture(redBarImages[x])
                    .thenApplyAsync(img -> img != null ? romanCharacterFinder.FindCharacter(img, index) : 0)
                    .thenApplyAsync(i -> {
                        if (i > 0)
                            intList.add(i);
                        return i;
                    });

            processList.add(numberRecognitionFuture);
        }


        for (Future<Integer> future:
                processList){
            try {
                System.out.println("Erkennte Zahl: " + future.get());
            }
            catch(Exception ex){
                ex.printStackTrace();
            }
        }


        Object[] objects =  processList.stream().map(f -> {
            try {
                int number = f.get();
                number  = number == 6 ? 4 : number;
                return number;
            }
            catch (Exception exc)
            {
                exc.printStackTrace();
                return 0;
            }
        }).toArray();

        Map<Integer, Long> numbers = new HashMap<>();
        for (int i = 1; i <= 5; i++) {
            final int currentEvaluationNumber = i;
            long numberCount = Arrays.stream(objects).filter(o -> (int) o == currentEvaluationNumber).count();
            numbers.put(currentEvaluationNumber, numberCount);
        }

        int maxKey = 1;
        long maxNumbers = numbers.get(1);
        for (int i = 2; i <= 5; i++) {
            long currentCount = numbers.get(i);
            if (maxNumbers < currentCount){
                maxKey = i;
                maxNumbers = currentCount;
            }
        }


        System.out.println("Number "+ maxKey + " found");

        if (Configuration.DOCOMMUNICATION) {

            communication.DisplayNumber(maxKey);
            communication.SendNumber(maxKey);
        }

        System.out.println("Number " + maxKey + " sent to Arduino");


        PrintNumberInConsole(maxKey, maxNumbers, objects.length);


        if (Configuration.DOTAKEIMAGES) {
            for (int i = 0; i < matrixesToEvaluate.length; i++) {
                Mat mat = matrixesToEvaluate[i];
                MatToBufImg matToBufImg = new MatToBufImg();
                matToBufImg.setMatrix(mat, ".png");

                File outputfile = new File(folderBasePath + "/" + "Original" + String.format("%03d", i)+ ".png");
                try {
                    ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                    System.out.println("Wrote File: " + outputfile.getAbsolutePath());

                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
            System.out.println("File write done");
        }

    }

    public static void SetUpEnvironment(){
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }

    public static void PrintNumberInConsole(int number, long numbersFound, long totalAmountOfPicturesTaken){
        switch(number){
            case 1: System.out.println( " __  \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|__| \n" +
                    "     ");
                break;

            case 2: System.out.println( " __   __  \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|__| |__| \n" +
                    "          ");
                break;

            case 3: System.out.println( " __   __   __  \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|__| |__| |__| \n" +
                    "               ");
                break;

            case 4: System.out.println(" __  ____    ____ \n" +
                    "|  | \\   \\  /   / \n" +
                    "|  |  \\   \\/   /  \n" +
                    "|  |   \\      /   \n" +
                    "|  |    \\    /    \n" +
                    "|__|     \\__/     \n" +
                    "                  ");
                break;

            case 5: System.out.println( "____    ____ \n" +
                    "\\   \\  /   / \n" +
                    " \\   \\/   /  \n" +
                    "  \\      /   \n" +
                    "   \\    /    \n" +
                    "    \\__/     \n" +
                    "             ");
                break;

            default: System.out.println("Couldn't find Character");
                break;
        }
        System.out.println("Precision: " + ((float)numbersFound/totalAmountOfPicturesTaken*100) + "%");
    }

}
