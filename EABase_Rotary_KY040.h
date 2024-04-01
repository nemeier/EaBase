#ifndef __EABase_Rotary_H
#define __EABase_Rotary_H
/*
Base code copied from http://www.mathertel.de/Arduino/RotaryEncoderLibrary.aspx

Works with KY-040 with pull up, no handing of button.

ESP8266 kode skal checkes men burde være ens med ESP32
ARDUINO kode mangler

*/
#include "EABase.h"
#include "EABase_Object.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define EABase_Rotary_MaxEncoders 16
const int8_t EABase_Rotary_KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0 , -1,
  -1,  0,  0,  1,
  0,  1, -1,  0
};

class EABase_Rotary : public EABase_Object
{
  private:
    EABase_ChipIO *_Chip;

  public:
    bool _rollOver = 0;
    volatile bool _hasChanges = false;

    uint8_t _pinClock;
    uint8_t _pinData;
    volatile int _data = 0;
    volatile int _minValue = 0;
    volatile int _maxValue = 0;

    volatile int8_t _oldState;
    volatile long _position;

    void setup() {}
    void loop() {}

    EABase_Rotary(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinData, int minValue, int maxValue, int start, bool rollOver);
    ~EABase_Rotary();
    void Reset(int minValue, int maxValue, int start, bool rollOver);
    bool HasChanged() {
      return _hasChanges;
    }
    int Get() {
      _hasChanges = false;
      return _data;
    }
};

#if defined(ESP32)
void IRAM_ATTR EABase_Rotary_ISR(void * arg)
{
  EABase_Rotary* object = (EABase_Rotary*)arg;
  int sig1 = digitalRead(object->_pinClock);
  int sig2 = digitalRead(object->_pinData);
  int8_t thisState = sig1 | (sig2 << 1);
  if (object->_oldState != thisState)
  {
    object->_position += EABase_Rotary_KNOBDIR[thisState | (object->_oldState << 2)];
    object->_oldState = thisState;
    if (thisState == 0 || thisState == 3)
    {
      if ((object->_position >> 1) > 0)
      {
        if (object->_data == object->_maxValue)
        {
          if (object->_rollOver == true)
            object->_data = object->_minValue;
        }
        else
          object->_data++;
        object->_hasChanges = true;

        object->_position = ((0 << 1) | (object->_position & 0x01L));
      }
      if ((object->_position << 1) < 0)
      {
        if (object->_data == object->_minValue)
        {
          if (object->_rollOver == true)
            object->_data = object->_maxValue;
        }
        else
          object->_data--;
        object->_hasChanges = true;

        object->_position = ((0 << 1) | (object->_position & 0x01L));
      }
    }
  }
}
#endif

EABase_Rotary::EABase_Rotary(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinData, int minValue, int maxValue, int start, bool rollOver) : EABase_Object(Chip->baseClass)
{
  _Chip = Chip;
  _pinClock = _Chip->getPinNo(pinClk);
  _pinData = _Chip->getPinNo(pinData);

  Reset(minValue, maxValue, start, rollOver);

  _Chip->setIgnore(pinClk);
  _Chip->setIgnore(pinData);
  pinMode(_pinClock, INPUT_PULLUP);
  pinMode(_pinData, INPUT_PULLUP);
#if defined(ESP32)
  attachInterruptArg(digitalPinToInterrupt(_pinClock), EABase_Rotary_ISR, this, CHANGE);
  attachInterruptArg(digitalPinToInterrupt(_pinData), EABase_Rotary_ISR, this, CHANGE);
#endif
}

EABase_Rotary::~EABase_Rotary()
{
#if defined(ESP32)
  detachInterrupt(digitalPinToInterrupt(_pinClock));
  detachInterrupt(digitalPinToInterrupt(_pinData));
#endif
}

void EABase_Rotary::Reset(int minValue, int maxValue, int start, bool rollOver)
{
  if (maxValue >= minValue)
  {
    _minValue = minValue;
    _maxValue = maxValue;
  }
  else
  {
    _minValue = maxValue;
    _maxValue = minValue;
  }
  _rollOver = rollOver;
  if (start > maxValue || start < minValue)
    _data = minValue;
  else
    _data = start;
  _hasChanges = true;
}

#endif
