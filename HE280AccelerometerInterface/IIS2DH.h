#ifndef IIS2DH_H
#define IIS2DH_H

#define ACCELEROMETER_I2C_ADDR 0x19
#define Z_PROBE_SENSITIVITY 20 // 0-126 7 bit value

#include <Arduino.h>
#include <Wire.h>

void accelerometer_send(uint8_t);
void accelerometer_write(uint8_t, uint8_t);
bool accelerometer_recv(uint8_t);
void accelerometer_init();
bool accelerometer_status();

#endif
