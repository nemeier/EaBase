#ifndef __EABase_ArduinoUNO_H
#define __EABase_ArduinoUNO_H

#include "EABase.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_ArduinoUNO : public EABase_ChipIO
{
  public:
    EABase_ArduinoUNO(EABase *eabase) : EABase_ChipIO(eabase) { }

    void setIgnoreI2C()
    {
      setIgnores( new (EABase_PINNO[3]) { 2, 18,19 } ); // SDA, SCL
    }
	
    void setIgnoreSPI()
    {
      setIgnores( new (EABase_PINNO[4]) { 3, 12,11,10 } ); // MOSI, MISO, SCK
    }

    void setup()
    {
      Direction = 0;
      PullUp = 0;
      Invert = 0;
      Output = 0;
      for (EABase_PINNO i = 0; i < maxPins(); i++)
      {
		if (getPin(Ignore, i) == false)
		{
		  pinMode(i, OUTPUT);
		  digitalWrite(i, false);
		}
      }
    }

    void loop() { }

    uint8_t maxPins() {
      return 22;
    }

    void setMode(EABase_PINNO pin, int value, bool invert)
    {
  	  if (getPin(Ignore, pin) == true)
	  { // Simulate output
        Direction = setPin(Direction, pin, false);
        PullUp = setPin(PullUp, pin, false);
        Invert = setPin(Invert, pin, false);
		return;
	  }
      switch (value)
      {
        case INPUT:
          Direction = setPin(Direction, pin, true);
          PullUp = setPin(PullUp, pin, false);
          Invert = setPin(Invert, pin, invert);
          pinMode(pin, value);
          break;
        case INPUT_PULLUP:
          Direction = setPin(Direction, pin, true);
          PullUp = setPin(PullUp, pin, true);
          Invert = setPin(Invert, pin, invert);
          pinMode(pin, value);
          break;
        case OUTPUT:
          Direction = setPin(Direction, pin, false);
          PullUp = setPin(PullUp, pin, false);
          Invert = setPin(Invert, pin, false);
          pinMode(pin, value);
          break;
      }
    }

    void setMode(EABase_PINNO *pins, int value, bool invert)
    {
      for (EABase_PINNO i = 0; i < pins[0]; i++)
        setMode(pins[i + 1], value, invert);
    }

    void pinWrite(EABase_PINNO pin, bool value)
    {
      if (pin >= 0 && pin < maxPins())
      {
        if (getPin(Direction, pin) == false)
        {
          Output = setPin(Output, pin, value);
		  if (getPin(Ignore, pin) == false)
            digitalWrite(pin, value);
        }
      }
    }

    void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
    {
      EABase_PINDATA work = values;
      bool value;
      for (EABase_PINNO i = 1; i <= pins[0]; i++)
      {
        if (getPin(Direction, pins[i]) == false)
        {
          value = (work & 1) > 0;
          Output = setPin(Output, pins[i], value);
		  if (getPin(Ignore, pins[i]) == false)
            digitalWrite(pins[i], value);
        }
        work = work >> 1;
      }
    }

    bool pinRead(EABase_PINNO pin)
    {
      if (getPin(Direction, pin) == false)
        return getPin(Output, pin);
      else
        return (getPin(Invert, pin) ? !digitalRead(pin) : digitalRead(pin));
      return false;
    }

    EABase_PINDATA pinRead(EABase_PINNO *pins)
    {
      EABase_PINDATA value = 0;
      for (EABase_PINNO i = 1; i <= pins[0]; i++)
      {
        value = value << 1;
        //if (digitalRead(pins[i]) != getPin(Invert, pins[i]))
		if (pinRead(pins[i])==true)
          value |= 1;
      }
      return value;
    }
};

#endif
