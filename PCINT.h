/******************************************************************************
PCINT.h
Library to provide functionality for Pin Change Interrupts (PCI)
Bobby Schulz @ Northern Widget LLC
11/13/2017
https://github.com/NorthernWidget/PCINT

PCINT allows you to utilize interupts on any pin on the microcontroller, by making use of the 

"Another area in which programming will be very different for the microcontroller
programmer than a system or application programmer is that interrupts
will be very important for the microcontroller application, while an
application programmer will rarely have to deal with them."
-Charles Rennolet, "Notes for Programing PIC Microcontrollers"

Distributed as-is; no warranty is given.
******************************************************************************/
#include "Arduino.h"
#include <avr/interrupt.h>

typedef void (*FunctionType)(); //FIX make into array representation? 

FunctionType GlobalFunc0;
FunctionType GlobalFunc1;
FunctionType GlobalFunc2;
FunctionType GlobalFunc3;

byte TriggerMode[4] = {0}; //Define all interrupt modes (Default to CHANGE)

byte IntPin[4] = {0}; //Define all pins for 4 ints

byte IntState = 0x00; //Holds the state of interrupt enable register when interrupts are turned off

void AttachPCI(byte Pin, void (*UserFunc)(void), int Mode);
boolean Trigger(byte Pin, int Mode);
boolean TestState(byte Pin);
void DetatchPCI(byte pin);

  
void AttachPCI(byte Pin, void (*UserFunc)(void), int Mode) //FIX use enumeration for Mode
{
    *digitalPinToPCMSK(Pin) |= bit (digitalPinToPCMSKbit(Pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(Pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(Pin)); // enable interrupt for the group
    
    switch(digitalPinToPCICRbit(Pin)){
      case 0:
        GlobalFunc0 = UserFunc;
        TriggerMode[0] = Mode;
        IntPin[0] = Pin;
        break;
      case 1:
        GlobalFunc1 = UserFunc;
        TriggerMode[1] = Mode;
        IntPin[1] = Pin;
        break;
      case 2:
        GlobalFunc2 = UserFunc;
        TriggerMode[2] = Mode;
        IntPin[2] = Pin;
        break;
      case 3:
        GlobalFunc3 = UserFunc;
        TriggerMode[3] = Mode;
        IntPin[3] = Pin;
        break;
    }
    IntState = PCICR; //Store interrupt state
}
  
boolean Trigger(byte Pin, int Mode) {
  if((TestState(Pin) && Mode == 3)) return true; //If mode is rising, and rising is true
  if((TestState(Pin) == false && Mode == 2)) return true; //If mode is fall, and falling is ture
  if(Mode == 1) return true; //Else mode is change and trigger regardless 
  return false;
}

boolean TestState(byte Pin){
  switch(digitalPinToPort(Pin)){
    case 1:
      if((PINA & digitalPinToBitMask(Pin)) > 0) return true;
      else return false;
    break;

    case 2:
      if((PINB & digitalPinToBitMask(Pin)) > 0) return true;
      else return false;
    break;

    case 3:
      if((PINC & digitalPinToBitMask(Pin)) > 0) return true;
      else return false;
    break;

    case 4:
      if((PIND & digitalPinToBitMask(Pin)) > 0) return true;
      else return false;
    break;
  }
}

void DetatchPCI(byte pin) {
  PCIFR  = 0x00; // clear any outstanding interrupt
  PCICR  &= bitClear(PCICR, digitalPinToPCICRbit(pin)); // enable interrupt for the group
  IntState = PCICR; //Store interrupt state
}

void EnablePCI(void) {
  PCICR = IntState;
}

void DissablePCI(void) {
  IntState = PCICR; //Store interrupt state value
  PCICR = 0x00; //Disbale all interrupts
}

// void BeginPCI(void) {
ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
  {
    if(Trigger(IntPin[0], TriggerMode[0])) GlobalFunc0(); //Only fun the function if trigger criteria is true 
  }

ISR (PCINT1_vect) // handle pin change interrupt for D8 to D13 here
  {
    if(Trigger(IntPin[1], TriggerMode[1])) GlobalFunc1(); //Only fun the function if trigger criteria is true 
  }
  
ISR (PCINT2_vect) // handle pin change interrupt for D8 to D13 here
  {
    if(Trigger(IntPin[2], TriggerMode[2])) GlobalFunc2(); //Only fun the function if trigger criteria is true 
  }
  
ISR (PCINT3_vect) // handle pin change interrupt for D8 to D13 here
  {
    if(Trigger(IntPin[3], TriggerMode[3])) GlobalFunc3(); //Only fun the function if trigger criteria is true 
  }
// }
 
// void setup() {  
//   Serial.begin(9600);
//   Serial.println("Welcome to the Machine...");
//   Serial.println(CHANGE);
 
// // enable interrupt for pin...
//   pciSetup(7, Dummy, 0);
// }
 
 
// void loop() {
//   delay(10);
//   if(Serial.read() == 'o') {
//     PCIDetatch(7);
//   }
//   // Nothing needed
// }