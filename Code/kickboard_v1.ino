//-------------------------------------------//
// Navigational Kickboard Project
// by Hazel Mitchell
//-------------------------------------------//
//
// Startup TODO:
// [] Check GPS location against list of sites
// [] Set operation mode
// [] Buzz/beep to indicate mode 
//    (1 = pool detected, 2 = open detected, 3 = location not known (pool mode)
// [] Init sensors for relevant mode
//    Pool mode: GPS off, optical flow + accelerometer on
//    Open mode: GPS on
//    Both: ToF obstacle detection
//
//-------------------------------------------//
//
// Pool (lane following) mode TODO:
// [] Continuous accelerometer sampling
// [] Continuous optical flow sampling
// [] Continuous obstacle detection
// [] Estimate kickboard orientation
// [] Estimate bearing if horizontal
// [] Obstacle warning
// [] Bearing correction
// [] (Optional) Record lap duration
//
//-------------------------------------------//
//
// Open water (beacon seeking) mode TODO:
// [] Continuous GPS sampling
// [] Calculate recent swimming vector
// [] Bearing correction
// [] (Optional) Calculate & buzz distance to next beacon
// [] (Optional) Buzz for each target location reached
// [] (Optional) Continuous obstacle detection
// [] (Optional) Send periodic SMS location updates
//
//-------------------------------------------//
//
// Emergency mode TODO:
// [] Periodic GPS sampling
// [] Emergency alarm
// [] Emergency lights
// [] Set course for emergency location
// [] Send periodic SMS location updates + emergency message
// [] Add voltage to SMS message
//
//-------------------------------------------//
//
// Instructions:
// To upload: 
// Under Tools > Upload method, select "STM32CubeProgrammer (DFU)."
// Press and hold the BOOT button on the Swan, 
// press and release RESET, then release BOOT to cause the Swan to jump into its bootloader. 
// This sequence must be done every time you want to upload firmware to the Swan.
//
//-------------------------------------------//

#include "Bitcraze_PMW3901.h"

//Pin definitions for Swan
#ifdef CS
#undef CS
#endif

#define CS 10 // Using digital pin 10 for chip select

#define BUZZER_PIN D9 // The Arduino pin connected to the buzzer

Bitcraze_PMW3901 flow(10);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("Start PMW3901");

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
  }
}

int16_t deltaX,deltaY; //Optical flow vars
int driftThreshold = 10; //Maximum deviation from a straight line allowed
int noteDuration = 1000 / 4;
int pauseBetweenNotes = noteDuration * 1.30; // to distinguish the notes, set a minimum time between them.

void loop() {
  // put your main code here, to run repeatedly:
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);

  Serial.print("X: ");
  Serial.print(deltaX);
  Serial.print(", Y: ");
  Serial.print(deltaY);
  Serial.print("\n");

  if (deltaX > driftThreshold){
    //Tell the user to turn left
    int nBuzz = 1 + int(abs(deltaX/100));
    for (int i = 0; i <= nBuzz; i++) {
      tone(BUZZER_PIN, 33, noteDuration); //C1 = 33 (Low note)
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN); // stop the tone playing:
    }
  }
  else if (deltaX < (0 - driftThreshold)){
    //Tell the user to turn right
    int nBuzz = 1 + int(abs(deltaX/100));
    for (int i = 0; i <= nBuzz; i++) {
      tone(BUZZER_PIN, 4186, noteDuration); //C8 = 33 (High note)
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN); // stop the tone playing:
    }
  }
  else {
    //Buzz both sides once
    tone(BUZZER_PIN, 262, noteDuration); //C4 = 262 (Mid note)
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN); // stop the tone playing:
  }

  delay(5000);
}

void buzzLeft(int nBuzz){
  
}
