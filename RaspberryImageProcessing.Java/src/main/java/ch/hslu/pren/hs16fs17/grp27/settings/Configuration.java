package ch.hslu.pren.hs16fs17.grp27.settings;

import com.pi4j.io.gpio.Pin;
import com.pi4j.io.gpio.RaspiPin;

/**
 * Created by silvio on 21.03.17.
 */
public class Configuration {

    public static final int SIDECAMERAINDEX = 1;
    public static final int FRONTCAMERAINDEX = 0;

    public static final Pin STARTSIGNALPIN = RaspiPin.GPIO_01;


    public static final Pin BIT1SIGNALPIN = RaspiPin.GPIO_02;
    public static final Pin BIT2SIGNALPIN = RaspiPin.GPIO_03;
    public static final Pin BIT3SIGNALPIN = RaspiPin.GPIO_04;

}
