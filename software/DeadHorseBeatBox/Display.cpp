#include "Display.h"

Display::Display(MidiManager * p_midi_manager) {
	p_midi_manager_ = p_midi_manager;
}

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
	USHORT current_track = p_pattern_->GetCurrentTrackNumber();
	ShowPageHeader();
	ShowBargraph(16, 23, 14, 1);
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
		case kGridModeChanceEdit: current_mode = 1; break;
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
		case kGridModeChanceEdit: oled_.print("Chance"); break;
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
	Track& r_current_track = p_pattern_->GetTrack(track);
	for (USHORT step = 0; step < STEPS_PER_PATTERN; step++) {
		oled_.setDrawColor(1);

		USHORT x_offset = step * kCharWidth;
		Step& r_current_step = r_current_track.GetStep(step);
		bool step_skipped = r_current_step.GetSkipState();
		bool step_enabled = r_current_step.GetEnableState();
		bool step_accented = r_current_step.GetAccentState();
		bool step_chance_set = r_current_step.GetChanceState();
		bool step_retriggered = r_current_step.GetRetriggerState();

		//Seperator lines every 4 steps
		if (step % 4 == 3) {
			oled_.drawPixel(x_offset + kCharWidth, y_offset + 3);
			oled_.drawPixel(x_offset + kCharWidth, y_offset + 7);
		}

		//Cursor
		if (step == r_current_track.GetCursorPosition()) {
			oled_.drawHLine(x_offset + 1, y_offset + kCharHeight + 1, 5);
		}

		if (step_skipped) {
			; //Draw nothing, step is skipped
		}
		else if (!step_enabled) {
			oled_.drawPixel(x_offset + 4, y_offset + 5); //Just a dot
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
	USHORT x_size = step_accented ? 5 : 3;
	USHORT y_size = step_accented ? 7 : 5;


	//Determine if box is filled based on chance
	if (step_chance_set) {//Chance
		oled_.drawFrame(x + offset, y + offset, x_size, y_size);
	}
	else {//Normal
		oled_.drawBox(x + offset, y + offset, x_size, y_size);
	}

	//Retrigger erase lines to show a retrigger
	if (step_retriggered) {
		oled_.setDrawColor(0);
		oled_.drawVLine(x + 4, y + offset, y_size);

		//If the step box had chance set then we also erase a horizontal line on a retrigger
		if (step_chance_set) {
			oled_.drawHLine(x + offset, y + 5, x_size);
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

void Display::ShowBargraph(USHORT y_offset, USHORT accent_height, USHORT std_height, USHORT fall_speed) {
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
	y_offset += accent_height;

	oled_.setDrawColor(1);

	for (USHORT track = 0; track < NUM_OF_TRACKS; track++) {
		USHORT x_offset = track * kCharWidth + 1;
		MidiEvent& r_event = p_midi_manager_->GetEvent(track);
		if (r_event.Playing) {
			//If a note is playing set the bargraph to appropriate height and and fill it
			bargraph[track] = (r_event.Velocity > 100) ? accent_height : std_height ;
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
	Serial.println("Splash Screen");
	oled_.setDrawColor(1);
	oled_.drawXBMP(0, 0, 128, 64, dedhors_bits);
	oled_.setFont(u8g2_font_timB10_tf);
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