# EaBase
Basic class libary for Arduino UNO, ESP8266 and ESP32

This libary encaptulate then most commom features that I use when coding with microproessors.

# Installation

Clone repositry to a folder in you current libary folder

    git clone https://github.com/nemeier/EaBase.git

# Demo application (Arduino UNO)

    /* Simple program that reads from pin 5 and set pin 6 */

    #include "EaBase.h"
    #include "EaBase_Arduino.h"
    #include "EaBase_Input.h"
    #include "EaBase_Output.h"

    EABase eaBase = EABase();
    EABase_ArduinoUNO eaBaseBoard = EABase_ArduinoUNO(&eaBase);
    EABase_Input eaBase_Switch = EABase_Input(&eaBaseBoard, 5, true, true, 100); // pullup with invert
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
    EaBase_MCP23017 eaMcp23017 = EABase_MCP23017(&eaBase, 4);
    EABase_Input eaBase_Switch = EABase_Input(&EaBase_MCP23017, 9, true, true, 100); // pullup with invert
    EABase_Output eaBase_LED = EABase_Output(&eaBaseBoard, 5);

    void setup()
    {
      Wire.begin(D1, D2);
      eaBaseBoard.setIgnoreI2C(1, 2);
      eaBase.setup();
    }

    void loop()
    {
      eaBase.loop();
      bool value = eaBase_Switch.get();
      eaBase_LED.set(value);
    }

# Basic variables

uint64_t EABase_PINDATA
uint8_t EABase_PINNO

## Pin numbers

### Arduino

Arduino pinnumbers are used.

### ESP8266

