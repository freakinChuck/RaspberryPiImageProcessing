package ch.hslu.pren.hs16fs17.grp27;

import ch.hslu.pren.hs16fs17.grp27.imageprocessing.GreenlightFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedBarFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedLightHeight;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RomanCharacterFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.MatToBufImg;
import ch.hslu.pren.hs16fs17.grp27.intercomm.GpioCommunication;
import ch.hslu.pren.hs16fs17.grp27.io.Camera;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Rect;
import java.sql.Timestamp;

import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

/**
 * Created by silvio on 17.03.17.
 */
public class Application {


    static Calendar calendar = Calendar.getInstance();
    static SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss");
    static String currentDateAsString = format1.format(calendar.getTime());
    public static String getFolderBaseBath(){
        return "/home/pi/Images/" + currentDateAsString;

    }
    static PrintStream  out;
    public static void main(String[] args) throws InterruptedException, IOException {

        out = System.out;

        String folderBasePath = getFolderBaseBath();

        if (Files.notExists(Paths.get(folderBasePath))) {
            new File(folderBasePath).mkdirs();
        }

        if (Configuration.DOPRINTTOFILE) {

            File file = new File(getFolderBaseBath() + "/Log.txt" );
            if (file.exists()) {
                file.delete();
                file.createNewFile();
            }
            out = new PrintStream(file);
        }

        try {

            GpioCommunication communication;

            if (Configuration.DOCOMMUNICATION && Configuration.DOWAITFORARDUINO) {
                out.println("Waiting for Arduino");

                communication = new GpioCommunication();
                communication.ClearPinData();

                communication.DisplayTilt();
                while (!communication.IsArduinoReady()) {
                    Thread.sleep(100);
                }
                communication.ClearPinData();
                communication.InitiateWaitState();

                out.println("Arduino Ready");

            }
            else if (Configuration.DOCOMMUNICATION) {
                communication = new GpioCommunication();
                communication.ClearPinData();
                communication.InitiateWaitState();

                out.println("ArduinoWait Deactivated");

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
            while (!frontCamera.Available() && count++ < 20) {
                Thread.sleep(250);
                out.println("Camera not Available");

            }

            if (!frontCamera.Available()) {
                out.println("TILT.....");
                if (Configuration.DOCOMMUNICATION)
                    communication.DisplayTilt();
                Thread.sleep(2000);

                out.println("Shutdown initiated....");
                if (Configuration.DOCOMMUNICATION)
                    communication.ClearPinData();
                return;
            }

            //Save Camera With and Height for further steps
            frontCameraWidth = frontCamera.Capture().width();
            frontCameraHeight = frontCamera.Capture().height();


            if (Configuration.DOCOMMUNICATION)
                communication.DisplayRedlightWait();

            Rect upperHalfOfImage = new Rect(0, 50 , frontCameraWidth *3 / 5, frontCameraHeight *3 /5);
            while (!redLightHeight.FindRedLightHeight(new Mat(frontCamera.Capture(), upperHalfOfImage))) {
                out.println("waiting for Redlight");
                try {
                    Thread.sleep(250);
                }
                catch (InterruptedException e) {
                }
            }


            out.println("Redlight found at: " + redLightHeight.getRedXPos());
            if (Configuration.DOCOMMUNICATION)
                communication.DisplayGreenlightWait();

            //Rect belowRedLight = new Rect(redLightHeight.getRedXPos(), 0, frontCameraWidth-redLightHeight.getRedXPos(),frontCameraHeight);
            Rect belowRedLight = new Rect(0, 0, redLightHeight.getRedXPos(), frontCameraWidth);
            long timesafty = System.currentTimeMillis() + 60000;
            //timesafty 30 sek green

            while (timesafty > System.currentTimeMillis() && !greenlightFinder.ImageContainsGreenLight(new Mat(frontCamera.Capture(), upperHalfOfImage))) {
                out.println("waiting for Greenlight " + timesafty + " < " +  System.currentTimeMillis());
                try {
                    Thread.sleep(250);
                }
                catch (InterruptedException e) {


                }
            }

            System.out.println("Green light Found");
            frontCamera.Shutdown();

            if (Configuration.DOCOMMUNICATION)
                communication.DisplayGo();

            if (Configuration.DOCOMMUNICATION)
                communication.SendStartSignal();

            System.out.println("Start Signal sent to Arduino");


            try {
                Thread.sleep(2000);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }

            Camera sideCamera = new Camera(Configuration.SIDECAMERAINDEX);

            out.println("Starting Number Recognition");
            List<Future<Integer>> processList = new ArrayList<>();
            List<Integer> intList = new ArrayList<>();

            boolean redBarFound = false;

            final int numberOfImagesToTake = 12;

            Image[] redBarImages = new Image[numberOfImagesToTake];

            while (redBarImages[0] == null) {
                Mat capturedImage = sideCamera.Capture();
                Image foundImage = redBarFinder.FindRedDoubleBar(capturedImage, 0);
                if (foundImage != null) {
                    out.println("Redbar found");
                    redBarImages[0] = foundImage;
                }
                System.gc();
            }

            Mat[] matrixesToEvaluate = new Mat[numberOfImagesToTake - 1];
            for (int i = 0; i < matrixesToEvaluate.length; i++) {
                out.println("Picture taken" + i);
                matrixesToEvaluate[i] = sideCamera.Capture();
            }

            if (Configuration.DOCOMMUNICATION)
                communication.DisplayRedbarFound();

            for (int i = 0; i < matrixesToEvaluate.length; i++) {
                redBarImages[i + 1] = redBarFinder.FindRedDoubleBar(matrixesToEvaluate[i], i);
                if (redBarImages[i + 1] != null) {
                    out.println("RedbarFound Picture" + i);
                }
                else {
                    out.println("RedbarFailed Picture" + i);
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


            for (Future<Integer> future : processList) {
                try {
                    out.println("Erkennte Zahl: " + future.get());
                }
                catch (Exception ex) {
                    ex.printStackTrace();
                }
            }


            Object[] objects = processList.stream().map(f -> {
                try {
                    int number = f.get();
                    number = number == 6 ? 4 : number;
                    return number;
                }
                catch (Exception exc) {
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
                if (maxNumbers < currentCount) {
                    maxKey = i;
                    maxNumbers = currentCount;
                }
            }
            if(maxNumbers == 0){
                maxKey =(int)(Math.random() * 5) +1; //If no nummbers found, get a random one
                out.println( "random number generated");
            }

            out.println("Number " + maxKey + " found");

            if (Configuration.DOCOMMUNICATION) {

                communication.DisplayNumber(maxKey);
                communication.SendNumber(maxKey);
            }

            out.println("Number " + maxKey + " sent to Arduino");


            PrintNumberInConsole(maxKey, maxNumbers, objects.length);

            if (Configuration.DOTAKEIMAGES) {
                for (int i = 0; i < matrixesToEvaluate.length; i++) {
                    Mat mat = matrixesToEvaluate[i];
                    MatToBufImg matToBufImg = new MatToBufImg();
                    matToBufImg.setMatrix(mat, ".png");

                    File outputfile = new File(folderBasePath + "/" + "Original" + String.format("%03d", i) + ".png");
                    try {
                        ImageIO.write(matToBufImg.getBufferedImage(), "png", outputfile);
                        out.println("Wrote File: " + outputfile.getAbsolutePath());

                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }

                }
                out.println("File write done");
            }

            Thread.sleep(60000);

        }
        catch (Exception exc) {

            if (Configuration.DOCOMMUNICATION) {
                GpioCommunication communication = new GpioCommunication();
                communication.DisplayTilt();

                Thread.sleep(5000);

                communication.SendNumber(3);
                communication.DisplayNumber(3);

            }

            File outputfile = new File(folderBasePath + "/" + "Error.txt");

            if (!outputfile.exists()) {
                outputfile.createNewFile();
            }

            out.println("***************************************************");

            out.print("Exception of Type: ");
            out.println(exc.getClass().getName());
            out.println(exc.getMessage());
            out.println();
            exc.printStackTrace(out);

            Thread.sleep(5000);


            throw exc;
        }
    }

    public static void SetUpEnvironment(){
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }

    public static void PrintNumberInConsole(int number, long numbersFound, long totalAmountOfPicturesTaken){
        switch(number){
            case 1: out.println( " __  \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|  | \n" +
                    "|__| \n" +
                    "     ");
                break;

            case 2: out.println( " __   __  \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|  | |  | \n" +
                    "|__| |__| \n" +
                    "          ");
                break;

            case 3: out.println( " __   __   __  \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|  | |  | |  | \n" +
                    "|__| |__| |__| \n" +
                    "               ");
                break;

            case 4: out.println(" __  ____    ____ \n" +
                    "|  | \\   \\  /   / \n" +
                    "|  |  \\   \\/   /  \n" +
                    "|  |   \\      /   \n" +
                    "|  |    \\    /    \n" +
                    "|__|     \\__/     \n" +
                    "                  ");
                break;

            case 5: out.println( "____    ____ \n" +
                    "\\   \\  /   / \n" +
                    " \\   \\/   /  \n" +
                    "  \\      /   \n" +
                    "   \\    /    \n" +
                    "    \\__/     \n" +
                    "             ");
                break;

            default: out.println("Couldn't find Character");
                break;
        }
        out.println("Precision: " + ((float)numbersFound/totalAmountOfPicturesTaken*100) + "%");
    }

}
