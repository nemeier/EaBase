#ifndef __EABase_OutputList_H
#define __EABase_OutputList_H

#include "EABase.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_OutputList : public EABase_Object
{
  public:
    EABase_OutputList(EABase_ChipIO *Chip, EABase_PINNO *pins) : EABase_Object(Chip->baseClass)
    {
      _Chip = Chip;
     _pins = (EABase_PINNO*) malloc (pins[0]+1);
      for (EABase_PINNO i = 0; i < pins[0]+1; i++)
        _pins[i] = pins[i];
    }

    void set(EABase_PINDATA value)
    {
      _Chip->pinWrite(_pins, value);
    }

    EABase_PINDATA get()
    {
      return _Chip->pinRead(_pins);
    }

    void setup()
    {
      _Chip->setMode(_pins, OUTPUT, false);
    }
    void loop() {}

    EABase_ChipIO  *_Chip;
    EABase_PINNO *_pins;
};

#endif
