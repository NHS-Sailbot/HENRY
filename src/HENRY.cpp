#include "HENRY.h"

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

static const char dataRows[7][27][6] = {
	{"     ", " ### ", "#### ", " ####", "#### ", "#####", "#####", " ####", "#   #", "#####", "#####", "#   #", "#    ", "#   #", "#   #", " ### ", "#### ", " ### ", "#### ", " ### ", "#####", "#   #", "#   #", "#   #", "#   #", "#   #", "#####"},
	{"     ", "#   #", "#   #", "#    ", "#   #", "#    ", "#    ", "#    ", "#   #", "  #  ", "   # ", "#  # ", "#    ", "## ##", "##  #", "#   #", "#   #", "#   #", "#   #", "#   #", "  #  ", "#   #", "#   #", "#   #", "#   #", "#   #", "    #"},
	{"     ", "#   #", "#   #", "#    ", "#   #", "#    ", "#    ", "#    ", "#   #", "  #  ", "   # ", "# #  ", "#    ", "# # #", "# # #", "#   #", "#   #", "#   #", "#   #", "#    ", "  #  ", "#   #", "#   #", "#   #", " # # ", " # # ", "   # "},
	{"     ", "#####", "#### ", "#    ", "#   #", "#### ", "#### ", "# ###", "#####", "  #  ", "   # ", "##   ", "#    ", "#   #", "#  ##", "#   #", "#### ", "#   #", "#### ", " ### ", "  #  ", "#   #", "#   #", "#   #", "  #  ", "  #  ", "  #  "},
	{"     ", "#   #", "#   #", "#    ", "#   #", "#    ", "#    ", "#   #", "#   #", "  #  ", "#  # ", "# #  ", "#    ", "#   #", "#   #", "#   #", "#    ", "#   #", "# #  ", "    #", "  #  ", "#   #", " # # ", "# # #", " # # ", "  #  ", " #   "},
	{"     ", "#   #", "#   #", "#    ", "#   #", "#    ", "#    ", "#   #", "#   #", "  #  ", "#  # ", "#  # ", "#    ", "#   #", "#   #", "#   #", "#    ", "#  ##", "#  # ", "#   #", "  #  ", "#   #", " # # ", "# # #", "#   #", "  #  ", "#    "},
	{"     ", "#   #", "#### ", " ####", "#### ", "#####", "#    ", " ### ", "#   #", "#####", " ##  ", "#   #", "#####", "#   #", "#   #", " ### ", "#    ", " ####", "#   #", " ### ", "  #  ", " ### ", "  #  ", " # # ", "#   #", "  #  ", "#####"}};

bool Ascii_ISO_3D(const char *message)
{
	const unsigned int messageLength = strlen(message),
					   dataWidth = messageLength * 7 - 1,
					   dataLength = dataWidth * 8;
	char data[dataLength];

	for (unsigned int i = 0; i < dataLength; ++i)
		data[i] = ' ';

	for (unsigned int line = 0; line < 7; ++line)
	{
		for (unsigned int letter = 0; letter < messageLength; ++letter)
		{
			unsigned int index = letter * 7 + dataWidth * line;
			unsigned int character;

			if (message[letter] > 64 && message[letter] < 91)
				character = message[letter] - 64;
			else if (message[letter] > 96 && message[letter] < 123)
				character = message[letter] - 96;
			else if (message[letter] == ' ')
				character = 0;
			else
				return 1;

			for (unsigned int i = 0; i < 5; ++i)
				data[index + i + 1] = dataRows[line][character][i];
		}
	}

	data[dataLength] = '\0';

	return Ascii_ISO_3D(data, dataWidth);
}

bool Ascii_ISO_3D(const char *data, const unsigned short width)
{
	//---------------------
	//|  bullet proofing  |
	//---------------------
	// run through the char set until it 
	// finds an end character.
	unsigned int dataLen = 0;
	while (data[dataLen] != '\0')
		dataLen++;
	// if the number of characters in the 
	// input data is indivisible by the input
	// width, return a failure code, ending the
	// execution of the function.
	if (dataLen % width != 0)
	{
		Serial.print("ERROR: Ascii_ISO_3D(...) failed to recognize image size!\n");
		return 1;
	}
	//---------------------
	//|  drawing process  |
	//---------------------
	// calculate the height of the "image" by 
	// dividing the number of characters in the 
	// image by the input width.
	const unsigned short height = dataLen / width;
	// naive way of having an adjustable value.
	const unsigned short scale = 1;
	// run through all of the input characters.
	for (unsigned short y = 0; y < height * scale; y++)
	{
		for (unsigned short x = 0; x < width * scale; x++)
		{
			// test all the values to change the representation 
			// of the image to be isometric 3D.
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
						Serial.print("\\ ");
				}
				else
				{
					if (data[currentIndex - width] == '#')
					{
						if (currentIndex == dataLen - 1)
							Serial.print("/ ");
						else
							Serial.print("//");
					}
					else
					{
						if (currentIndex == dataLen - 1)
							Serial.print("  ");
						else
							Serial.print(" /");
					}
				}
			}
		}
		// Start a new line after all the characters 
		// have been printed in the line.
		Serial.print("\n");
		// offset all the lines at the beginning 
		// to make it look slanted.
		for (unsigned short i = 0; i < y + 1; i++)
			Serial.print(" ");
	}
	Serial.print("\n");
	return 0;
}
} // namespace HENRY