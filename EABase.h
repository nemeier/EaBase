#ifndef __EABase_H
#define __EABase_H

#include "EABase_Object.h";

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define EABase_PINDATA uint64_t
#define EABase_PINNO uint8_t

class EABase
{
  public:
    EABase()
    {
    }

    void add(EABase_Object *obj)
    {
      if (classListFirst == NULL)
      {
        classListFirst = obj;
        classListLast = obj;
      }
      else
      {
        classListLast->classListNext = obj;
        classListLast = obj;
      }
    }

    void setup()
    {
      EABase_Object *obj = classListFirst;
      while (obj != NULL)
      {
        obj->setup();
        obj = obj->classListNext;
      }
    }

    void loop()
    {
      EABase_Object *obj = classListFirst;
      while (obj != NULL)
      {
        obj->loop();
        obj = obj->classListNext;
      }
    }
  private:
    EABase_Object *classListFirst = NULL;
    EABase_Object *classListLast = NULL;
};

#endif
