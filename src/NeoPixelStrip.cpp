#include "NeoPixelStrip.h"

#include <Adafruit_NeoPixel.h>

namespace HENRY {
	namespace NeoPixelStrip {
		static Adafruit_NeoPixel s_strip;
		constexpr static const byte NUMPIXELS = 15, PIN = 10;

		uint init()
		{
			s_strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
			s_strip.begin();
		}
		static unsigned long s_millisDelay = 0;
		void setColor(Math::cvec4 color)
		{
			unsigned long currentMillis = millis();
			uint col = *(int *)&color;
			if (currentMillis - s_millisDelay > 500) {
				s_millisDelay += 500;
				for (uint i = 0; i < NUMPIXELS; ++i) {
					s_strip.setPixelColor(i, col);
					s_strip.show();
				}
			}
		}
	} // namespace NeoPixelStrip
} // namespace HENRY
