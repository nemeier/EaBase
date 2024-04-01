#ifndef __EABase_NodeMCUv10_H
#define __EABase_NodeMCUv10_H

#include "EABase.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_NodeMCUv10 : public EABase_ChipIO
{
  private:
    uint8_t getPinNo(uint8_t pin)
    {
      switch (pin)
      {   // D         GPIO
        case 0: return 16;
        case 1: return 5;
        case 2: return 4;
        case 3: return 0;
        case 4: return 2;
        case 5: return 14;
        case 6: return 12;
        case 7: return 13;
        case 8: return 15;
        case 9: return 3;
        case 10: return 1;
      }
    }

  public:
    EABase_NodeMCUv10(EABase *eabase) : EABase_ChipIO(eabase)
	{
		setIgnores( new (EABase_PINNO[3]) { 2, 9, 10 } );
	}

	void setIgnoreI2C(EABase_PINNO SDA, EABase_PINNO SCL)
	{
		setIgnore(SDA);	// SDA pin number
		setIgnore(SCL); // SCL pin number
	}
	
	void setIgnoreSPI()
	{
		setIgnores( new (EABase_PINNO[4]) { 3, 7,6,5 } ); // MOSI, MISO, SCK
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
          pinMode(getPinNo(i), OUTPUT);
		  digitalWrite(getPinNo(i), false);
		}
      }
    }

    void loop() { }

    uint8_t maxPins() {
      return 11;
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
          pinMode(getPinNo(pin), value);
          break;
        case INPUT_PULLUP:
          Direction = setPin(Direction, pin, true);
          PullUp = setPin(PullUp, pin, true);
          Invert = setPin(Invert, pin, invert);
          pinMode(getPinNo(pin), value);
          break;
        case OUTPUT:
          Direction = setPin(Direction, pin, false);
          PullUp = setPin(PullUp, pin, false);
          Invert = setPin(Invert, pin, false);
          pinMode(getPinNo(pin), value);
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
            digitalWrite(getPinNo(pin), value);
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
            digitalWrite(getPinNo(pins[i]), value);
        }
        work = work >> 1;
      }
    }

    bool pinRead(EABase_PINNO pin)
    {
      if (getPin(Direction, pin) == false)
        return getPin(Output, pin);
      else
        return (getPin(Invert, pin) ? !digitalRead(getPinNo(pin)) : digitalRead(getPinNo(pin)));
      return false;
    }

    EABase_PINDATA pinRead(EABase_PINNO *pins)
    {
      EABase_PINDATA value = 0;
      for (EABase_PINNO i = 1; i <= pins[0]; i++)
      {
        value = value << 1;
        //if (digitalRead(getPinNo(pins[i])) != getPin(Invert, pins[i]))
		if (pinRead(pins[i])==true)
          value |= 1;
      }
      return value;
    }
};

#endif
