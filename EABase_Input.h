#ifndef __EABase_Input_H
#define __EABase_Input_H

#include "EABase.h"
#include "EABase_Output.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_Input : public EABase_Output
{
  public:
    EABase_Input(EABase_ChipIO *Chip, EABase_PINNO pin, bool pullup = false, bool invert = false, uint16_t prelTime = 0) : EABase_Output(Chip, pin)
    {
      _pullup = pullup;
      _invert = invert;
      _currValue = false;
      setPrelTime(prelTime);
    };

    void setPrelTime(uint16_t prelTime)
    {
      _preltime = prelTime;
      _lastMillis = 0;
    }

    void setup()
    {
      _Chip->setMode(_pin, (_pullup==true?INPUT_PULLUP:INPUT), _invert);
    }
    
    bool get()
    {
      bool value;
      if (_preltime == 0)
        return EABase_Output::get();
      if (_lastMillis == 0)
      {
        value = EABase_Output::get();
        if (value != _lastValue)
        {
          _lastValue = value;
          _lastMillis = millis();
        }
      }
      else
      {
        if (millis()-_lastMillis > _preltime)
        {
          value = EABase_Output::get();
          if (_lastValue == value)
          {
            _currValue = value;
            _lastMillis = 0;
          }
          else
          {
            _lastValue = value;
            _lastMillis = millis();
          }
        }
      }
      return _currValue;
    }
  private:
    bool _pullup;
    bool _invert;
    uint16_t _preltime;
    unsigned long _lastMillis;
    bool _currValue;
    bool _lastValue;
};

#endif

