#ifndef __EABase_PCB8574_H
#define __EABase_PCB8574_H

#include "EABase.h"
#include "EABase_Chip.h"
#include "Wire.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_PCB8574 : public EABase_ChipIO
{
  public:
    EABase_PCB8574(EABase *eabase, uint8_t chipAddr) : EABase_ChipIO(eabase)
    {
      I2CAddr = 0x20 | (chipAddr & 0x07);
    }

    void setup()
    {
      Direction = 0;
      Output = 0;
      Invert = 0;
      PullUp = 0;
      writePort(Output);  // All low
    }

    void loop() { }

    uint8_t maxPins() {
      return 8;
    }

    void setMode(EABase_PINNO pin, int value, bool invert)
    {
      switch (value)
      {
        case INPUT:
          Direction = setPin(Direction, pin, true);
          PullUp = setPin(PullUp, pin, false);
          Invert = setPin(Invert, pin, invert);
          break;
        case INPUT_PULLUP:
          Direction = setPin(Direction, pin, true);
          PullUp = setPin(PullUp, pin, true);
          Invert = setPin(Invert, pin, invert);
          break;
        case OUTPUT:
          Direction = setPin(Direction, pin, false);
          PullUp = setPin(PullUp, pin, false);
          Invert = setPin(Invert, pin, false);
          break;
      }
    }

    void setMode(EABase_PINNO *pins, int value, bool invert)
    {
      for (EABase_PINNO i = 0; i < pins[0]; i++)
      {
        switch (value)
        {
          case INPUT:
            Direction = setPin(Direction, pins[i + 1], true);
            PullUp = setPin(PullUp, pins[i + 1], false);
            Invert = setPin(Invert, pins[i + 1], invert);
            break;
          case INPUT_PULLUP:
            Direction = setPin(Direction, pins[i + 1], true);
            PullUp = setPin(PullUp, pins[i + 1], true);
            Invert = setPin(Invert, pins[i + 1], invert);
            break;
          case OUTPUT:
            Direction = setPin(Direction, pins[i + 1], false);
            PullUp = setPin(PullUp, pins[i + 1], false);
            Invert = setPin(Invert, pins[i + 1], false);
            break;
        }
      }
    }

    void pinWrite(EABase_PINNO pin, bool value)
    {
      if (pin >= 0 && pin < maxPins())
      {
        if (getPin(Direction, pin) == false)
        {
          Output = setPin(Output, pin, (getPin(Invert, pin)==true ? !value : value));
          writePort(Output);
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
          Output = setPin(Output, pins[i], (getPin(Invert, pins[i])==true ? !value : value));
        }
        work = work >> 1;
      }
      writePort(Output);
    }

    bool pinRead(EABase_PINNO pin)
    {
      if (getPin(Direction, pin) == false)
        return getPin(Output, pin);
      else
        return getPin(readPort(), pin);
      return false;
    }

    EABase_PINDATA pinRead(EABase_PINNO *pins)
    {
      EABase_PINDATA input = readPort();
      EABase_PINDATA value = 0;
      for (EABase_PINNO i = 0; i < pins[0]; i++)
      {
        value = value << 1;
        if (getPin(input, pins[i + 1]) == true)
          value |= 1;
      }
      return value;
    }

  private:
    uint8_t  I2CAddr;

    EABase_PINDATA readPort()
    {
      Wire.requestFrom(I2CAddr, (uint8_t)1);
      uint16_t output = __wirerecv();
      return output;
    }

    void writePort(EABase_PINDATA value)
    {
      Wire.beginTransmission(I2CAddr);
      __wiresend(value & 0x00FF);
      Wire.endTransmission();
    }

    void __wiresend(uint8_t x) {
#if ARDUINO >= 100
      Wire.write((uint8_t) x);
#else
      Wire.send(x);
#endif
    }

    uint8_t __wirerecv(void) {
#if ARDUINO >= 100
      return Wire.read();
#else
      return Wire.receive();
#endif
    }
};

#endif
