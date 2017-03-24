package ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper;

import org.opencv.core.Mat;
import org.opencv.core.Rect;

import java.util.List;

/**
 * Created by silvio on 21.03.17.
 */
public class Image {

    public Image(Mat martix, List<Rect> foundRectangles){
        this.martix = martix;
        this.foundRectangles = foundRectangles;
    }

    private Mat martix;
    private List<Rect> foundRectangles;

    public Mat getMartix() {
        return martix;
    }

    public void setMartix(Mat martix) {
        this.martix = martix;
    }

    public List<Rect> getFoundRectangles() {
        return foundRectangles;
    }

    public void setFoundRectangles(List<Rect> foundRectangles) {
        this.foundRectangles = foundRectangles;
    }
}
