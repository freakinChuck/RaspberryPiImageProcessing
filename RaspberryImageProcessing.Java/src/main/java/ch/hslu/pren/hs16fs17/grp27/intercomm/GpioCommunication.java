package ch.hslu.pren.hs16fs17.grp27.intercomm;

import ch.hslu.pren.hs16fs17.grp27.settings.Configuration;
import com.pi4j.io.gpio.*;
import com.pi4j.wiringpi.Gpio;

/**
 * Created by silvio on 24.03.17.
 */
public class GpioCommunication {

    private final GpioPinDigitalOutput startSignalPin;
    private final GpioPinDigitalOutput bit1SignalPin;
    private final GpioPinDigitalOutput bit2ignalPin;
    private final GpioPinDigitalOutput bit3SignalPin;

    private final GpioPinDigitalOutput displayDotPin;
    private final GpioPinDigitalOutput displayTopMidPin;
    private final GpioPinDigitalOutput displayTopLeftPin;
    private final GpioPinDigitalOutput displayTopRightPin;
    private final GpioPinDigitalOutput displayMidPin;
    private final GpioPinDigitalOutput displayBottomMidPin;
    private final GpioPinDigitalOutput displayBottomLeftPin;
    private final GpioPinDigitalOutput displayBottomRightPin;

    private final GpioPinDigitalInput arduinoReadyPin;


    public GpioCommunication(){
        final GpioController gpio = GpioFactory.getInstance();

        startSignalPin = gpio.provisionDigitalOutputPin(Configuration.STARTSIGNALPIN, "StartPin", PinState.LOW);
        startSignalPin.setShutdownOptions(true, PinState.LOW);


        bit1SignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT1SIGNALPIN, "Bit1Pin", PinState.LOW);
        bit1SignalPin.setShutdownOptions(true, PinState.LOW);
        bit2ignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT2SIGNALPIN, "Bit2Pin", PinState.LOW);
        bit2ignalPin.setShutdownOptions(true, PinState.LOW);
        bit3SignalPin = gpio.provisionDigitalOutputPin(Configuration.BIT3SIGNALPIN, "Bit3Pin", PinState.LOW);
        bit3SignalPin.setShutdownOptions(true, PinState.LOW);

        displayDotPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYDOTPIN);
        displayDotPin.setShutdownOptions(true, PinState.LOW);

        displayTopMidPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYTOPMIDPIN);
        displayTopMidPin.setShutdownOptions(true, PinState.LOW);
        displayTopLeftPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYTOPLEFTPIN);
        displayTopLeftPin.setShutdownOptions(true, PinState.LOW);
        displayTopRightPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYTOPRIGHTPIN);
        displayTopRightPin.setShutdownOptions(true, PinState.LOW);

        displayMidPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYMIDPIN);
        displayMidPin.setShutdownOptions(true, PinState.LOW);


        displayBottomMidPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYBOTTOMMIDPIN);
        displayBottomMidPin.setShutdownOptions(true, PinState.LOW);
        displayBottomLeftPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYBOTTOMLEFTPIN);
        displayBottomLeftPin.setShutdownOptions(true, PinState.LOW);
        displayBottomRightPin = gpio.provisionDigitalOutputPin(Configuration.DISPLAYBOTTOMRIGHTPIN);
        displayBottomRightPin.setShutdownOptions(true, PinState.LOW);

        arduinoReadyPin = gpio.provisionDigitalInputPin(Configuration.READYSIGNALPIN);
    }


    public void SendStartSignal(){
        startSignalPin.setState(PinState.HIGH);
    }

    public void ClearPinData() {
        startSignalPin.setState(PinState.LOW);
        bit1SignalPin.setState(PinState.LOW);
        bit2ignalPin.setState(PinState.LOW);
        bit3SignalPin.setState(PinState.LOW);

        displayDotPin.setState(PinState.HIGH);

        displayTopMidPin.setState(PinState.HIGH);
        displayTopLeftPin.setState(PinState.HIGH);
        displayTopRightPin.setState(PinState.HIGH);

        displayMidPin.setState(PinState.HIGH);

        displayBottomMidPin.setState(PinState.HIGH);
        displayBottomLeftPin.setState(PinState.HIGH);
        displayBottomRightPin.setState(PinState.HIGH);


    }

    public boolean IsArduinoReady() {
        if (!Configuration.DOCOMMUNICATION)
            return true;

        return arduinoReadyPin.isHigh();
    }

    public void SendNumber(int number){

        if (number == 6)
            number = 4;

        bit1SignalPin.setState(number % 2 == 1 ? PinState.HIGH : PinState.LOW);
        bit2ignalPin.setState(number % 4 >= 2 ? PinState.HIGH : PinState.LOW);
        bit3SignalPin.setState(number >= 4  ? PinState.HIGH : PinState.LOW);
    }

    public void InitiateWaitState(){
        displayDotPin.low();
    }
    public void DisplayNumber(int number){
        if (number == 6)
            number = 4;

        switch (number){
            case 1:
                displayDotPin.high();

                displayTopMidPin.high();
                displayTopLeftPin.high();
                displayTopRightPin.low();

                displayMidPin.high();

                displayBottomMidPin.high();
                displayBottomLeftPin.high();
                displayBottomRightPin.low();
                break;
            case 2:
                displayDotPin.high();

                displayTopMidPin.low();
                displayTopLeftPin.high();
                displayTopRightPin.low();

                displayMidPin.low();

                displayBottomMidPin.low();
                displayBottomLeftPin.low();
                displayBottomRightPin.high();
                break;
            case 3:
                displayDotPin.high();

                displayTopMidPin.low();
                displayTopLeftPin.high();
                displayTopRightPin.low();

                displayMidPin.low();

                displayBottomMidPin.low();
                displayBottomLeftPin.high();
                displayBottomRightPin.low();
                break;
            case 4:
                displayDotPin.high();

                displayTopMidPin.high();
                displayTopLeftPin.low();
                displayTopRightPin.low();

                displayMidPin.low();

                displayBottomMidPin.high();
                displayBottomLeftPin.high();
                displayBottomRightPin.low();
                break;
            case 5:
                displayDotPin.high();

                displayTopMidPin.low();
                displayTopLeftPin.low();
                displayTopRightPin.high();

                displayMidPin.low();

                displayBottomMidPin.low();
                displayBottomLeftPin.high();
                displayBottomRightPin.low();
                break;
        }

    }
    public void DisplayTilt(){
        displayDotPin.low();

        displayTopMidPin.low();
        displayTopLeftPin.low();
        displayTopRightPin.low();

        displayMidPin.low();

        displayBottomMidPin.low();
        displayBottomLeftPin.low();
        displayBottomRightPin.low();
    }
    public void DisplayZero(){
        displayDotPin.high();

        displayTopMidPin.low();
        displayTopLeftPin.low();
        displayTopRightPin.low();

        displayMidPin.high();

        displayBottomMidPin.low();
        displayBottomLeftPin.low();
        displayBottomRightPin.low();
    }
}
