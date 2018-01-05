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
	USHORT current_track = p_pattern_->GetCurrentTrack();
	ShowPageHeader();
	ShowBargraph(16, 23, 1);
	ShowTracks(40, current_track);
	ShowSteps(48, current_track);
}

void Display::ShowTrackSelectDisplay() {
/*
	USHORT current_mode = 0;

	ShowPageHeader();

	oled_.setFont(u8g2_font_pressstart2p_8f);
	oled_.setFontPosTop();

	switch (default_grid_mode_) {
		case kGridModeAccentEdit: current_mode = 0; break;
		case kGridModeProbabilityEdit: current_mode = 1; break;
		case kGridModeSkipEdit: current_mode = 5; break;
		case kGridModeJumpEdit: current_mode = 4; break;
		case kGridModeRetriggerEdit: current_mode = 2; break;
		case kGridModeNoteEdit: current_mode = 3; break;
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
	*/
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
		default: break;
	}

	oled_.setCursor(7 * kCharWidth, 4);
	oled_.print("BPM:");
	oled_.setCursor(11 * kCharWidth, 4);
	oled_.print(p_clock_->GetTempo());

	//Seperator
	oled_.drawHLine(0, 14, 132);
}

void Display::ShowSteps(USHORT y_offset, USHORT track) {
	for (USHORT step = 0; step < STEPS_PER_PATTERN; step++) {
		oled_.setDrawColor(1);

		USHORT x_offset = step * kCharWidth;
		bool step_skipped = p_pattern_->GetSkipState(track, step);
		bool step_enabled = p_pattern_->GetEnableState(track, step);
		bool step_accented = p_pattern_->GetAccent(track, step);
		bool step_chance_set = p_pattern_->GetProbability(track, step);
		bool step_retriggered = (p_pattern_->GetBurstMultiplier(track, step) > 1);

		//Seperator lines every 4 steps
		if (step % 4 == 3) {
			oled_.drawPixel(x_offset + kCharWidth, y_offset + 2);
			oled_.drawPixel(x_offset + kCharWidth, y_offset + 5);
		}

		//Cursor
		if (step == p_pattern_->GetCursorPosition(track)) {
			oled_.drawHLine(x_offset + 1, y_offset + kCharHeight - 1, 5);
		}

		if (step_skipped) {
			; //Draw nothing, step is skipped
		}
		else if (!step_enabled) {
			oled_.drawPixel(x_offset + 4, y_offset + 4); //Just a dot
		}
		else {
			DrawStepBox(x_offset, y_offset, step_accented, step_chance_set, step_retriggered); //Draw a stepbox
		}
	}
}

void Display::DrawStepBox(USHORT x, USHORT y, bool step_accented, bool step_chance_set, bool step_retriggered) {
	oled_.setDrawColor(1);

	//Determine size and offset of box based on accent
	USHORT offset = step_accented ? 2 : 3;
	USHORT size = step_accented ? 5: 3;

	//Determine if box is filled based on chance
	if (step_chance_set) {//Chance
		oled_.drawFrame(x + offset, y + offset, size, size);
	}
	else {//Normal
		oled_.drawBox(x + offset, y + offset, size, size);
	}

	//Retrigger erase lines to show a retrigger
	if (step_retriggered) {
		oled_.setDrawColor(0);
		oled_.drawVLine(x + 4, y + 2, 5);

		//If the step box had chance set then we also erase a horizontal line on a retrigger
		if (step_chance_set) {
			oled_.drawHLine(x + 2, y + 4, 5);
		}
	}
}

void Display::ShowTracks(USHORT y_offset, USHORT current_track) {
	oled_.setFont(u8g2_font_pressstart2p_8f);
	for (USHORT track = 0; track < NUM_OF_TRACKS; track++) {
		USHORT x_offset = track * kCharWidth;
		oled_.setDrawColor(1);
		if (current_track == track) {
			//if track is selected use the main color for the box and invert the color for drawing text
			oled_.drawBox(x_offset, y_offset, kCharWidth, kCharHeight);
			oled_.setDrawColor(0);
		}
		oled_.setCursor(x_offset, y_offset);
		oled_.print(track_labels[track]);
	}
}

void Display::ShowBargraph(USHORT y_offset, USHORT height, USHORT fall_speed) {
	//Seperator 1
	oled_.drawPixel(32, y_offset + 4);
	oled_.drawPixel(32, y_offset + 14);

	//Seperator 2
	oled_.drawPixel(64, y_offset + 4);
	oled_.drawPixel(64, y_offset + 14);

	//Seperator 3
	oled_.drawPixel(96, y_offset + 4);
	oled_.drawPixel(96, y_offset + 14);

	//Offset adjust
	y_offset += height;

	oled_.setDrawColor(1);
	
	for (USHORT track = 0; track < NUM_OF_TRACKS; track++) {
		USHORT x_offset = track * kCharWidth + 1;
		if (p_pattern_->GetTrackNotePlaying(track)) {
			//If a note is playing set the bargraph to the top and fill it
			bargraph[track] = height;
			oled_.drawBox(x_offset, y_offset - bargraph[track], kCharWidth - 2, bargraph[track]);
		} else if (bargraph[track] > 0) {
			//If a note is not playing but the bargrah is not empty then make it hollow and let it fall
			bargraph[track] = bargraph[track] - fall_speed;
			oled_.drawFrame(x_offset, y_offset - bargraph[track], kCharWidth - 2, bargraph[track]);
		} else {
			; //Bargraph is empty, do nothing
		}
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
	default_grid_mode_ = p_grid_->GetDefaultGridMode();

	oled_.clearBuffer();
	if (display_mode_ == kDisplayModePatternProperties) {
		//if (p_grid_->GetGridMode() == kGridModeSelectTrack) {
		//	ShowTrackSelectDisplay();
		//} else {
			ShowPatternProperties();
		//}
	}
	oled_.sendBuffer();
}