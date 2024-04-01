#ifndef __EABase_Output_H
#define __EABase_Output_H

#include "EABase.h"
#include "EABase_Object.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_Output : EABase_Object
{
  public:
    EABase_Output(EABase_ChipIO *Chip, 
      EABase_PINDATA pin) : EABase_Object(Chip->baseClass)
    {
      _Chip = Chip;
      _pin = pin;
    }

    void setup()
    {
      _Chip->setMode(_pin, OUTPUT, false);
    }

    void loop() { }

    void set(bool value)
    {
      _Chip->pinWrite(_pin, value);
    }

    bool get()
    {
      return _Chip->pinRead(_pin);
    }

  public:
    EABase_PINDATA _pin;
    EABase_ChipIO *_Chip;
};

#endif
