#include "DisplayView.h"

namespace Display {
	DisplayView::DisplayView() 
	{
		oled_.begin();
		oled_.setFlipMode(0);
		oled_.enableUTF8Print();	// enable UTF8 support for the Arduino print() function
		oled_.setFontDirection(0);
		oled_.setFontMode(1);		// Activate transparent font mode
	}

	DisplayView::~DisplayView() {}

	void DisplayView::DisplaySplashScreen()
	{
		oled_.clearBuffer();
		oled_.setDrawColor(1);
		oled_.drawXBMP(0, 0, 128, 64, dedhors_bits);
		oled_.setFont(u8g2_font_timB10_tf);
		oled_.drawStr(95, 64, "v0.01");
		oled_.sendBuffer();
	}

	void DisplayView::DisplayPageHeader(USHORT x, USHORT y, String current_parameter, float tempo)
	{
		oled_.setFont(u8g2_font_pressstart2p_8f);
		oled_.setFontPosTop();
		oled_.setDrawColor(1);
		oled_.setCursor(x + 0, y + 4);
		oled_.print(current_parameter);
		oled_.setCursor(x + (7 * kCharWidth), y + 4);
		oled_.print("BPM:");
		oled_.setCursor(x + (11 * kCharWidth), y + 4);
		oled_.print(tempo);
		oled_.drawHLine(x + 0, y + 14, 132);
	}

	void DisplayView::DisplayStepBox(USHORT step, USHORT y, bool step_enabled, bool step_accented, bool step_chance_set, bool step_retriggered)
	{
		oled_.setDrawColor(1);

		USHORT x = step * kCharWidth;

		//Determine size and offset of box based on accent
		USHORT offset = step_accented ? 2 : 3;
		USHORT x_size = step_accented ? 5 : 3;
		USHORT y_size = step_accented ? 7 : 5;

		if (!step_enabled)
		{
			oled_.drawPixel(x + 4, y + 5); //Just a dot
		}
		else
		{
			//Determine if box is filled based on chance
			if (step_chance_set)
			{
				oled_.drawFrame(x + offset, y + offset, x_size, y_size);
			}
			else
			{
				oled_.drawBox(x + offset, y + offset, x_size, y_size);
			}

			//Retrigger erase lines to show a retrigger
			if (step_retriggered)
			{
				oled_.setDrawColor(0);
				oled_.drawVLine(x + 4, y + offset, y_size);

				//If the step box had chance set then we also erase a horizontal line on a retrigger
				if (step_chance_set)
				{
					oled_.drawHLine(x + offset, y + 5, x_size);
				}
			}
		}
	}

	void DisplayView::DisplayStepBackground(USHORT y)
	{
		for (USHORT step_number = 0; step_number < STEPS_PER_PATTERN; step_number++)
		{
			USHORT x = step_number * kCharWidth;
			//Seperator lines every 4 steps
			if (step_number % 4 == 3) 
			{
				oled_.drawPixel(x + kCharWidth, y + 3);
				oled_.drawPixel(x + kCharWidth, y + 7);
			}

		}
	}

	void DisplayView::DisplayStepCursor(USHORT step, USHORT y)
	{
		USHORT x = step * kCharWidth;
		oled_.drawHLine(x + 1, y + kCharHeight + 1, 5);
	}
}