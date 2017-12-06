#include "Grid.h"

Grid::Grid(){
	trellis_.begin(TRELLIS_ADDRESS_1, TRELLIS_ADDRESS_2);
	trellis_.setBrightness(TRELLIS_BRIGHTNESS);
	ClearGrid();
	trellis_.writeDisplay();
}

Grid::~Grid(){ }

void Grid::ClearGrid(){
	//Clear the entire grid
	for (int button_num = 0; button_num < TRELLIS_NUM_OF_BUTTONS; button_num++) {
		trellis_led_buffer_[button_num] = false;
	}
}


void Grid::DisplaySingleTrackEditMode() {
	USHORT current_track = p_pattern_->GetCurrentTrack();
	for (USHORT currentStep = 0; currentStep < STEPS_PER_PATTERN; currentStep++) {
		USHORT accent = p_pattern_->GetAccent(current_track, currentStep);
		USHORT probability = p_pattern_->GetProbability(current_track, currentStep);
		trellis_led_buffer_[GetGridNumber(0, currentStep)] = p_pattern_->GetEnableState(current_track, currentStep);
		trellis_led_buffer_[GetGridNumber(1, currentStep)] = LedLightPattern(accent);
		trellis_led_buffer_[GetGridNumber(2, currentStep)] = LedLightPattern(probability);
		trellis_led_buffer_[GetGridNumber(3, currentStep)] = p_pattern_->GetTriggerState(current_track, currentStep);
	}
	//Display Cursor
	int current_led = p_pattern_->GetCursorPosition(current_track);
	trellis_led_buffer_[current_led] = (current_pulse_ / (PULSE_PER_STEP / 2) % 2 == 0);
}

void Grid::DisplayFourTrackEditMode() {
	USHORT current_track = p_pattern_->GetCurrentTrack();
	USHORT current_track_display_row = current_track % TRELLIS_NUM_OF_ROWS;
	USHORT min_track = current_track - current_track_display_row; //The first track to display in this block of tracks
	for (USHORT currentStep = 0; currentStep < STEPS_PER_PATTERN; currentStep++) {
		trellis_led_buffer_[GetGridNumber(0, currentStep)] = p_pattern_->GetEnableState(min_track, currentStep);
		trellis_led_buffer_[GetGridNumber(1, currentStep)] = p_pattern_->GetEnableState(min_track + 1, currentStep);
		trellis_led_buffer_[GetGridNumber(2, currentStep)] = p_pattern_->GetEnableState(min_track + 2, currentStep);
		trellis_led_buffer_[GetGridNumber(3, currentStep)] = p_pattern_->GetEnableState(min_track + 3, currentStep);
	}
	//Display Cursors
	for (USHORT track_to_display_cursor = min_track; track_to_display_cursor < min_track + TRELLIS_NUM_OF_ROWS; track_to_display_cursor++) {
		USHORT current_cursor_position = p_pattern_->GetCursorPosition(track_to_display_cursor);
		trellis_led_buffer_[GetGridNumber(track_to_display_cursor - min_track, current_cursor_position)] = (current_pulse_ / (PULSE_PER_STEP / 2) % 2 == 0);
	}
}


bool Grid::LedLightPattern(USHORT LightParam) {
	// For parameters that range from one to four this contains the logic to control their flashing pattern
	bool led_lit = false;
	switch (LightParam) {
		case 4: led_lit = true; break; //On
		case 3: led_lit = (current_pulse_ % 30 < 15); break; //High pulse
		case 2: led_lit = (current_pulse_ % 30 < 2); break;  //Low pulse
		case 1: led_lit = false; break; //Off
		default: led_lit = false; break;
	}
	return led_lit;
}

void Grid::DisplayFunctionSelectMenu() {
	ClearGrid();
	trellis_led_buffer_[0] = true;
	trellis_led_buffer_[1] = true;
}

void Grid::DisplayPlayingTracks() {
	/* This shows the midi activity of every track while in track select mode.  
	Row 4 shows the cursor and pattern for the currently selected track. */
	USHORT row_4_offset = (TRELLIS_BUTTONS_PER_ROW * 3);
	USHORT current_track = p_pattern_->GetCurrentTrack();
	USHORT cursor_position = p_pattern_->GetCursorPosition(current_track);

	for (int current_led = 0; current_led < (TRELLIS_NUM_OF_BUTTONS); current_led++) {
		//Update the LED to show if a note is playing for the first two rows
		if (current_led < NUM_OF_TRACKS) {
			trellis_led_buffer_[current_led] = p_pattern_->GetTrackNotePlaying(current_led);
		} else if (current_led >= row_4_offset) { //last row will show the enable state for the current tack
			trellis_led_buffer_[current_led] = p_pattern_->GetEnableState(current_track, current_led - row_4_offset);
		} else { //Otherwise just clear it out
			trellis_led_buffer_[current_led] = false;
		}
	}

	//Display Cursor
	trellis_led_buffer_[cursor_position + row_4_offset] = (current_pulse_ / (PULSE_PER_STEP / 2) % 2 == 0) /*Cursor flash pattern*/;
}

void Grid::WriteCurrentPattern() {
	for (int current_led = 0; current_led < TRELLIS_NUM_OF_BUTTONS; current_led++) {
		if (trellis_led_buffer_[current_led]) {
			trellis_.setLED(led_renumber_[current_led]);
		}
		else {
			trellis_.clrLED(led_renumber_[current_led]);
		}
	}
	trellis_.writeDisplay();
}

