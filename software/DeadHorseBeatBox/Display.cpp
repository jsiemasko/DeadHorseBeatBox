#include "Display.h"

Display::Display() { }

Display::~Display(){ }

void Display::GraphicsSetup() {
	Serial.println("Graphics Setup");
	oled_.begin();
	oled_.setFlipMode(0);
	oled_.enableUTF8Print();			// enable UTF8 support for the Arduino print() function
	oled_.setFontDirection(0);
	oled_.setFontMode(1);			// Activate transparent font mode
	oled_.setDrawColor(2);
}

void Display::ShowPatternProperties(){
	char digits[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6' };
	const USHORT char_height = 9;
	const USHORT char_width = 8;

	oled_.clearBuffer();
	oled_.setFont(u8g2_font_pressstart2p_8f);
	oled_.setFontPosTop();
	oled_.setCursor(0, 4);
	oled_.setDrawColor(1);
	switch (p_grid_->GetDefaultGridMode()) {
		case kGridModeSelectTrack:		oled_.print("Select Track");	break;
		case kGridModeAccentEdit:		oled_.print("Accent");			break;
		case kGridModeProbabilityEdit:	oled_.print("Probability");		break;
		case kGridModeSkipEdit:			oled_.print("Skip");			break;
		case kGridModeJumpEdit:			oled_.print("Jump");			break;
		case kGridModeRetriggerEdit:	oled_.print("Retrigger");		break;
		case kGridModeNoteEdit:			oled_.print("Note");			break;
		case kGridModeModEdit:			oled_.print("Modulo");			break;
	}
	
	oled_.setCursor(0, 44);		oled_.print("BPM:");
	oled_.setCursor(48, 44);	oled_.print(p_clock_->GetTempo());

	//Tracks
	for (USHORT track = 0; track < NUM_OF_TRACKS; track++) {
		if (p_pattern_->GetTrackNotePlaying(track)) {
			oled_.setDrawColor(1);
			oled_.drawBox(track * char_width, 14, char_width, char_height);
			oled_.setDrawColor(0);
			oled_.setCursor(track * char_width, 14);
			oled_.print(digits[track]);
			oled_.setDrawColor(1);
		}
		else {
			oled_.setDrawColor(1);
			oled_.setCursor(track * char_width, 14);
			oled_.print(digits[track]);
		}
	}

	//Steps
	for (USHORT step = 0; step < STEPS_PER_PATTERN; step++) {
		if (p_pattern_->GetCursorPosition(p_pattern_->GetCurrentTrack()) == step) {
			oled_.setDrawColor(1);
			oled_.drawBox(step * char_width, 23, char_width, char_height);
			oled_.setDrawColor(0);
			oled_.setCursor(step * char_width, 23);
			oled_.print(digits[step]);
			oled_.setDrawColor(1);
		}
		else {
			oled_.setDrawColor(1);
			oled_.setCursor(step * char_width, 23);
			oled_.print(digits[step]);
		}
	}

	oled_.setCursor(0, 54);		oled_.print("A P R N M J S");
	oled_.sendBuffer();
}

void Display::SplashHorse(){
	oled_.clearBuffer();
	Serial.print("Splash Screen");
	oled_.drawXBMP(0, 0, 128, 64, dedhors_bits);
	oled_.setFont(u8g2_font_timB10_tf);
	oled_.setDrawColor(1);
	oled_.drawStr(95, 64, "v0.01");
	oled_.sendBuffer();
	delay(DISPLAY_SPLASH_SCREEN_DISPLAY_TIME);
	oled_.clearBuffer();
	oled_.sendBuffer();
}

void Display::UpdateDisplay(ULONG pulse) {
	if (display_mode_ == kDisplayModePatternProperties) {
		ShowPatternProperties();
	} else {
		oled_.clear();
	}
}