package ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper;

import net.sourceforge.tess4j.ITesseract;
import net.sourceforge.tess4j.Tesseract;

/**
 * Created by tenga on 12.05.2017.
 */
public class TesseractInstance {

    private  static TesseractInstance instance = null;

    public ITesseract getSingletonTesseract() {
        return singletonTesseract;
    }

    private  ITesseract singletonTesseract = null;

    protected TesseractInstance(){
        singletonTesseract = new Tesseract();
        singletonTesseract.setDatapath(System.getenv("TESSDATA_PREFIX"));
        singletonTesseract.setTessVariable("tessedit_char_whitelist", "IV");
    }



    public static TesseractInstance getInstance(){
        if(instance == null){
            instance = new TesseractInstance();
        }
        return instance;

    }
}
