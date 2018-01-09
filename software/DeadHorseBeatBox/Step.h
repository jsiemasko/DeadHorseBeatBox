// Step.h

#ifndef _STEP_h
#define _STEP_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "arduino.h"

class Step {
private:
	bool enable_state_ = false;
	bool skip_state_ = false;
	bool accent_state_ = false;
	bool chance_state_ = false;
	bool note_state_ = false;
	bool retrigger_state_ = false;
public:
	Step();
	~Step();

	inline bool GetEnableState() { return enable_state_; }
	inline void ToggleEnableState() { enable_state_ = !enable_state_; }

	inline bool GetSkipState() { return skip_state_; }
	inline void ToggleSkipState() { skip_state_ = !skip_state_; }

	inline bool GetAccentState() { return accent_state_; }
	inline void ToggleAccentState() { accent_state_ = !accent_state_; }

	inline bool GetChanceState() { return chance_state_; }
	inline void ToggleChanceState() { chance_state_ = !chance_state_; }

	inline bool GetNoteState() { return note_state_; }
	inline void ToggleNoteState() { note_state_ = !note_state_; }

	inline bool GetRetriggerState() { return retrigger_state_; }
	inline void ToggleRetriggerState() { retrigger_state_ = !retrigger_state_; }

	USHORT AccentVelocity = 127;
	USHORT ChanceAmount = 50;
	USHORT NoteScaleOffset = 0;		// 0 - 7. Offset for MIDI root note using the current scaled
	USHORT RetriggerAmount = 1;
	USHORT GateLength = 48;
};

#endif