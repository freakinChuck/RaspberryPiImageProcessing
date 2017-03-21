package ch.hslu.pren.hs16fs17.grp27;

import ch.hslu.pren.hs16fs17.grp27.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RedBarFinder;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.RomanCharacterFinder;
import ch.hslu.pren.hs16fs17.grp27.io.Camera;
import ch.hslu.pren.hs16fs17.grp27.playground.*;
import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import org.opencv.core.Core;
import org.opencv.core.Mat;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

/**
 * Created by silvio on 17.03.17.
 */
public class Application {
    public static void main(String[] args){

        Application.SetUpEnvironment();

        Camera sideCamera = new Camera(Configuration.SIDECAMERAINDEX);
        RedBarFinder redBarFinder = new RedBarFinder();
        RomanCharacterFinder romanCharacterFinder = new RomanCharacterFinder();

        List<Future<Integer>> processList = new ArrayList<>();

        int trys = 0;
        boolean redBarFound = false;
        while (processList.size() <= 10) {
            Mat capturedImage = sideCamera.Capture();
            Image foundImage = redBarFinder.FindRedDoubleBar(capturedImage);
            if(foundImage != null){

                Future<Integer> numberRecognitionFuture = CompletableFuture.supplyAsync(() -> foundImage)
                        .thenApply(img -> romanCharacterFinder.FindCharacter(img));

                processList.add(numberRecognitionFuture);

            }
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
