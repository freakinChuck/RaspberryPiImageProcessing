package ch.hslu.pren.hs16fs17.grp27;

import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.GreenlightFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedBarFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RomanCharacterFinder;
import ch.hslu.pren.hs16fs17.grp27.intercomm.ArduinoCommunication;
import ch.hslu.pren.hs16fs17.grp27.io.Camera;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.Core;
import org.opencv.core.Mat;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Executor;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * Created by silvio on 17.03.17.
 */
public class Application {
    public static void main(String[] args){

        Application.SetUpEnvironment();

        Camera frontCamera = new Camera(Configuration.FRONTCAMERAINDEX);
        GreenlightFinder greenlightFinder = new GreenlightFinder();
        RedBarFinder redBarFinder = new RedBarFinder();
        RomanCharacterFinder romanCharacterFinder = new RomanCharacterFinder();
        ArduinoCommunication communication = new ArduinoCommunication();

        //Executor executor = new ThreadPoolExecutor(3, 100, );

        while (!greenlightFinder.ImageContainsGreenLight(frontCamera.Capture())){
            System.out.println("waiting for Greenlight");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {


            }
        }

        System.out.println("Green light Found");
        frontCamera.Shutdown();

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

        int trys = 0;
        boolean redBarFound = false;
        while (intList.size() <= 10) {
            Mat capturedImage = sideCamera.Capture();

            Future<Integer> numberRecognitionFuture = CompletableFuture.completedFuture(capturedImage)
                    .thenApplyAsync(img -> {
                        Image foundImage = redBarFinder.FindRedDoubleBar(img);
                        if(foundImage != null) {
                            System.out.println("Redbar found");
                            return foundImage;
                        }
                        return null;
                    })
                    .thenApplyAsync(img -> img != null ? romanCharacterFinder.FindCharacter(img) : 0)
                    .thenApplyAsync(i -> {
                        if (i > 0)
                            intList.add(i);
                        return i;
                    });

            processList.add(numberRecognitionFuture);

            trys++;
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

        System.out.println("Hallo Welt");

    }

    public static void SetUpEnvironment(){
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }

}
