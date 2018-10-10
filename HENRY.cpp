#include "HENRY.h"

#include "Arduino.h"

namespace HENRY
{
	/* A function that takes in a set of data
	that represents an image and it's width,
	and it prints it to the serial monitor
	in isometric 3D.

		for example:
	------------------------------
		const char * image =
		" # # # #"
		"# # # # "
		"  ##  ##"
		"##  ##  ";

		Ascii_ISO_3D(image, 8);
	------------------------------*/
	int Ascii_ISO_3D(const char * data, const unsigned short width)
	{
		/*---------------------
		|   bullet proofing   |
		---------------------*/
		/* run through the char set until it 
		finds an end character. */
		unsigned int dataLen = 0;
		while (data[dataLen] != '\0')
			dataLen++;
		/* if the number of characters in the 
		input data is indivisible by the input
		width, return a failure code, ending the
		execution of the function. */
		if (dataLen % width != 0)
		{
			Serial.println("ERROR::ASCII_ISO_3D: Data does not match width!");
			Serial.println("  Possible causes: input width is not the width of the image");
			Serial.println("                   char set does not have a constant width");
			return 0;
		}
		/*---------------------
		|   drawing process   |
		---------------------*/
		/* calculate the height of the "image" by 
		dividing the number of characters in the 
		image by the input width. */
		const unsigned short height = dataLen / width;
		/* naive way of having an adjustable value. */
		const unsigned short scale = 1;
		/* run through all of the input characters. */
		for (unsigned short y = 0; y < height * scale; y++)
		{
			for (unsigned short x = 0; x < width * scale; x++)
			{
				/* test all the values to change the representation 
				of the image to be isometric 3D. */
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
			/* Start a new line after all the characters 
			have been printed in the line. */
			Serial.println(' ');
			/* offset all the lines at the beginning 
			to make it look slanted. */
			for (unsigned short i = 0; i < y + 1; i++)
				Serial.print(' ');
		}
		return 1;
	}
}