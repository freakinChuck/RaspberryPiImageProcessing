package ch.hslu.pren.hs16fs17.grp27.intercomm;

import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import com.pi4j.io.gpio.*;

/**
 * Created by silvio on 24.03.17.
 */
public class ArduinoCommunication {

    private final GpioPinDigitalOutput startSignalPin;
    private final GpioPinDigitalOutput bit1SignalPin;
    private final GpioPinDigitalOutput bit2ignalPin;
    private final GpioPinDigitalOutput bit3SignalPin;
    public ArduinoCommunication(){
        final GpioController gpio = GpioFactory.getInstance();

        startSignalPin = gpio.provisionDigitalOutputPin(Configuration.STARTSIGNALPIN, "StartPin", PinState.LOW);
        startSignalPin.setShutdownOptions(true, PinState.LOW);


        bit1SignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT1SIGNALPIN, "Bit1Pin", PinState.LOW);
        bit1SignalPin.setShutdownOptions(true, PinState.LOW);
        bit2ignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT2SIGNALPIN, "Bit2Pin", PinState.LOW);
        bit2ignalPin.setShutdownOptions(true, PinState.LOW);
        bit3SignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT3SIGNALPIN, "Bit3Pin", PinState.LOW);
        bit3SignalPin.setShutdownOptions(true, PinState.LOW);

    }


    public void SendStartSignal(){
        startSignalPin.setState(PinState.HIGH);
    }

    public void ClearPinData() {
        startSignalPin.setState(PinState.LOW);
        bit1SignalPin.setState(PinState.LOW);
        bit2ignalPin.setState(PinState.LOW);
        bit3SignalPin.setState(PinState.LOW);
    }

    public void SendNumber(int number){

        if (number == 6)
            number = 4;

        bit1SignalPin.setState(number % 2 == 1 ? PinState.HIGH : PinState.LOW);
        bit2ignalPin.setState(number % 4 >= 2 ? PinState.HIGH : PinState.LOW);
        bit3SignalPin.setState(number >= 4  ? PinState.HIGH : PinState.LOW);
    }
}
