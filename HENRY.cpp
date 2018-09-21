#include "HENRY.h"

#include "Arduino.h"

namespace HENRY
{
	int Ascii_ISO_3D(const char * data, const unsigned short width)
	{
		// bullet proofing
		unsigned int dataLen = 0;
		while (data[dataLen] != '\0')
			dataLen++;
		if (dataLen % width != 0)
		{
			Serial.println("ERROR::ASCII_ISO_3D: Data does not match width!");
			Serial.println("  Possible causes: input width is not the width of the image");
			Serial.println("                   char set does not have a constant width");
			return 0;
		}
		// start the drawing process
		const unsigned short height = dataLen / width;
		const unsigned short scale = 1;
		for (unsigned short y = 0; y < height * scale; y++)
		{
			for (unsigned short x = 0; x < width * scale; x++)
			{
				unsigned short currentIndex = x / scale + y / scale * width;
				if (data[currentIndex] == '#')
					Serial.print("\\\\");
				else
				{
					if (data[currentIndex + 1] == ' ')
					{
						if (data[currentIndex - width] == '#')
						{
							if (data[currentIndex - width + 1] == ' ')
								Serial.print("/ ");
							else
								Serial.print("//");
						}
						else if (data[currentIndex - width + 1] == '#')
							Serial.print("\\/");
						else
							Serial.print("  ");
					}
					else if (data[currentIndex - width + 1] == '#')
					{
						if (data[currentIndex - width] == '#')
							Serial.print("//");
						else
							Serial.print("\\/");
					}
					else
					{
						if (data[currentIndex - width] == '#')
							Serial.print("//");
						else
							Serial.print(" /");
					}
				}
			}
			Serial.println(' ');
			for (unsigned short i = 0; i < y + 1; i++)
				Serial.print(' ');
		}
		return 1;
	}
}