#ifndef __EABase_KeyPad_H
#define __EABase_KeyPad_H

#include "EABase.h"
#include "EABase_InputList.h"
#include "EABase_OutputList.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EABase_KeyPad : public EABase_Object
{
  public:
    EABase_KeyPad(EABase_InputList *kbInput, EABase_OutputList *kbOutput, uint8_t *keyPad) : EABase_Object(kbInput->_Chip->baseClass)
    {
      uint8_t count = kbInput->_pins[0] * kbOutput->_pins[0];
      _kbInput = kbInput;
      _kbOutput = kbOutput;
      _keyPad = (uint8_t*) malloc (count);
      for (uint8_t i = 0; i < count; i++)
        _keyPad[i] = keyPad[i];
      _kbInput->setPrelTime(0); // Disable prel test in Input
    }

    bool hasValue()
    {
      return (_currValue > 0);
    }
    uint8_t value()
    {
      uint8_t retValue = _currValue;
      _currValue = 0;
      return retValue;
    }

    void loop()
    {
      switch (_state)
      {
        case 0:// Read from keyboard
          {
            uint8_t cValue = ReadKeyboard();
            if (cValue != 0xFF)
            {
              _lastValue = cValue;
              _state = 1;
              _lastMillis = millis();
            }
          }
          break;
        case 1: // Second time
          if (millis() - _lastMillis > _timer1)
          {
            uint8_t cValue = ReadKeyboard();
            if (cValue == _lastValue)
            {
              _currValue = _lastValue;
              _lastMillis = millis();
              _state = 2;
            }
            else
            {
              if (cValue == 0xFF)
              {
                _state = 0;
              }
              else
              {
                _lastValue = cValue;
                _lastMillis = millis();
              }
            }
          }
          break;
        case 2:
          {
            if (millis() - _lastMillis > _timer2)
              _state = 0;
          }
          break;
      }
    }

    void setup()
    {
    }

  private:
    uint8_t *_keyPad;
    EABase_InputList *_kbInput;
    EABase_OutputList *_kbOutput;
    unsigned long _lastMillis;
    unsigned long _timer1 = 50;
    unsigned long _timer2 = 300;
    uint8_t _currValue = 0;
    uint8_t _lastValue = 0;
    uint8_t _state = 0;

    uint8_t ReadKeyboard()
    {
      uint8_t outpos = 0;
      uint8_t inpos = 0;
      uint16_t invalue = 0;
      uint16_t outvalue = 0;

      for (outpos = 0; outpos < _kbOutput->_pins[0]; outpos++)
      {
        outvalue = 0;
        for (uint8_t i = 0; i < _kbOutput->_pins[0]; i++)
        {
          outvalue = outvalue << 1;
          if (i != outpos) outvalue = outvalue | 0x01;
        }
        _kbOutput->set(outvalue);
        invalue = _kbInput->get();
        for (inpos = 0; inpos < _kbInput->_pins[0]; inpos++)
        {
          if ((invalue & 0x01) == 1)
          {
            return _keyPad[(inpos * _kbInput->_pins[0]) + outpos];
          }
          invalue = invalue >> 1;
        }
      }
      return 0xFF;
    }
};

#endif

