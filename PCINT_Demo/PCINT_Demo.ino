#include "PCINT.h"

/*
 * Code to demonstrait use of the PCINT functionality
 * 
 * Open serial monitor at 9600 baud
 * Enter "on" and hit return to turn on all interrupts
 * Enter "off" and hit return to turn off interrupts
 * Enter "detatch" and hit enter to detatch the interrupt from pin 7
 * 
 * Restart board to get back to initial conditions
 * 
 * A falling edge on pin 7 will cause the interrupt to be triggered 
 */
void setup() {  
  Serial.begin(9600);
  Serial.println("Welcome to the Machine...");
 
  AttachPCI(7, Dummy, FALLING); //Attach PCI intterupt to digital pin 7, looking for a falling edge, and calling Dummy as the ISR
}
 
 
void loop() {
  String Temp = "null";
  while(Serial.available() == 0); //Wait until character is entered
  
  if(Serial.available() > 0) {
    Temp = Serial.readStringUntil('\n');
  }
  if(Temp == "off") { //Turn off interrupts by entering o
    DissablePCI();
    Serial.println("Interrupts off");
  }
  else if(Temp == "on") { //Turn interrupts back on and enable interrupt on pin 7 once again
    EnablePCI(); 
    Serial.println("Interrups on");
  }
  else if(Temp == "detatch") { //Detatch interrupt from pin 7 (making this vertor usable for other interrupts)
    DetatchPCI(7); 
    Serial.println("Interrupt Vector 1 free");
  }
  else Serial.println("Command not understood");
  delay(1);
}

int Dummy(void) {
  Serial.println("Point to hell...");
}
