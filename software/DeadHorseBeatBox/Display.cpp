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
	ShowPageHeader();
	oled_.setCursor(0, 44);		oled_.print("BPM:");
	oled_.setCursor(40, 44);	oled_.print(p_clock_->GetTempo());
	ShowTracks(15);
	ShowSteps(24);
}

void Display::ShowTrackSelectDisplay() {
	USHORT current_mode = 0;

	ShowPageHeader();

	oled_.setFont(u8g2_font_pressstart2p_8f);
	oled_.setFontPosTop();

	switch (default_grid_mode_) {
		case kGridModeAccentEdit: current_mode = 0; break;
		case kGridModeProbabilityEdit: current_mode = 1; break;
		case kGridModeSkipEdit: current_mode = 6; break;
		case kGridModeJumpEdit: current_mode = 5; break;
		case kGridModeRetriggerEdit: current_mode = 2; break;
		case kGridModeNoteEdit: current_mode = 3; break;
		case kGridModeModEdit: current_mode = 4; break;
		default: current_mode = 0; break;
	}

	//Modes
	for (USHORT mode = 0; mode < kNumOfModes; mode++) {
		if (mode == current_mode) {
			oled_.setDrawColor(1);
			oled_.drawBox(mode * kCharWidth, 14, kCharWidth, kCharHeight);
			oled_.setDrawColor(0);
			oled_.setCursor(mode * kCharWidth, 14);
			oled_.print(mode_initials[mode]);
			oled_.setDrawColor(1);
		}
		else {
			oled_.setDrawColor(1);
			oled_.setCursor(mode * kCharWidth, 14);
			oled_.print(mode_initials[mode]);
		}
	}
	oled_.setCursor(0, 44);		oled_.print("BPM:");
	oled_.setCursor(40, 44);	oled_.print(p_clock_->GetTempo());

	ShowSteps(24);
}

void Display::ShowPageHeader() {
	oled_.setFont(u8g2_font_pressstart2p_8f);
	oled_.setFontPosTop();
	oled_.setDrawColor(1);

	//Mode full name
	oled_.setCursor(0, 4);
	switch (default_grid_mode_) {
		case kGridModeAccentEdit: oled_.print("Accent"); break;
		case kGridModeProbabilityEdit: oled_.print("Chance"); break;
		case kGridModeSkipEdit: oled_.print("Skip"); break;
		case kGridModeJumpEdit: oled_.print("Jump"); break;
		case kGridModeRetriggerEdit: oled_.print("Retrig"); break;
		case kGridModeNoteEdit: oled_.print("Note"); break;
		case kGridModeModEdit: oled_.print("Modulo"); break;
		default: break;
	}

	oled_.setCursor(7 * kCharWidth, 4);
	oled_.print("Trk:");
	oled_.setCursor(7 * kCharWidth, 4);

	//Seperator
	oled_.drawHLine(0, 13, 132);
}

void Display::ShowSteps(USHORT y_offset) {
	for (USHORT step = 0; step < STEPS_PER_PATTERN; step++) {
		if (current_step_ == step) {
			oled_.setDrawColor(1);
			oled_.drawBox(step * kCharWidth, y_offset, kCharWidth, kCharHeight);
			oled_.setDrawColor(0);
			oled_.setCursor(step * kCharWidth, y_offset);
			oled_.print(single_digits[step]);
			oled_.setDrawColor(1);
		}
		else {
			oled_.setDrawColor(1);
			oled_.setCursor(step * kCharWidth, y_offset);
			oled_.print(single_digits[step]);
		}
	}
}

void Display::ShowTracks(USHORT y_offset) {
	for (USHORT track = 0; track < NUM_OF_TRACKS; track++) {
		oled_.setDrawColor(1);
		if (p_pattern_->GetTrackNotePlaying(track)) {
			oled_.drawFrame(track * kCharWidth, y_offset, kCharWidth, kCharHeight + 1);
		}
		if (current_track_ == track) {
			oled_.drawBox(track * kCharWidth, y_offset, kCharWidth, kCharHeight);
			oled_.setDrawColor(0);
		}
		oled_.setCursor(track * kCharWidth, y_offset);
		oled_.print(single_digits[track]);
	}
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
	current_track_ = p_pattern_->GetCurrentTrack();
	current_step_ = p_pattern_->GetCursorPosition(current_track_);
	default_grid_mode_ = p_grid_->GetDefaultGridMode();

	oled_.clearBuffer();
	if (display_mode_ == kDisplayModePatternProperties) {
		if (p_grid_->GetGridMode() == kGridModeSelectTrack) {
			ShowTrackSelectDisplay();
		} else {
			ShowPatternProperties();
		}
	}
	oled_.sendBuffer();
}