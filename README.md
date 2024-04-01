# EaBase
Basic class libary for Arduino, ESP8266 and ESP32

This libary encaptulate then most commom features that I use when coding with microproessors.

# Demo application (Arduino UNO)

	/* Simple program that reads from pin 5 and set pin 6 */

    #include "EaBase.h"
    #include "EaBase_Arduino.h"
    #include "EaBase_Input.h"
    #include "EaBase_Output.h"

    EABase eaBase = EABase();
    EABase_ArduinoUNO eaBaseBoard = EABase_ArduinoUNO(&eaBase);
    EABase_Input eaBase_Switch = EABase_Input(&eaBaseBoard, 5, INPUT_PULLUP, true, 100); // pullup with invert
    EABase_Output eaBase_LED = EABase_Output(&eaBaseBoard, 6); 

    void setup()
    {
      eaBase.setup();
    }

    void loop()
    {
      eaBase.loop();
      bool value = eaBase_Switch.get();
      eaBase_LED.set(value);
    }

# Demo application (ESP8266 with MCP23017)

	/* same reading input from pin 9 on MCP23017 (device id 4) and set pin 5 and ESP8266) */
	
	#include <Wire.h>
    #include "EaBase.h"
    #include "EaBase_NodeMCUv10.h"
	#include "EaBase_MCP23017.h"
    #include "EaBase_Input.h"
    #include "EaBase_Output.h"

    EABase eaBase = EABase();
    EABase_NodeMCUv10 eaBaseBoard = EABase_NodeMCUv10(&eaBase);
	EaBase_MCP23017 eaMcp23017 = EaBase_MCP23017(EABase_MCP23017(&eaBase, 4);
    EABase_Input eaBase_Switch = EABase_Input(&EaBase_MCP23017, 9, INPUT_PULLUP, true, 100); // pullup with invert
    EABase_Output eaBase_LED = EABase_Output(&eaBaseBoard, 5); 

    void setup()
    {
      eaBase.setup();
    }

    void loop()
    {
	  Wire.begin(D1, D2);
	  eaBaseBoard.setIgnoreI2C(1, 2);
      eaBase.loop();
      bool value = eaBase_Switch.get();
      eaBase_LED.set(value);
    }

# Basic variables

uint64_t EABase_PINDATA
uint8_t EABase_PINNO

# EABase (EaBase.h)
Basic class where all objects are added, this class contains the basic setup and run methon

* void add(EABase_Object *obj)
* void setup(), call all setup methods in the EABase object structure
* void loop(), call all loop methods in the EABase object structure

Object contains pointers for list of EA_Base_Objects (classListFirst,classListNext)

# EABase_Object (EaBase_Object.h)
Basic abstract class for implementing object structure

* EABase_Object(EABase *eabase)
* virtual void setup()
* virtual void loop()

## EaBase_Chip.h

## EABase_Chip
Basic structure of a chip, extended from EABase_Object
* EaBase_Chip(EABase *eabase)

## EABase_ChipIO
Basic abstract structure of a IO chip, extended from EABase_Chip, EABase_Object

* EABase_ChipIO(EABase *eabase)
* virtual uint8_t getPinNo(uint8_t pin)
* void setIgnore(EABase_PINNO pin)
* void setIgnores(EABase_PINNO *pins)
* virtual void setMode(EABase_PINNO pin, int value, bool invert)
* virtual void setMode(EABase_PINNO *pins, int value, bool invert)
* virtual void pinWrite(EABase_PINNO pin, bool value)
* virtual void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* virtual bool pinRead(EABase_PINNO pin)
* virtual EABase_PINDATA pinRead(uint8_t *pins)
* virtual uint8_t maxPins()
* EABase_PINDATA setPin(EABase_PINDATA input, uint8_t pin, bool value)
* bool getPin(EABase_PINDATA input, int pin)
* EABase_PINDATA Ignore = 0;
* EABase_PINDATA Direction = 0;
* EABase_PINDATA PullUp = 0;
* EABase_PINDATA Output = 0;
* EABase_PINDATA Invert = 0;


### setMode values
	INPUT
	INPUT_PULLUP


## EABase_ArduinoUNO (EaBase_ArduinoUNO.h)
Implements Arduino UNO board (Can be used for mini as well), extended from EABase_ChipIO

* EABase_ArduinoUNO(EABase *eabase)
* void setIgnoreI2C()
* void setIgnoreSPI()
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_NodeMCUv10.h
Implements ESP8266 ModeMCU v1.0, extended from EABase_ChipIO

* uint8_t getPinNo(uint8_t pin)
* EABase_NodeMCUv10(EABase *eabase)
* void setIgnoreI2C()
* void setIgnoreSPI()
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_ESP32Wroom.h
Implements ESP32 Wroom board (v4), extended from EABase_ChipIO

* uint8_t getPinNo(uint8_t pin)
* EABase_NodeMCUv10(EABase *eabase)
* void setIgnoreI2C()
* void setIgnoreSPI()
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_MCP23017.h
Implements I2C chip MCP23017, extended from EABase_ChipIO

* EABase_MCP23017(EABase *eabase, uint8_t chipAddr)
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_PCB8574.h
Implements I2C check PCB8574, extended from EABase_ChipIO

* EABase_PCB8574(EABase *eabase, uint8_t chipAddr)
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_Rotary_KY040.h
Implements KY040 rotary encoder, extended from public EABase_Object
* void setup() {}
* void loop() {}
* EABase_Rotary(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinData, int minValue, int maxValue, int start, bool rollOver)
* ~EABase_Rotary();
* void Reset(int minValue, int maxValue, int start, bool rollOver);
* bool HasChanged()
* int Get()

Public but secundary informations
* bool _rollOver = 0;
* volatile bool _hasChanges = false;
* uint8_t _pinClock;
* uint8_t _pinData;
* volatile int _data = 0;
* volatile int _minValue = 0;
* volatile int _maxValue = 0;
* volatile int8_t _oldState;
* volatile long _position;

## EABase_TM1637_4Digit.h
Implements TM1637 4 digit display, extended from EABase_Object

* EABase_TM1637(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinDIO)
* void setup() {}
* void loop() {}
* void SetParm(uint8_t brightness = EABase_TM1637_BRIGHT_TYPICAL, uint8_t SetData = 0x40, uint8_t SetAddr = 0xc0)
* void ClearDisplay()
* void Set(int value)
* void BlinkDigit(unsigned long value)
* void BlinkPoint(unsigned long value)

## EABase_Output.h
Implements a bit for digital output, extended from EABase_Object

* EABase_Output(EABase_ChipIO *Chip, EABase_PINDATA pin)
* void setup() {}
* void loop() {}
* void set(bool value)
* void get()

## EABase_OutputBlink.h
Implements a bit for a blinking digital output (Not PWM!!), extended from EABase_Object

* EABase_OutputBlink(EABase_ChipIO *Chip, EABase_PINDATA pin)
* void set(unsigned long value)
* void setup()
* void loop() {}

## EABase_OutputDblBlink.h
Implements two bits for a blinking digital output (Not PWM!!), extended from EABase_Object

* EABase_OutputDblBlink(EABase_ChipIO *Chip, EABase_PINDATA pin1, EABase_PINDATA pin2)
* void setTimer(unsigned long value)
* void setState(uint8_t state)
* void setup() {}
* void loop() {}

## EABase_OutputList.h
Combine multiple bits as a hex output, extended from EABase_Object

* EABase_OutputList(EABase_ChipIO *Chip, EABase_PINNO *pins)
* void set(EABase_PINDATA value)
* EABase_PINDATA get()
* void setup() {}
* void loop() {}

## EABase_Input.h
Implements a bit for digital input, extended from EABase_Object

* EABase_Input(EABase_ChipIO *Chip, EABase_PINNO pin, bool pullup = false, bool invert = false, uint16_t prelTime = 0)
* void setPrelTime(uint16_t prelTime)
* void setup()
* bool get()

## EABase_InputList.h
Implements multiple pin read, extended EABase_OutputList

* EABase_InputList(EABase_ChipIO *Chip, EABase_PINNO *pins, bool pullup = false, bool invert = false, uint16_t prelTime = 0)
* void setPrelTime(uint16_t prelTime)
* void setup()
* EABase_PINDATA get()

## EABase_KeyPad.h
Implements a keypad ei. 4x4 keypad, extends EABase_Object

* EABase_KeyPad(EABase_InputList *kbInput, EABase_OutputList *kbOutput, uint8_t *keyPad)
* bool hasValue()
* uint8_t value()
* void setup() {}
* void loop() {}
