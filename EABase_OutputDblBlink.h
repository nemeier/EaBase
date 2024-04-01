#ifndef __EABase_OutputDblBlink_H
#define __EABase_OutputDblBlink_H

#include "EABase_Output.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_OutputDblBlink : EABase_Object
{
  public:
    EABase_OutputDblBlink(EABase_ChipIO *Chip, EABase_PINDATA pin1, EABase_PINDATA pin2) : EABase_Object(Chip->baseClass)
    {
      _Output_Pin1 = new EABase_Output(Chip, pin1);
      _Output_Pin2 = new EABase_Output(Chip, pin2);
    }

    void setTimer(unsigned long value)
    {
      _state = 0;
      _timer = value;
      if (value > 0)
        _lastMillis = millis();
      else
        _lastMillis = 0;
    }

    void setState(uint8_t state)
    {
      _state = state;
      switch(_state)
      {
        case 0:
          setTimer(_timer);
          break;
        case 1:
          _Output_Pin1->set(false);
          _Output_Pin2->set(false);
          break;
        case 2:
          _Output_Pin1->set(true);
          _Output_Pin2->set(false);
          break;
        case 3:
          _Output_Pin1->set(false);
          _Output_Pin2->set(true);
          break;
        case 4:
          _Output_Pin1->set(true);
          _Output_Pin2->set(true);
          break;
      }
    }

    void setup()
    {
    }

    void loop()
    {
      if (_state == 0)
      {
        if (_timer > 0)
        {
          if (millis() - _lastMillis > _timer)
          {
            bool value = _Output_Pin1->get();
            _Output_Pin1->set(!value);
            _Output_Pin2->set(value);
            _lastMillis = millis();
          }
        }
      }
    }

  protected:
    EABase_Output *_Output_Pin1;
    EABase_Output *_Output_Pin2;

    unsigned long _lastMillis = 0;
    unsigned long _timer = 0;
    uint8_t _state;
};

#endif
