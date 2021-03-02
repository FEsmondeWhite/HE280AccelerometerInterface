#include <Wire.h>

#define DEBUG_ENABLED false
#define ACCELEROMETER_I2C_ADDR 0x19
#define DEBOUNCE_INTERVAL 10 // 10 ms debounce time

#include "IIS2DH.h"


const uint8_t click_pin = 2; // "INT" interrupt pin from HE280 - Blue on my model

// TODO: Not currently using the arming sequence. Automatically re-arms after trigger debounce interval.
const uint8_t mod_pin = 3; // Used to arm the probe. Z Probe Mod pin.
const uint8_t trigger_pin = 13; // Sends a HIGH signal when probe touches the table. Connected to Duet's Z probe in. Duet set to Type: 5.


void setup() {
  pinMode(click_pin, INPUT);
  pinMode(mod_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);

  Serial.begin(115200); // open the serial port at 115200 bps

  accelerometer_init();
}

void loop() {
  
  Serial.println("Resetting accelerometer.");
  while (!accelerometer_status());
  
  Serial.println("Accelerometer ready.");

  while (digitalRead(click_pin));

  Serial.println("Accelerometer triggered");
  
  digitalWrite(trigger_pin,HIGH);
  delay(DEBOUNCE_INTERVAL);

  Serial.println("Done trigger");
  
  digitalWrite(trigger_pin,LOW);
  
}
