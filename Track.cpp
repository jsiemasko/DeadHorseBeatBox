#include "Track.h"

Track::Track(USHORT track_num, USHORT midi_root_note) { 
	track_num_ = track_num; 
	SetMidiRootNote(midi_root_note); 
}

Track::~Track(){ }

//MIDI
void Track::SetMidiRootNote(USHORT midiRootNote) {
	for (int current_step = 0; current_step < STEPS_PER_PATTERN; current_step++) {
		steps_[current_step].MidiRootNote  = midiRootNote;
	}
}

//Burst Multiplier
void Track::SetBurstMultiplier(USHORT burstMultiplier) {
	for (int current_step = 0; current_step < STEPS_PER_PATTERN; current_step++) {
		steps_[current_step].BurstMultiplier = burstMultiplier;
	}
}

//Timing
void Track::Advance(){
	//If we have no active steps don't advance
	if (num_steps_to_trigger <= 0) { return; }

	//If we have saved a next position move to it then clear the next position placeholder.
	if (next_cursor_position_ != kNoNextPosition){
		cursor_position_ = next_cursor_position_;
		next_cursor_position_ = kNoNextPosition;
		return;
	}
	
	//Normal advance
	do{
		switch (direction_) {
			case kDirectionForward:		
				cursor_position_ = (cursor_position_ + 1) % STEPS_PER_PATTERN; 
				break;
			case kDirectionBackwards:
				cursor_position_ = (cursor_position_ == 0) ? STEPS_PER_PATTERN - 1 : (cursor_position_ - 1) % STEPS_PER_PATTERN;
				break;
			case kDirectionRandom:
				cursor_position_ = rand() % STEPS_PER_PATTERN;
				break;
			case kDirectionRandomWalk: 
				short int walk_direction = (rand() % 3) - 1; // -1, 0, 1
				cursor_position_ = ((cursor_position_ == 0) && (walk_direction == -1)) ? STEPS_PER_PATTERN - 1 : (cursor_position_ + walk_direction) % STEPS_PER_PATTERN;
				break;
		}
	} while (steps_[cursor_position_].Trigger == false);
}

void Track::ProcessPulse(ULONG pulse){
	//If we are at the start of a new step then advance before updating
	bool start_of_new_step = pulse % PULSE_PER_STEP == 0;
	if (start_of_new_step) {
		Advance(); 
	}
	Step currentStep = steps_[cursor_position_];
	
	bool probability_trigger = ((rand() % 4) /* 0-3 */ <= currentStep.Probability);
	if (start_of_new_step) {
		if (currentStep.Enabled && currentStep.Trigger && probability_trigger) {
			MidiEvent midi_event;
			midi_event.RootNote = currentStep.MidiRootNote;
			midi_event.Velocity = (currentStep.Accent * 31) + 3; //Convert accent 1-4 to Velocity 34-127
			midi_event.Channel = midi_channel;
			midi_event.PulseLife = 5;
			midi_event.Track = track_num_;
			p_midi_manager_->AddEvent(midi_event);
		}
	}
}
