#ifndef __EABase_OutputBlink_H
#define __EABase_OutputBlink_H

#include "EABase_Output.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_OutputBlink : EABase_Output
{
  public:
    EABase_OutputBlink(EABase_ChipIO *Chip, EABase_PINDATA pin) : EABase_Output(Chip, pin)
    {
    }

    void set(unsigned long value)
    {
      _timer = value;
      if (value > 0)
        _lastMillis = millis();
      else
        _lastMillis = 0;
    }
    
    void setup()
    {
      EABase_Output::setup();
    }
    
    void loop()
    {
      if (_timer > 0)
      {
        if (millis()-_lastMillis > _timer)
        {
          EABase_Output::set(!EABase_Output::get());
          _lastMillis = millis();
        }
      }
    }
    
  protected:
    unsigned long _lastMillis = 0;
    unsigned long _timer = 0;
};

#endif
