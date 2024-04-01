#ifndef __EABase_Chip_H
#define __EABase_Chip_H

#include "EABase.h"
#include "EABase_Object.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_Chip : EABase_Object
{
  public:
    EABase_Chip(EABase *eabase) : EABase_Object(eabase) {
      baseClass = eabase;
    }
    
    EABase *baseClass;
};

class EABase_ChipIO : public EABase_Chip
{
  public:
    EABase_ChipIO(EABase *eabase) : EABase_Chip(eabase) {}
    
    virtual uint8_t getPinNo(uint8_t pin) { return pin; }

    void setIgnore(EABase_PINNO pin) { Ignore = setPin(Ignore, pin, true); }
    void setIgnores(EABase_PINNO *pins)
    {
      for (EABase_PINNO i = 0; i < pins[0]; i++)
        setIgnore(pins[i + 1]);
    }

    virtual void setMode(EABase_PINNO pin, int value, bool invert) = 0;
    virtual void setMode(EABase_PINNO *pins, int value, bool invert) = 0;
    virtual void pinWrite(EABase_PINNO pin, bool value) = 0;
    virtual void pinWrite(EABase_PINNO *pins, EABase_PINDATA values) = 0;
    virtual bool pinRead(EABase_PINNO pin) = 0;
    virtual EABase_PINDATA pinRead(uint8_t *pins) = 0;
    virtual uint8_t maxPins() = 0;

    EABase_PINDATA setPin(EABase_PINDATA input, uint8_t pin, bool value)
    {
      EABase_PINDATA work = input;
      if (pin >= 0 && pin < maxPins())
      {
        if (value == true)
          return (input | ((EABase_PINDATA)1 << pin));
        else
          return (input & ~((EABase_PINDATA)1 << pin));
      }
      return input;
    }
    
    bool getPin(EABase_PINDATA input, int pin)
    {
      if (pin >= 0 && pin < maxPins())
      {
        if ((input & ((EABase_PINDATA)1 << pin)) > 0)
          return true;
        else
          return false;
      }
      return false;
    }
	
    EABase_PINDATA Ignore = 0;
    EABase_PINDATA Direction = 0;
    EABase_PINDATA PullUp = 0;
    EABase_PINDATA Output = 0;
    EABase_PINDATA Invert = 0;
};

#endif
