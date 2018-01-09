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
	Track& r_current_track = p_pattern_->GetCurrentTrack();
	for (USHORT current_step = 0; current_step < STEPS_PER_PATTERN; current_step++) {
		Step& r_current_step = r_current_track.GetStep(current_step);

		//Determine which param to show based on the edit mode
		bool edit_param_value = false;
		switch (current_grid_mode_) {
			case kGridModeAccentEdit: edit_param_value = r_current_step.GetAccentState(); break;
			case kGridModeChanceEdit: edit_param_value = r_current_step.GetChanceState(); break;
			case kGridModeRetriggerEdit: edit_param_value = r_current_step.GetRetriggerState(); break;
			case kGridModeSkipEdit: edit_param_value = r_current_step.GetSkipState(); break;
			case kGridModeNoteEdit: edit_param_value = r_current_step.GetNoteState(); break;
			//Jump
			default: edit_param_value = false;  break;
		}
		trellis_led_buffer_[GetGridNumber(0, current_step)] = r_current_step.GetEnableState();
		trellis_led_buffer_[GetGridNumber(1, current_step)] = edit_param_value;
	}
	//Display Cursor
	int current_led = r_current_track.GetCursorPosition();
	bool cursor_display_state = (current_pulse_ / (PULSE_PER_STEP / 2) % 2 == 0);
	trellis_led_buffer_[current_led] = cursor_display_state; //Cursor 1
	trellis_led_buffer_[current_led + STEPS_PER_PATTERN] = cursor_display_state; //Cursor 1
}

void Grid::DisplayPlayingTracks() {
	// This shows the midi activity of every track while in track select mode.  
	for (int current_led = 0; current_led < (TRELLIS_NUM_OF_BUTTONS); current_led++) {
		//Update the LED to show if a note is playing for the first two rows
		if (current_led < NUM_OF_TRACKS) {
			trellis_led_buffer_[current_led] = p_pattern_->GetTrackNotePlaying(current_led);
		} else { //Otherwise just clear it out
			trellis_led_buffer_[current_led] = false;
		}
	}

	//Show currently selected mode LED
	switch (default_grid_mode_) {
		case kGridModeAccentEdit: trellis_led_buffer_[16] = true; break;
		case kGridModeChanceEdit: trellis_led_buffer_[17] = true; break;
		case kGridModeRetriggerEdit: trellis_led_buffer_[18] = true; break;
		case kGridModeNoteEdit: trellis_led_buffer_[19] = true; break;
		case kGridModeJumpEdit: trellis_led_buffer_[20] = true; break;
		case kGridModeSkipEdit: trellis_led_buffer_[21] = true; break;
		default: break;
	}	
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
		case kGridModeSelectTrack:		DisplayPlayingTracks();	break;
		default:						DisplaySingleTrackEditMode(); break;
	}

	WriteCurrentPattern();
}

void Grid::ProcessGridButton(USHORT button_num){
	button_num = button_renumber_[button_num];
	Track& r_current_track = p_pattern_->GetCurrentTrack();
	if (current_grid_mode_ == kGridModeSelectTrack) { //Track Select
													  //Select a track if one exists and then go back to default edit mode
		if (button_num < NUM_OF_TRACKS) {
			p_pattern_->SetCurrentTrack(button_num);
		}
		else {
			switch (button_num) {
			case 16: default_grid_mode_ = kGridModeAccentEdit;		break; //Accent
			case 17: default_grid_mode_ = kGridModeChanceEdit;	break; //Probability 
			case 18: default_grid_mode_ = kGridModeRetriggerEdit;	break; //Retrigger
			case 19: default_grid_mode_ = kGridModeNoteEdit;		break; //Note 
			case 20: default_grid_mode_ = kGridModeJumpEdit;		break; //Jump 
			case 21: default_grid_mode_ = kGridModeSkipEdit;		break; //Skip
			}
		}
	} else {
		if (button_num < TRELLIS_BUTTONS_PER_ROW) {
			r_current_track.GetStep(button_num).ToggleEnableState();
		}
		else if (button_num < TRELLIS_BUTTONS_PER_ROW * 2) {
			//Get the step, offsetting for the fact we using the second row of buttons
			Step& r_current_step = r_current_track.GetStep(button_num - TRELLIS_BUTTONS_PER_ROW);

			switch (current_grid_mode_){
				case kGridModeAccentEdit: r_current_step.ToggleAccentState(); break;
				case kGridModeChanceEdit: r_current_step.ToggleChanceState(); break;
				case kGridModeSkipEdit: r_current_step.ToggleSkipState(); break;
				case kGridModeRetriggerEdit: r_current_step.ToggleRetriggerState(); break;
				case kGridModeNoteEdit: r_current_step.ToggleNoteState(); break;
				case kGridModeJumpEdit: break; //TODO: this.
				default: break; //Unknown mode, do nothing
			}
		}
	} 
}

void Grid::ReadSwitches() {
	//Check if throttle time has expired, if not then bail out
	long current_millis = millis();
	if (current_millis - throttle_previous_ms_ < TRELLIS_READ_THROTTLE_IN_MS) { return; }

	throttle_previous_ms_ = current_millis;
		
	//Refresh buttons
	track_select_button_.CheckForPress();
	function_select_button_.CheckForPress();
	encoder_button_.CheckForPress();

	//Check if we need to switch modes
	if (track_select_button_.IsPressed()) {
		current_grid_mode_ = kGridModeSelectTrack;
	} else if (function_select_button_.IsPressed()) {
		;//current_grid_mode_ = kGridModeSelectFunction;
	} else {//Return to default mode
		current_grid_mode_ = default_grid_mode_;
	}

	//Read Trellis
	trellis_.readSwitches();
	for (USHORT i = 0; i < TRELLIS_NUM_OF_BUTTONS; i++) {
		if (trellis_.justPressed(i)) { ProcessGridButton(i); }
	}

	//Take an encoder reading, if we have accumulated enough then send take an action and reset our accumulation
	long current_encoder_position_ = encoder_.read();
	if (current_encoder_position_ >= ENCODER_SCALE || current_encoder_position_ <= -ENCODER_SCALE) {
		p_clock_->OffsetTempo(current_encoder_position_ / ENCODER_SCALE);
		encoder_.write(0);
	}
}


void Grid::UpdateSelectButtonDisplay() {
	LedMode current_led_mode = (current_grid_mode_ == kGridModeSelectTrack) ? kLedModeFlash : kLedModeOff;
	track_select_led_.SetMode(current_led_mode);
	track_select_led_.UpdateDisplay(current_pulse_);
}