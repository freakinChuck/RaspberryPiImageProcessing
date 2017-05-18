package ch.hslu.pren.hs16fs17.grp27.settings;

import com.pi4j.io.gpio.Pin;
import com.pi4j.io.gpio.RaspiPin;

/**
 * Created by silvio on 21.03.17.
 */
public class Configuration {

    public static final int SIDECAMERAINDEX = 0;
    public static final int FRONTCAMERAINDEX = 0;

    public static final Pin STARTSIGNALPIN = RaspiPin.GPIO_01;


    public static final Pin BIT1SIGNALPIN = RaspiPin.GPIO_02;
    public static final Pin BIT2SIGNALPIN = RaspiPin.GPIO_03;
    public static final Pin BIT3SIGNALPIN = RaspiPin.GPIO_04;

    public static final Pin DISPLAYDOTPIN = RaspiPin.GPIO_27;
    public static final Pin DISPLAYTOPMIDPIN = RaspiPin.GPIO_21;
    public static final Pin DISPLAYTOPLEFTPIN = RaspiPin.GPIO_28;
    public static final Pin DISPLAYTOPRIGHTPIN = RaspiPin.GPIO_22;
    public static final Pin DISPLAYMIDPIN = RaspiPin.GPIO_29;
    public static final Pin DISPLAYBOTTOMMIDPIN = RaspiPin.GPIO_24;
    public static final Pin DISPLAYBOTTOMLEFTPIN = RaspiPin.GPIO_25;
    public static final Pin DISPLAYBOTTOMRIGHTPIN = RaspiPin.GPIO_23;

    public static final boolean DOTAKEIMAGES = true;

}