On ESP8266 Dx-numbers are used, Dx-numbers are printed on the board, just write 1 for D1, 2 for D2 ect. (Don't use D1, D2 ect.)

### ESP32

On ESP32 GPIO numbers are used, GPIO numbers are printed on the board

### List of pinnumbers

__uint8_t PANEL_OutputPins[] = {4, 1, 2, 3, 5};__ /* Lenght, pin1, pin2 ... */

The contains length byte and a list of pinnumber, here 4 pins are reference, pinnumbers 1, 2, 3 and 5.

# EABase (EaBase.h)
Basic class where all objects are added, this class contains the basic setup and run methon

* __EABase()__
* void add(EABase_Object *obj)
* __void setup()__, call all setup methods in the EABase object structure
* __void loop()__, call all loop methods in the EABase object structure

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

### EABase_ChipIO(EABase *eabase) (Constructor)

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

### setMode value
false = no pullup
true = pullup

## EABase_ArduinoUNO (EaBase_ArduinoUNO.h)
Implements Arduino UNO board (Can be used for mini as well), extended from EABase_ChipIO

* __EABase_ArduinoUNO(EABase *eabase)__
* __void setIgnoreI2C()__
* __void setIgnoreSPI()__
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_NodeMCUv10 (EaBase_NodeMCUv10.h)
Implements ESP8266 ModeMCU v1.0, extended from EABase_ChipIO

* __EABase_NodeMCUv10(EABase *eabase)__
* __void setIgnoreI2C(EABase_PINNO SDA, EABase_PINNO SCL)__
* __void setIgnoreSPI()__
* uint8_t getPinNo(uint8_t pin)
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_ESP32Wroom (EaBase_ESP32Wroom.h)
Implements ESP32 Wroom board (v4), extended from EABase_ChipIO

* __EABase_ESP32Wroom(EABase *eabase)__
* __void setIgnoreI2C()__
* __void setIgnoreSPI()__
* uint8_t getPinNo(uint8_t pin)
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_MCP23017 (EaBase_MCP23017.h)
Implements I2C chip MCP23017, extended from EABase_ChipIO

* __EABase_MCP23017(EABase *eabase, uint8_t chipAddr)__
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_PCB8574 (EaBase_PCB8574.h)
Implements I2C check PCB8574, extended from EABase_ChipIO

* __EABase_PCB8574(EABase *eabase, uint8_t chipAddr)__
* void setup()
* void loop()
* uint8_t maxPins()
* void setMode(EABase_PINNO pin, int value, bool invert)
* void setMode(EABase_PINNO *pins, int value, bool invert)
* void pinWrite(EABase_PINNO pin, bool value)
* void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
* bool pinRead(EABase_PINNO pin)
* EABase_PINDATA pinRead(EABase_PINNO *pins)

## EABase_Rotary_KY040 (EABase_Rotary_KY040.h)
Implements KY040 rotary encoder, extended from public EABase_Object
* __EABase_Rotary(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinData, int minValue, int maxValue, int start, bool rollOver)__
* __~EABase_Rotary()__
* __bool HasChanged()__
* __int Get()__
* void setup() {}
* void loop() {}
* void Reset(int minValue, int maxValue, int start, bool rollOver);

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

## EABase_TM1637_4Digit (EaBase_TM1637_4Digit.h)
Implements TM1637 4 digit display, extended from EABase_Object

* __EABase_TM1637(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinDIO)__
* __void SetParm(uint8_t brightness = EABase_TM1637_BRIGHT_TYPICAL, uint8_t SetData = 0x40, uint8_t SetAddr = 0xc0)__
* __void ClearDisplay()__
* __void Set(int value)__
* __void BlinkDigit(unsigned long value)__
* __void BlinkPoint(unsigned long value)__
* void setup() {}
* void loop() {}

## EABase_Output (EaBase_Output.h)
Implements a bit for digital output, extended from EABase_Object

* __EABase_Output(EABase_ChipIO *Chip, EABase_PINDATA pin)__
* __void set(bool value)__
* __void get()__
* void setup() {}
* void loop() {}

## EABase_Output (EaBase_OutputBlink.h)
Implements a bit for a blinking digital output (Not PWM!!), extended from EABase_Object

* __EABase_OutputBlink(EABase_ChipIO *Chip, EABase_PINDATA pin)__
* __void set(unsigned long value)__
* void setup()
* void loop() {}

## EABase_OutputDblBlink (EaBase_OutputDblBlink.h)
Implements two bits for a blinking digital output (Not PWM!!), extended from EABase_Object

* __EABase_OutputDblBlink(EABase_ChipIO *Chip, EABase_PINDATA pin1, EABase_PINDATA pin2)__
* __void setTimer(unsigned long value)__
* __void setState(uint8_t state)__
* void setup() {}
* void loop() {}

### Parameters for setState

* 0: Blinking handled by the timer
* 1: Both led is turned Off
* 2: Led 1 is On, Led 2 is Off
* 3: Led 1 is Off, Led 2 is On
* 4: Both led is turned On

## EABase_OutputList (EaBase_OutputList.h)
Combine multiple bits as a hex output, extended from EABase_Object

* __EABase_OutputList(EABase_ChipIO *Chip, EABase_PINNO *pins)__
* __void set(EABase_PINDATA value)__
* __EABase_PINDATA get()__
* void setup() {}
* void loop() {}

## EABase_Input (EaBase_Input.h)
Implements a bit for digital input, extended from EABase_Object

* __EABase_Input(EABase_ChipIO *Chip, EABase_PINNO pin, bool pullup = false, bool invert = false, uint16_t prelTime = 0)__
* __void setPrelTime(uint16_t prelTime)__
* __bool get()__
* void setup()

### Parameters for constructor
* __Chip__, reference to processor to chip (EaBase_ArduinoUNO, EaBase_ESP32Wroom, EaBase_NodeMCUv10, EaBase_MCP23017 or EABase_PCB8574)
* __pin__, Pin number
* __pullup__, Enable pullup (True/False)
* __invert__, Invert result (True/Flase)
* __prelTime__, Time to check for prell is miliseconds, default 0 (no check)


## EABase_InputList (EaBase_InputList.h)
Implements multiple pin read, extended EABase_OutputList

* __EABase_InputList(EABase_ChipIO *Chip, EABase_PINNO *pins, bool pullup = false, bool invert = false, uint16_t prelTime = 0)__
* __void setPrelTime(uint16_t prelTime)__
* __EABase_PINDATA get()__
* void setup()

## EABase_KeyPad (EaBase_KeyPad.h)
Implements a keypad ei. 4x4 keypad, extends EABase_Object

* __EABase_KeyPad(EABase_InputList *kbInput, EABase_OutputList *kbOutput, uint8_t *keyPad)__
* __bool hasValue()__
* __uint8_t value()__
* void setup() {}
* void loop() {}

### Sample

    #include "EaBase.h"
    #include "EaBase_Arduino.h"
    #include "EaBase_MCP23017.h"
    #include "EaBase_Input.h"
    #include "EaBase_Output.h"

    uint8_t PANEL_keypad_keys[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
    uint8_t PANEL_OutputPins[] = {4, 1, 2, 3, 4}; /* Lenght, pin1, pin2 ... */
    uint8_t PANEL_InputPins[] = {4, 5, 6, 7, 8}; /* Lenght, pin1, pin2 ... */

    EABase eaBase = EABase();
    EABase_ArduinoUNO eaBaseBoard = EABase_ArduinoUNO(&eaBase);
    EaBase_MCP23017 eaMcp23017 = EaBase_MCP23017(EABase_MCP23017(&eaBase, 4);
    EABase_OutputList   PANEL_OutputList = EABase_OutputList(&eaMcp23017, PANEL_OutputPins);
    EABase_InputList    PANEL_InputList = EABase_InputList(&eaMcp23017, PANEL_InputPins, true, true, 100);
    EABase_KeyPad       PANEL_keypad = EABase_KeyPad(&PANEL_InputList, &PANEL_OutputList, PANEL_keypad_keys);

    void setup()
    {
      Wire.begin();
      eaBaseBoard.setIgnoreI2C();
      eaBase.setup();
    }

    void loop()
    {
      eaBase.loop();
      if (PANEL_keypad.hasValue() == true)
      {
        char key = PANEL_keypad.value();
	...
      }
    }
