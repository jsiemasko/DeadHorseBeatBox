#include "Grid.h"

Grid::Grid(DHMidi::MidiManager * p_midi_manager, Song::Pattern * p_pattern, Clock * p_clock){
	p_midi_manager_ = p_midi_manager;
	p_pattern_ = p_pattern;
	p_clock_ = p_clock;
}

Grid::~Grid(){ }


void Grid::DisplaySingleTrackEditMode() {
	Song::Track& r_current_track = p_pattern_->GetCurrentTrack();
	for (USHORT current_step = 0; current_step < STEPS_PER_PATTERN; current_step++) {
		Song::Step& r_current_step = r_current_track.GetStep(current_step);

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
		trellis_.SetBuffer(0, current_step, r_current_step.GetEnableState());
		trellis_.SetBuffer(1, current_step, edit_param_value);
	}
	//Display Cursor
	int cursor_pos = r_current_track.GetCursorPosition();
	bool cursor_display_state = ((millis() / 50L) % 2 == 0);
	trellis_.SetBuffer(0, cursor_pos, cursor_display_state);
	trellis_.SetBuffer(1, cursor_pos, cursor_display_state);
}

void Grid::DisplayPlayingTracks() {
	// This shows the midi activity of every track while in track select mode.  
	for (int current_led = 0; current_led < TRELLIS_BUTTONS_PER_ROW; current_led++) {
		//Update the LED to show if a note is playing for the first two rows, clear the second row
		trellis_.SetBuffer(0, current_led, p_midi_manager_->GetEvent(current_led).Playing);
		trellis_.SetBuffer(1, current_led, false);
	}

	//Show currently selected mode LED
	switch (default_grid_mode_) {
		case kGridModeAccentEdit: trellis_.SetBuffer(1, 0, true); break;
		case kGridModeChanceEdit: trellis_.SetBuffer(1, 1, true); break;
		case kGridModeRetriggerEdit: trellis_.SetBuffer(1, 2, true); break;
		case kGridModeNoteEdit: trellis_.SetBuffer(1, 3, true); break;
		case kGridModeJumpEdit: trellis_.SetBuffer(1, 4, true); break;
		case kGridModeSkipEdit: trellis_.SetBuffer(1, 5, true); break;
		default: break;
	}	
}

void Grid::UpdateDisplay(ULONG pulse) {
	current_pulse_ = pulse;
	UpdateSelectButtonDisplay();

	//Update grid
	switch (current_grid_mode_) {
		case kGridModeSelectTrack:		DisplayPlayingTracks();	break;
		default:						DisplaySingleTrackEditMode(); break;
	}

	trellis_.UpdateDisplay();
	tempo_led_.UpdateDisplay(pulse);
}

void Grid::ProcessGridButton(USHORT button_num){
	Song::Track& r_current_track = p_pattern_->GetCurrentTrack();
	if (current_grid_mode_ == kGridModeSelectTrack) { //Track Select
		if (button_num < NUM_OF_TRACKS) {
			p_pattern_->SetCurrentTrack(button_num);
		} else {
			switch (button_num) {
			case 16: default_grid_mode_ = kGridModeAccentEdit;		break; //Accent
			case 17: default_grid_mode_ = kGridModeChanceEdit;		break; //Probability 
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
			button_num -= TRELLIS_BUTTONS_PER_ROW;
			Song::Step& r_current_step = r_current_track.GetStep(button_num);

			switch (current_grid_mode_){
				case kGridModeAccentEdit: r_current_step.ToggleAccentState(); break;
				case kGridModeChanceEdit: r_current_step.ToggleChanceState(); break;
				case kGridModeSkipEdit: r_current_step.ToggleSkipState(); break;
				case kGridModeRetriggerEdit: r_current_step.ToggleRetriggerState(); break;
				case kGridModeNoteEdit: r_current_step.ToggleNoteState(); break;
				case kGridModeJumpEdit: r_current_track.SetCursorPosition(button_num); break; //TODO: this.
				default: break; //Unknown mode, do nothing
			}
		}
	} 
}

void Grid::CheckForModeSwitch() {
	//Check if we need to switch modes
	if (track_select_button_.IsPressed()) {
		current_grid_mode_ = kGridModeSelectTrack;
	} else if (function_select_button_.JustReleased()) {
		p_clock_->TogglePlayState();
	} else {//Return to default mode
		current_grid_mode_ = default_grid_mode_;
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
	encoder_.Update();
	trellis_.ReadSwitches();

	CheckForModeSwitch();

//	bool is_any_key_pressed = false;
	for (USHORT button = 0; button < TRELLIS_NUM_OF_BUTTONS; button++) {
		if (trellis_.JustReleased(button)) {
			ProcessGridButton(button);
		}
	}
		/*
		//Only process the button if the encoder has not changed
		if (trellis_.JustReleased(i) && !cancel_press_[i]) { 
			ProcessGridButton(i); 
		} else if (trellis_.IsKeyPressed(i) && encoder_.HasValue()) {
			//If the encoder has moved then negate the key press
			cancel_press_[i] = true;
		} else if (!trellis_.IsKeyPressed(i) && cancel_press_[i]) {
			//If key is not pressed then clear the encoder changed flag
			cancel_press_[i] = false;
		}

		//If any of the keys have been pressed mark is_any_key_pressed
		if (trellis_.IsKeyPressed(i)) { is_any_key_pressed = true; }
	}
	*/
	
	/*
	//Only move the clock with the encoder if no buttons were being held
	if (!is_any_key_pressed && encoder_.HasValue()){
		p_clock_->OffsetTempo(encoder_.GetValue());
	}/* else if (encoder_.HasValue()) {
		Track current_track = p_pattern_->GetCurrentTrack();
		for (USHORT button_num = TRELLIS_BUTTONS_PER_ROW; button_num < TRELLIS_NUM_OF_BUTTONS; button_num++) {
			button_num = button_renumber_[button_num];
		
			if (trellis_.isKeyPressed(button_num)) {
				if (current_grid_mode_ == kGridModeAccentEdit) {
					Serial.println(button_num - TRELLIS_BUTTONS_PER_ROW);
					//current_track.GetStep(button_renumber_[button_num - TRELLIS_BUTTONS_PER_ROW]).ModifyAccentVelocity(encoder_.GetValue());
				}

			}
		
		}
		*/
			
}

void Grid::UpdateSelectButtonDisplay() {
	Controls::LedMode current_led_mode = (current_grid_mode_ == kGridModeSelectTrack) ? Controls::kLedModeFlash : Controls::kLedModeOff;
	track_select_led_.SetMode(current_led_mode);
	track_select_led_.UpdateDisplay(current_pulse_);
}