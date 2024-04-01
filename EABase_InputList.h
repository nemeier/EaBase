#ifndef __EABase_InputList_H
#define __EABase_InputList_H

#include "EABase.h"
#include "EABase_OutputList.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_InputList : public EABase_OutputList
{
  public:
    EABase_InputList(EABase_ChipIO *Chip, EABase_PINNO *pins, bool pullup = false, bool invert = false, uint16_t prelTime = 0) : EABase_OutputList(Chip, pins)
    {
      _pullup = pullup;
      _invert = invert;
      _currValue = 0;
      setPrelTime(prelTime);
    }

    void setup()
    {
      _Chip->setMode(_pins, (_pullup == true ? INPUT_PULLUP : INPUT), _invert);
    }

    void setPrelTime(uint16_t prelTime)
    {
      _preltime = prelTime;
      _lastMillis = 0;
    }

    EABase_PINDATA get()
    {
      EABase_PINDATA value;
      if (_preltime == 0)
        return EABase_OutputList::get();
      if (_lastMillis == 0)
      {
        value = EABase_OutputList::get();
        if (value != _lastValue)
        {
          _lastValue = value;
          _lastMillis = millis();
        }
      }
      else
      {
        if (millis() - _lastMillis > _preltime)
        {
          value = EABase_OutputList::get();
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
    uint16_t _currValue;
    uint16_t _lastValue;
};

#endif
