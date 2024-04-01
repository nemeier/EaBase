#ifndef __EABase_TM1637_H
#define __EABase_TM1637_H

/* Base code copied from

   https://github.com/Seeed-Studio/Grove_4Digital_Display/blob/master/TM1637.cpp

   TM1637.cpp
   A library for the 4 digit display

   Copyright (c) 2012 seeed technology inc.
   Website    : www.seeed.cc
   Author     : Frankie.Chu
   Create Time: 9 April,2012
   Change Log :

   The MIT License (MIT)
*/

#include "EABase.h"
#include "EABase_Object.h"
#include "EABase_Chip.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define EABase_TM1637_ADDR_AUTO  0x40
#define EABase_TM1637_ADDR_FIXED 0x44
#define EABase_TM1637_BRIGHT_TYPICAL 2
#define EABase_TM1637_BRIGHT_DARKEST 0
#define EABase_TM1637_BRIGHTEST 7

static int8_t EABase_TM1637_TubeTab[] = { 0x3f, 0x06, 0x5b, 0x4f,
                                          0x66, 0x6d, 0x7d, 0x07,
                                          0x7f, 0x6f, 0x77, 0x7c,
                                          0x39, 0x5e, 0x79, 0x71, 0x7f
                                        };//0~9,A,b,C,d,E,F,_

class EABase_TM1637 : public EABase_Object
{
  private:
    int data = -1;
    unsigned long blinkDigit = 0;
    unsigned long blinkPoint = 0;
    unsigned long lastDigit = 0;
    unsigned long lastPoint = 0;
    bool DigitOn = false;
    bool PointOn = false;

    uint8_t pinClock;
    uint8_t pinData;
    boolean PointFlag = false;
    EABase_ChipIO *_Chip;

    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;

  public:

    EABase_TM1637(EABase_ChipIO *Chip, uint8_t pinClk, uint8_t pinDIO) : EABase_Object(Chip->baseClass)
    {
      _Chip = Chip;
      pinClock = pinClk;
      pinData = pinDIO;
    }

    void setup()
    {
      _Chip->setMode(pinData, OUTPUT, false);
      _Chip->setMode(pinClock, OUTPUT, false);
      SetParm();
      ClearDisplay();
    }

    void loop()
    {
      bool hasChanged =false;
      if (blinkDigit > 0)
      {
        if (lastDigit == 0 || millis() > lastDigit + blinkDigit)
        {
          lastDigit = millis();
          hasChanged = true;
          DigitOn = !DigitOn;
        }
      }
      if (blinkPoint > 0)
      {
        if (lastPoint == 0 || millis() > lastPoint + blinkPoint)
        {
          lastPoint = millis();
          hasChanged = true;
          PointOn = !PointOn;
        }
      }
      if (hasChanged)
      {
        PointFlag = PointOn;
        if (DigitOn)
          Set(data);
        else
          clearDisplay();
      }
    }

    void SetParm(uint8_t brightness = EABase_TM1637_BRIGHT_TYPICAL, uint8_t SetData = 0x40, uint8_t SetAddr = 0xc0)
    {
      Cmd_SetData = SetData;
      Cmd_SetAddr = SetAddr;
      Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
    }

    void ClearDisplay()
    {
      Set(-1);
    }

    void Set(int value)
    {
      if (value < 0)
      {
        data = -1;
        blinkDigit = false;
        blinkPoint = false;
        clearDisplay();
      }
      else
      {
        data = value;
        while (data > 9999)
          data -= 10000;
        Display(0, getDigit(4, data));
        Display(1, getDigit(3, data));
        Display(2, getDigit(2, data));
        Display(3, getDigit(1, data));
      }
    }

    void BlinkDigit(unsigned long value)
    {
      blinkDigit = value;
    }

    void BlinkPoint(unsigned long value)
    {
      blinkPoint = value;
    }
    
  private:

    void clearDisplay(void)
    {
      Display(0x00, 0x7f);
      Display(0x01, 0x7f);
      Display(0x02, 0x7f);
      Display(0x03, 0x7f);
    }

    int getDigit(int pos, int value)
    {
      if (pos < 1) return 0;
      if (pos == 1)
        return value % 10;
      else
        return getDigit(pos - 1, value / 10);
    }

    void Point(boolean pointFlag)
    {
      PointFlag = pointFlag;
    }

