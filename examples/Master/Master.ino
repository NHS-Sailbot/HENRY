#include <GPS.h>

HENRY::GPS gps;

void setup()
{
	gps.begin();
}

void loop()
{
	// update the modules
	gps.update();

	// output
	Serial.print("Longitude: "); Serial.print(gps.getLongitude(), 6);
	Serial.print("  Latitude: "); Serial.println(gps.getLatitude(), 6);

	
}