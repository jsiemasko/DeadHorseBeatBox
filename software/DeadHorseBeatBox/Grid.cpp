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
		
		//Determine which param to show based on the edit mode
		bool edit_param_value = false;
		switch (current_grid_mode_) {
			case kGridModeAccentEdit: edit_param_value = p_pattern_->GetAccent(current_track, currentStep); break;
			case kGridModeProbabilityEdit: edit_param_value = p_pattern_->GetProbability(current_track, currentStep); break;
			//case kGridModeRetriggerEdit: edit_param_value = p_pattern_->GetAccent(current_track, currentStep); break;
			case kGridModeSkipEdit: edit_param_value = p_pattern_->GetSkipState(current_track, currentStep); break;
			//case kGridModeNoteEdit: edit_param_value = p_pattern_->GetAccent(current_track, currentStep); break;
			//case kGridModeModEdit: edit_param_value = p_pattern_->GetAccent(current_track, currentStep); break;
			//Jump
			default: edit_param_value = false;  break;
		}
		trellis_led_buffer_[GetGridNumber(0, currentStep)] = p_pattern_->GetEnableState(current_track, currentStep);
		trellis_led_buffer_[GetGridNumber(1, currentStep)] = edit_param_value;
	}
	//Display Cursor
	int current_led = p_pattern_->GetCursorPosition(current_track);
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
		case kGridModeProbabilityEdit: trellis_led_buffer_[17] = true; break;
		case kGridModeRetriggerEdit: trellis_led_buffer_[18] = true; break;
		case kGridModeNoteEdit: trellis_led_buffer_[19] = true; break;
		case kGridModeModEdit: trellis_led_buffer_[20] = true; break;
		case kGridModeJumpEdit: trellis_led_buffer_[21] = true; break;
		case kGridModeSkipEdit: trellis_led_buffer_[22] = true; break;
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
	if (current_grid_mode_ == kGridModeAccentEdit) { //Step Toggle
		if (button_num < TRELLIS_BUTTONS_PER_ROW) {
			p_pattern_->ToggleEnableState(p_pattern_->GetCurrentTrack(), button_num);
		}
		else if (button_num < TRELLIS_BUTTONS_PER_ROW * 2) {
			p_pattern_->ToggleAccent(p_pattern_->GetCurrentTrack(), button_num - TRELLIS_BUTTONS_PER_ROW);
		}
/*
		else if (button_num < TRELLIS_BUTTONS_PER_ROW * 3) {
			p_pattern_->ToggleProbability(p_pattern_->GetCurrentTrack(), button_num - (TRELLIS_BUTTONS_PER_ROW * 2));
		}
		else {
			p_pattern_->ToggleBurstMultiplier(p_pattern_->GetCurrentTrack(), button_num - (TRELLIS_BUTTONS_PER_ROW * 3));
		}
		*/
	} else if (current_grid_mode_ == kGridModeSelectTrack) { //Track Select
		//Select a track if one exists and then go back to default edit mode
		if (button_num < NUM_OF_TRACKS) {
			p_pattern_->SetCurrentTrack(button_num);
		}
		else {
			switch (button_num) {
				case 16: default_grid_mode_ = kGridModeAccentEdit;		break; //Accent
				case 17: default_grid_mode_ = kGridModeProbabilityEdit;	break; //Probability 
				case 18: default_grid_mode_ = kGridModeRetriggerEdit;	break; //Retrigger
				case 19: default_grid_mode_ = kGridModeNoteEdit;		break; //Note 
				case 20: default_grid_mode_ = kGridModeModEdit;			break; //Modulo
				case 21: default_grid_mode_ = kGridModeJumpEdit;		break; //Jump 
				case 22: default_grid_mode_ = kGridModeSkipEdit;		break; //Skip
			}
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
		encoder_button_.CheckForPress();

		//Switch in and out of param edit mode
		if (encoder_button_.JustPressed()) { param_edit_ = !param_edit_; }

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
	}

	//Check for encoder change
	long current_encoder_position_ = encoder_.read();
	if (current_encoder_position_ != 0) {
		encoder_count = (encoder_count + 1) % 4;

		// Encode produces 4 events every time it turns a click.  Only process every forth event
		if (encoder_count != 3) { return;  }

		if (param_edit_ == true) {
			if (current_param_ == kParamMenuItemBpm) {
				p_clock_->OffsetTempo(current_encoder_position_);
			}
			else if (current_param_ == kParamMenuItemTrack) {
				if (current_encoder_position_ > 0) {
					p_pattern_->IncrementCurrentTrack();
				}
				else {
					p_pattern_->DecrementCurrentTrack();
				}
			}
		}
		else { //We are in param select, not param edit
			if (current_param_ == kParamMenuItemTrack) {
				current_param_ = kParamMenuItemBpm;
			}
			else {
				current_param_ = kParamMenuItemTrack;
			}
		}
		encoder_.write(0);
	}
}

void Grid::UpdateSelectButtonDisplay() {
	LedMode current_led_mode = (current_grid_mode_ == kGridModeSelectTrack) ? kLedModeFlash : kLedModeOff;
	track_select_led_.SetMode(current_led_mode);
	track_select_led_.UpdateDisplay(current_pulse_);
}