#ifndef __EABase_MCP23017_H
#define __EABase_MCP23017_H

#include "EABase.h"
#include "EABase_Chip.h"
#include "Wire.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define __EMCP_IODIR  0x00
#define __EMCP_IPOL   0x02
#define __EMCP_GPPU   0x0C
#define __EMCP_GPIO   0x12
#define __EMCP_OLAT   0x14

class EABase_MCP23017 : public EABase_ChipIO
{
  public:
    EABase_MCP23017(EABase *eabase, uint8_t chipAddr) : EABase_ChipIO(eabase)
    {
      I2CAddr = 0x20 | (chipAddr & 0x07);
    }

    void setup()
    {
      writePort(__EMCP_IODIR, Direction); // All output
      writePort(__EMCP_GPPU, PullUp); // No PullUp
      writePort(__EMCP_IPOL, Invert); // Invert
      writePort(__EMCP_GPIO, Output);  // All low
    }

    void loop() { }

    uint8_t maxPins() {
      return 16;
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
      writePort(__EMCP_IODIR, Direction);
      writePort(__EMCP_GPPU, PullUp);
      writePort(__EMCP_IPOL, Invert);
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
      writePort(__EMCP_IODIR, Direction);
      writePort(__EMCP_GPPU, PullUp);
      writePort(__EMCP_IPOL, Invert);
    }

    void pinWrite(EABase_PINNO pin, bool value)
    {
      if (pin >= 0 && pin < maxPins())
      {
        if (getPin(Direction, pin) == false)
        {
          Output = setPin(Output, pin, (getPin(Invert, pin) == true ? !value : value));
          writePort(__EMCP_GPIO, Output);
        }
      }
    }

    void pinWrite(EABase_PINNO *pins, EABase_PINDATA values)
    {
      EABase_PINDATA work = values;
      bool value;
      /*
            Serial.print(" ..");
            Serial.print(Output,HEX);
            Serial.print("+");
            Serial.print(values,HEX);
            Serial.print("+");
      */
      for (EABase_PINNO i = 1; i <= pins[0]; i++)
      {
        if (getPin(Direction, pins[i]) == false)
        {
          value = (work & 1) > 0;
          Output = setPin(Output, pins[i], (getPin(Invert, pins[i]) == true ? !value : value));
        }
        work = work >> 1;
      }
      /*
            Serial.print(Output,HEX);
            Serial.println(".. ");
      */
      writePort(__EMCP_GPIO, Output);
    }

    bool pinRead(EABase_PINNO pin)
    {
      return getPin(readPort(__EMCP_GPIO), pin);
    }

    EABase_PINDATA pinRead(EABase_PINNO *pins)
    {
      EABase_PINDATA input = readPort(__EMCP_GPIO);
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

    EABase_PINDATA readPort(uint8_t reg)
    {
      Wire.beginTransmission(I2CAddr);
      __wiresend(reg);
      Wire.endTransmission();
      Wire.requestFrom(I2CAddr, (uint8_t)2);

      uint16_t output = __wirerecv();
      output |= __wirerecv() << 8;
      return output;
    }

    void writePort(uint8_t reg, EABase_PINDATA value)
    {
	  delay(5);
      Wire.beginTransmission(I2CAddr);
      __wiresend(reg);
      __wiresend(value & 0x00FF);
      __wiresend((value >> 8) & 0x00FF);
      Wire.endTransmission();
    }

    void __wiresend(uint8_t x) {
#if ARDUINO >= 100
      Wire.write(x);
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
