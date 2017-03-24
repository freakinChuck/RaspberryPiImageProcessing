package ch.hslu.pren.hs16fs17.grp27.imageprocessing;

import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.Image;
import ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper.RomanCharacterPicture;

/**
 * Created by silvio on 21.03.17.
 */
public class RomanCharacterFinder {

    public int FindCharacter(Image image) {

        RomanCharacterPicture picture = new RomanCharacterPicture(image.getMartix(), image.getFoundRectangles());

        return picture.evaluatePicture();
    }
}