void Grid::UpdateDisplay(ULONG pulse) {
	current_pulse_ = pulse;
	UpdateSelectButtonDisplay();

	//Update grid
	switch (current_grid_mode_) {
		case kGridModeSingleTrackEdit:	DisplaySingleTrackEditMode(); break;
		case kGridModeFourTrackEdit:	DisplayFourTrackEditMode(); break;
		case kGridModeSelectTrack:		DisplayPlayingTracks();	break;
		case kGridModeSelectFunction:	DisplayFunctionSelectMenu(); break;
		default:						DisplaySingleTrackEditMode(); break;
	}

	WriteCurrentPattern();
}

void Grid::ProcessGridButton(USHORT button_num){
	button_num = button_renumber_[button_num];
	if (current_grid_mode_ == kGridModeSingleTrackEdit) { //Step Toggle
		if (button_num < TRELLIS_BUTTONS_PER_ROW) {
			p_pattern_->ToggleEnableState(p_pattern_->GetCurrentTrack(), button_num);
		}
		else if (button_num < TRELLIS_BUTTONS_PER_ROW * 2) {
			p_pattern_->ToggleAccent(p_pattern_->GetCurrentTrack(), button_num - TRELLIS_BUTTONS_PER_ROW);
		}
		else if (button_num < TRELLIS_BUTTONS_PER_ROW * 3) {
			p_pattern_->ToggleProbability(p_pattern_->GetCurrentTrack(), button_num - (TRELLIS_BUTTONS_PER_ROW * 2));
		}
		else {
			p_pattern_->ToggleTriggerState(p_pattern_->GetCurrentTrack(), button_num - (TRELLIS_BUTTONS_PER_ROW * 3));
		}
	} else if (current_grid_mode_ == kGridModeFourTrackEdit) { //Step Toggle
		USHORT current_track = p_pattern_->GetCurrentTrack();
		USHORT current_track_display_row = current_track % TRELLIS_NUM_OF_ROWS;
		USHORT min_track = current_track - current_track_display_row;
		USHORT pressed_track = min_track + (button_num / TRELLIS_BUTTONS_PER_ROW);
		ushort pressed_col = button_num % TRELLIS_BUTTONS_PER_ROW;
		p_pattern_->ToggleEnableState(pressed_track, pressed_col);
	} else if (current_grid_mode_ == kGridModeSelectTrack) { //Track Select
		//Select a track if one exists and then go back to default edit mode
		if (button_num < NUM_OF_TRACKS) {
			p_pattern_->SetCurrentTrack(button_num);
		}
		//current_grid_mode_ = default_grid_mode_;
	} else if (current_grid_mode_ == kGridModeSelectFunction) { //Function Select
		switch (button_num) {
			case 0: default_grid_mode_ = kGridModeSingleTrackEdit; break;
			case 1: default_grid_mode_ = kGridModeFourTrackEdit; break;
			default: break;
		}
	}
}

void Grid::ReadSwitches()
{
	//Check if throttle time has expired
	long current_millis = millis();
	if (current_millis - throttle_previous_ms_ > TRELLIS_READ_THROTTLE_IN_MS) {
		throttle_previous_ms_ = current_millis;
		
		//Refresh buttons
		track_select_button_.CheckForPress();
		function_select_button_.CheckForPress();

		//Check if we need to switch modes
		if (encoder_button_.IsPressed()) { Serial.println("Encoder Button"); }
		if (track_select_button_.IsPressed() && function_select_button_.IsPressed()) {
			current_grid_mode_ = kGridModeSelectTrackFunction;
		} else if (track_select_button_.IsPressed()) {
			current_grid_mode_ = kGridModeSelectTrack;
		} else if (function_select_button_.IsPressed()) {
			current_grid_mode_ = kGridModeSelectFunction;
		} else {//Return to default mode
			current_grid_mode_ = default_grid_mode_;
		}

		//Read Trellis
		trellis_.readSwitches();
		for (USHORT i = 0; i < TRELLIS_NUM_OF_BUTTONS; i++) {
			if (trellis_.justPressed(i)) { ProcessGridButton(i); }
		}
	}

	//Check for encoder change
	long current_encoder_position_ = encoder_.read();
	if (current_encoder_position_ != 0) {
		encoder_change_amount_ = current_encoder_position_;
		p_clock_->OffsetTempo(encoder_change_amount_);
		encoder_.write(0);
	}
}

void Grid::UpdateSelectButtonDisplay() {
	//Determine mode
	switch (current_grid_mode_) {
	case kGridModeSingleTrackEdit:
		function_select_led_.SetMode(kLedModeOff);
		track_select_led_.SetMode(kLedModeOff);
		break;
	case kGridModeSelectFunction:
		function_select_led_.SetMode(kLedModeFlash);
		track_select_led_.SetMode(kLedModeOff);
		break;
	case kGridModeSelectTrack:
		function_select_led_.SetMode(kLedModeOff);
		track_select_led_.SetMode(kLedModeFlash);
		break;
	case kGridModeSelectTrackFunction:
		function_select_led_.SetMode(kLedModeFlash);
		track_select_led_.SetMode(kLedModeFlash);
		break;
	default:
		function_select_led_.SetMode(kLedModeSolid);
		track_select_led_.SetMode(kLedModeOff);
	}

	//Update button LEDs
	track_select_led_.UpdateDisplay(current_pulse_);
	function_select_led_.UpdateDisplay(current_pulse_);
}