#include "EABase_Object.h"
#include "EABase.h"

EABase_Object::EABase_Object(EABase *eabase)
{
  eabase->add(this);
}

