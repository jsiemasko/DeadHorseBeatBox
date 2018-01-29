#include "Track.h"

namespace Song {
	Track::Track(USHORT track_num, USHORT midi_root_note) {
		track_num_ = track_num;
		midi_root_note_ = midi_root_note;
	}

	Track::~Track() { }

	// TIMING
	void Track::Advance() {
		//If we have saved a next position move to it then clear the next position placeholder.
		if (next_cursor_position_ != kNoNextPosition) {
			cursor_position_ = next_cursor_position_;
			next_cursor_position_ = kNoNextPosition;
			return;
		}

		//Normal advance if we land on a step that is to be skipped try again.
		do {
			CalculateNumberOfSteps();
			//If we have no active steps don't advance
			if (num_steps_ <= 0) { return; }

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
		} while ((steps_[cursor_position_].GetSkipState() == true));
		//Loop if we land on a skipped step, if there are not active steps we will bail out in the next iteration
	}

	void Track::CalculateNumberOfSteps() {
		// Zero out and recalculate the number of steps that are not skipped
		num_steps_ = 0;
		for (USHORT i = 0; i < STEPS_PER_PATTERN; i++) {
			if (!steps_[i].GetSkipState()) { num_steps_++; }
		}

	}

	void Track::ProcessPulse(ULONG pulse) {
		// Wrap pulse number to get the current pulse we are on in the current step
		USHORT pulse_in_step = pulse % PULSE_PER_STEP;

		// If we are at the start of a new step then advance before updating
		if (pulse_in_step == 0) { Advance(); }

		// Pull the current step
		Step current_step = steps_[cursor_position_];

		// We only do a probability check once per step so that the burst is triggered as an atomic unit
		if (pulse_in_step == 0) {
			probability_trigger_ = current_step.GetChanceState() ? (rand() % 100 >= current_step.GetChanceAmount()) : true;
		}

		// Do we need to check for a note trigger? Always check on first pulse and use retrigger math for the rest.
		bool note_check_needed = (pulse_in_step == 0) || ((pulse_in_step % current_step.GetRetriggerPulses() == 0) && current_step.GetRetriggerState());

		// Check all the values to see if we need a new note
		if (note_check_needed && current_step.GetEnableState() && !current_step.GetSkipState() && probability_trigger_) {
			Midi::MidiEvent midi_event;
			midi_event.RootNote = midi_root_note_ + (current_step.GetNoteState() ? current_step.GetNoteOffset() : 0);
			midi_event.Velocity = current_step.GetAccentState() ? current_step.GetAccentVelocity() : 100;
			midi_event.Channel = midi_channel_;
			midi_event.PulseLife = current_step.GetGateLength();
			midi_event.Track = track_num_;
			p_midi_manager_->AddEvent(midi_event);
		}
	}
}