package ch.hslu.pren.hs16fs17.grp27.intercomm;

import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import com.pi4j.io.gpio.*;

/**
 * Created by silvio on 24.03.17.
 */
public class ArduinoCommunication {

    public void SendStartSignal(){
        final GpioController gpio = GpioFactory.getInstance();

        // provision gpio pin #01 as an output pin and turn on
        final GpioPinDigitalOutput pin = gpio.provisionDigitalOutputPin(Configuration.STARTSIGNALPIN, "StartPin", PinState.HIGH);
        // set shutdown state for this pin
        pin.setShutdownOptions(true, PinState.LOW);
    }

}
