#ifndef __EABase_Object_H
#define __EABase_Object_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase;

class EABase_Object
{
  public:
    EABase_Object(EABase *eabase);

    virtual void setup() = 0;
    virtual void loop() = 0;
  public:
    EABase_Object *classListNext = NULL;
};

#endif
