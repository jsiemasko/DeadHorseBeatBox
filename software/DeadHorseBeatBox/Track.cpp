#include "Track.h"

Track::Track(USHORT track_num, USHORT midi_root_note) { 
	track_num_ = track_num; 
	midi_root_note_ = midi_root_note;
}

Track::~Track(){ }

// TIMING
void Track::Advance(){
	//If we have no active steps don't advance
	if (num_steps_ <= 0) { return; }

	//If we have saved a next position move to it then clear the next position placeholder.
	if (next_cursor_position_ != kNoNextPosition){
		cursor_position_ = next_cursor_position_;
		next_cursor_position_ = kNoNextPosition;
		return;
	}
	
	//Normal advance if we land on a step that is to be skipped try again.
	do{
		CalculateNumberOfSteps();
		switch (direction_) {
			case kTrackDirectionForward:		
				cursor_position_ = (cursor_position_ + 1) % STEPS_PER_PATTERN; 
				break;
			case kTrackDirectionBackwards:
				cursor_position_ = (cursor_position_ == 0) ? STEPS_PER_PATTERN - 1 : (cursor_position_ - 1) % STEPS_PER_PATTERN;
				break;
			case kTrackDirectionRandom:
				cursor_position_ = rand() % STEPS_PER_PATTERN;
				break;
			case kTrackDirectionRandomWalk: 
				short int walk_direction = (rand() % 3) - 1; // -1, 0, 1
				cursor_position_ = ((cursor_position_ == 0) && (walk_direction == -1)) ? STEPS_PER_PATTERN - 1 : (cursor_position_ + walk_direction) % STEPS_PER_PATTERN;
				break;
		}
	} while ((steps_[cursor_position_].GetSkipState() == true) && (num_steps_ > 0));
}

void Track::CalculateNumberOfSteps() {
	// Zero out and recalculate the number of steps that are not skipped
	num_steps_ = 0;
	for (USHORT i = 0; i < STEPS_PER_PATTERN; i++) {
		if (!steps_[i].GetSkipState()) { num_steps_++; }
	}

}

void Track::ProcessPulse(ULONG pulse){
	// Wrap pulse number to get the current pulse we are on in the current step
	USHORT pulse_in_step = pulse % PULSE_PER_STEP;
	
	// If we are at the start of a new step then advance before updating
	if (pulse_in_step == 0) { 
		Advance(); 
	}

	// Pull the current step
	Step currentStep = steps_[cursor_position_];
	
	// We only do a probability check once per step so that the burst is triggered as an atomic unit
	if (pulse_in_step == 0) { 
		probability_trigger_ = currentStep.GetChanceState() ? (rand() % 2 == 0) : true; 
	}

	// Do we need to check for a note trigger? Always check on first pulse and use retrigger math for the rest.
	bool note_check_needed = (pulse_in_step == 0) || ((pulse_in_step % currentStep.GetRetriggerPulses() == 0) && currentStep.GetRetriggerState());

	// Check all the values to see if we need a new note
	if (note_check_needed && currentStep.GetEnableState() && !currentStep.GetSkipState() && probability_trigger_) {
		MidiEvent midi_event;
		midi_event.RootNote = midi_root_note_;
		midi_event.Velocity = currentStep.GetAccentState() ? 127 : 100;
		midi_event.Channel = midi_channel_;
		midi_event.PulseLife = 5;
		midi_event.Track = track_num_;
		p_midi_manager_->AddEvent(midi_event);
	}
}
