//Original Sample Code:
//https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/*
  bnoSensor is the oreintation/magnatometer sensor class which haddels the data recieved from the sensor 
  and the needed code in order to return the orientation values of the sensor(x, y, z).    
 */

Adafruit_BNO055 bnoSensor = Adafruit_BNO055(55);
 
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bnoSensor.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bnoSensor.setExtCrystalUse(false);
}
 
void loop(void) 
{
  /* Sensor event class contains an orientation component which it self contains xyz components.
 
  /* Get a new sensor event */ 
  sensors_event_t event; 

  /* bnoSensor modifies event throught getEvent(), updating the orientaion component.*/
  bnoSensor.getEvent(&event);
  
  /* Display the floating point data of the orientation componet of event*/
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  
  delay(100);
}