    void Display(int8_t DispData[])
    {
      int8_t SegData[4];
      uint8_t i;
      for (i = 0; i < 4; i ++)
      {
        SegData[i] = DispData[i];
      }
      Coding(SegData);
      Start();
      WriteByte(EABase_TM1637_ADDR_AUTO);
      Stop();
      Start();
      WriteByte(Cmd_SetAddr);
      for (i = 0; i < 4; i ++)
      {
        WriteByte(SegData[i]);
      }
      Stop();
      Start();
      WriteByte(Cmd_DispCtrl);
      Stop();
    }

    void Display(uint8_t BitAddr, int8_t DispData)
    {
      int8_t SegData;
      SegData = Coding(DispData);
      Start();
      WriteByte(EABase_TM1637_ADDR_FIXED);
      Stop();
      Start();
      WriteByte(BitAddr | 0xc0);
      WriteByte(SegData);
      Stop();
      Start();
      WriteByte(Cmd_DispCtrl);
      Stop();
    }

    void Coding(int8_t DispData[])
    {
      uint8_t PointData;
      if (PointFlag == true)
        PointData = 0x80;
      else
        PointData = 0;
      for (uint8_t i = 0; i < 4; i ++)
      {
        if (DispData[i] == 0x7f)
          DispData[i] = 0x00;
        else
          DispData[i] = EABase_TM1637_TubeTab[DispData[i]] + PointData;
      }
    }

    int8_t Coding(int8_t DispData)
    {
      uint8_t PointData;
      if (PointFlag == true)
        PointData = 0x80;
      else
        PointData = 0;
      if (DispData == 0x7f)
        DispData = 0x00 + PointData;
      else
        DispData = EABase_TM1637_TubeTab[DispData] + PointData;
      return DispData;
    }

    int WriteByte(int8_t wr_data)
    {
      uint8_t i, count1;
      for (i = 0; i < 8; i++)                     //sent 8bit data
      {
        _Chip->pinWrite(pinClock, LOW);           // digitalWrite(pinClock, LOW);
        if (wr_data & 0x01)
          _Chip->pinWrite(pinData, HIGH);         // digitalWrite(pinData, HIGH); //LSB first
        else
          _Chip->pinWrite(pinData, LOW);          // digitalWrite(pinData, LOW);
        wr_data >>= 1;
        _Chip->pinWrite(pinClock, HIGH);          // digitalWrite(pinClock, HIGH);
      }
      _Chip->pinWrite(pinClock, LOW);             // digitalWrite(pinClock, LOW); //wait for the ACK
      _Chip->pinWrite(pinData, HIGH);             // digitalWrite(pinData, HIGH);
      _Chip->pinWrite(pinClock, HIGH);            // digitalWrite(pinClock, HIGH);
      _Chip->setMode(pinData, INPUT, false);      // pinMode(pinData, INPUT);

      BitDelay();
      uint8_t ack = _Chip->pinRead(pinData);      // uint8_t ack = digitalRead(pinData);
      if (ack == 0)
      {
        _Chip->setMode(pinData, OUTPUT, false);   // pinMode(pinData, OUTPUT);
        _Chip->pinWrite(pinData, LOW);            // digitalWrite(pinData, LOW);
      }
      BitDelay();
      _Chip->setMode(pinData, OUTPUT, false);     // pinMode(pinData, OUTPUT);
      BitDelay();

      return ack;
    }

    void BitDelay(void)
    {
      delayMicroseconds(50);
    }

    void Start(void)
    {
      _Chip->pinWrite(pinClock, HIGH);            // digitalWrite(pinClock, HIGH); //send start signal to TM1637
      _Chip->pinWrite(pinData, HIGH);             // digitalWrite(pinData, HIGH);
      _Chip->pinWrite(pinData, LOW);              // digitalWrite(pinData, LOW);
      _Chip->pinWrite(pinClock, LOW);             // digitalWrite(pinClock, LOW);
    }

    void Stop(void)
    {
      _Chip->pinWrite(pinClock, LOW);             // digitalWrite(pinClock, LOW);
      _Chip->pinWrite(pinData, LOW);              // digitalWrite(pinData, LOW);
      _Chip->pinWrite(pinClock, HIGH);            // digitalWrite(pinClock, HIGH);
      _Chip->pinWrite(pinData, HIGH);             // digitalWrite(pinData, HIGH);
    }
};

#endif
