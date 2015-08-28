#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

#include "AltOSCompanion.h"

typedef unsigned int(*map_func)(float);

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

bool companionConnected = false;
unsigned long startTime;

float normalize(float val) {
  return (val + 180.0) / 360.0;
}

float denormalize(float val) {
  return (val * 360.0) - 180.0;
}

unsigned int encode(float val) {
  return normalize(val) * 10000;
}

float decode(unsigned int val) {
  return denormalize(val / 10000.0);
}

unsigned int map0To360(float val) {
  return normalize(val) * 360.0;
}

void mapAndPrintOrientation(sensors_vec_t orientation, map_func m); // Avoids compile error
void mapAndPrintOrientation(sensors_vec_t orientation, map_func m) {
  Serial.print("Roll: ");
  Serial.print(m(orientation.roll)); 
  Serial.print(" Pitch: ");
  Serial.print(m(orientation.pitch)); 
  Serial.print(" Heading: ");
  Serial.println(m(orientation.heading)); 
}

/* Companion board id (can be anything, although AltosUI only displays
 * the first byte.
 */
#define COMPANTION_ID 0x11E1

/* The number of two byte channels we want. These will show up in AltosUI
 * under the "companion" tab
 */
#define NUM_CHANNELS 3

/* The minimum time between requests from the Telemetrum for sample data. 
 * This is documented as being a unit "in 100Hz ticks". 
 * My telemetrum would request data at a rate of ~18.53ms (52.72Hz)
 * when I set this value to 1. However, the
 * board only sends data every 1s on the ground and 10ms while in flight.
 */
#define FETCH_RATE 1 

void setup() {

  Serial.begin (115200); 
  
  accel.begin();
  mag.begin();
  
  AltOSCompanion::configure(COMPANTION_ID, FETCH_RATE, NUM_CHANNELS);

  startTime = millis();
}

void loop() {
  
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  
  if (AltOSCompanion::isConnected() && !companionConnected) {
    Serial.print("Companion Board connected to Telemetrum (");
    Serial.print(AltOSCompanion::serialNumber());
    Serial.print(") Flight #");
    Serial.println(AltOSCompanion::flightNumber());
    companionConnected = true;
  }

  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);
   
  if(companionConnected && dof.fusionGetOrientation(&accel_event, &mag_event, &orientation)) {
    /*if (millis() > startTime + 2000) {
      mapAndPrintOrientation(orientation, map0To360);
      startTime = millis();
    }*/
    AltOSCompanion::updateChannel(0, map0To360(orientation.roll));
    AltOSCompanion::updateChannel(1, map0To360(orientation.pitch));
    AltOSCompanion::updateChannel(2, map0To360(orientation.heading));
  }
}
