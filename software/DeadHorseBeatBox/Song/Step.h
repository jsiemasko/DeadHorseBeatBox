#ifndef _SONG_STEP_h
#define _SONG_STEP_h

#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "arduino.h"

namespace Song {
	class Step {
	private:
		bool enable_state_ = false;
		bool skip_state_ = false;
		bool accent_state_ = false;
		long accent_velocity_ = 127;
		bool chance_state_ = false;
		USHORT chance_amount_ = 50;
		bool note_state_ = false;
		USHORT note_offset_ = 0;
		bool retrigger_state_ = false;
		USHORT retrigger_amount_ = 0;
		USHORT gate_length_ = 5;

		// Burst mod values to make the math work
		USHORT retrigger_pulses[5] = { PULSE_PER_STEP / 2, PULSE_PER_STEP / 3, PULSE_PER_STEP / 4, PULSE_PER_STEP / 6, PULSE_PER_STEP / 12 };

	public:
		Step();
		~Step();

		//Enable
		inline bool GetEnableState() { return enable_state_; }
		inline void ToggleEnableState() { enable_state_ = !enable_state_; }

		//Skip
		inline bool GetSkipState() { return skip_state_; }
		inline void ToggleSkipState() { skip_state_ = !skip_state_; }

		//Accent
		inline bool GetAccentState() { return accent_state_; }
		inline void ToggleAccentState() { accent_state_ = !accent_state_; }
		inline long GetAccentVelocity() { return accent_velocity_; }
		inline void ModifyAccentVelocity(long accent_velocity_offset) {
			Serial.println(accent_velocity_offset);
			//accent_velocity_ += accent_velocity_offset; 
			//accent_velocity_ = (accent_velocity_ >= 127) ? 127 : accent_velocity_;
			//accent_velocity_ = (accent_velocity_ <= 0) ? 0 : accent_velocity_;
			Serial.println(accent_velocity_);
		}

		//Chance
		inline bool GetChanceState() { return chance_state_; }
		inline void ToggleChanceState() { chance_state_ = !chance_state_; }
		inline USHORT GetChanceAmount() { return chance_amount_; }
		inline void SetChanceAmount(USHORT chance_amount) { chance_amount_ = chance_amount; }

		//Note
		inline bool GetNoteState() { return note_state_; }
		inline void ToggleNoteState() { note_state_ = !note_state_; }
		inline USHORT GetNoteOffset() { return note_offset_; }
		inline void SetNoteOffset(USHORT note_offset) { note_offset_ = note_offset; }

		//Retrigger
		inline bool GetRetriggerState() { return retrigger_state_; }
		inline void ToggleRetriggerState() { retrigger_state_ = !retrigger_state_; }
		inline USHORT GetRetriggerAmount() { return retrigger_amount_; }
		inline void SetRetriggerAmount(USHORT retrigger_amount) { retrigger_amount_ = retrigger_amount; }
		inline USHORT GetRetriggerPulses() { return retrigger_pulses[retrigger_amount_]; }

		//Gate Length
		inline void SetGateLength(USHORT gate_length) { gate_length_ = gate_length; }
		inline USHORT GetGateLength() { return gate_length_; }
	};
}
#endif