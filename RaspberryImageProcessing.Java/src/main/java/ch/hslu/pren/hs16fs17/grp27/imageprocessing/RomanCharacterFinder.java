package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.RomanCharacterPicture;
import net.sourceforge.tess4j.ITesseract;
import net.sourceforge.tess4j.Tesseract;
import org.opencv.core.*;
import org.opencv.imgproc.Imgproc;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.FileOutputStream;

/**
 * Created by silvio on 21.03.17.
 */
public class RomanCharacterFinder {

    public int FindCharacter(Image image) {

        RomanCharacterPicture picture = new RomanCharacterPicture(image.getMartix(), image.getFoundRectangles());

        return picture.evaluatePicture();
    }
}
