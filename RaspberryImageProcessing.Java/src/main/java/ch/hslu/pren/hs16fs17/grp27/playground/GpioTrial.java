package ch.hslu.pren.hs16fs17.grp27.playground;

import com.pi4j.io.gpio.*;

/**
 * Created by silvio on 19.03.17.
 */
public class GpioTrial {

    public static void FooBar(){
        try {
            final GpioController gpio = GpioFactory.getInstance();
            final GpioPinDigitalOutput outputPin = gpio.provisionDigitalOutputPin(RaspiPin.GPIO_01, "TrialOutputPin", PinState.LOW);

            Thread.sleep(5000);

            outputPin.setState(PinState.HIGH);
            Thread.sleep(5000);

            outputPin.setState(PinState.LOW);
            Thread.sleep(5000);

            outputPin.setState(PinState.HIGH);
            Thread.sleep(5000);


        }
        catch(InterruptedException exc) { }

    }

}
