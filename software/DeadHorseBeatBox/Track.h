// Track.h

#ifndef _TRACK_h
#define _TRACK_h

#include "DHBB_Options.h"
#include "arduino.h"
#include "MidiManager.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

// Track direction controls direction of track playback
enum TrackDirection {
	kTrackDirectionForward,
	kTrackDirectionBackwards,
	kTrackDirectionRandom,
	kTrackDirectionRandomWalk
};

// Step controls what will trigger on each step of the track
struct Step {
	bool Enabled = false;			// Whether or not the step will trigger a midi event.
	bool Skip = false;				// Should the track cursor skip this step
	bool Accent = false;			// Apply the accent velocity?
	USHORT AccentVelocity = 127;
	bool Chance = false;
	USHORT ChanceAmount = 50;
	bool Note = false;
	USHORT NoteScaleOffset = 0;		// 0 - 7. Offset for MIDI root note using the current scaled
	bool Retrigger = false;
	USHORT RetriggerAmount = 1;
	USHORT GateLength = 48;
};

class Track{
private:
	Step steps_[STEPS_PER_PATTERN];
	USHORT track_num_ = 0;
	MidiManager * p_midi_manager_ = 0;		// Pointer to MIDI manager to allow us to trigger MIDI notes
	USHORT cursor_position_ = 0;			// Keep track of current cursor location
	USHORT next_cursor_position_ = 0;		// If <> kNoNextPosition force to cursor this position on next advance, else use TrackDirection
	USHORT num_steps_ = STEPS_PER_PATTERN;	// Number of steps that are not set to skip, defaults to steps in pattern
	USHORT midi_channel_ = 0;				// MIDI channel is set per track
	USHORT midi_root_note_ = 60;			// MIDI root note, step offsets from this based off scale settings
	TrackDirection direction_ = kTrackDirectionForward;	// Controls how the cursor advances
	bool probability_trigger_ = true;		// Boolean results of current probability check. Storing so we check it only once per step, not on each burst.
	USHORT retrigger_pulses[6] = { PULSE_PER_STEP, PULSE_PER_STEP / 2, PULSE_PER_STEP / 3, PULSE_PER_STEP / 4, PULSE_PER_STEP / 6, PULSE_PER_STEP / 12 }; // Burst mod values to make the math work
	void Advance();							// Advance per track direction setting

public:
	Track(USHORT track_num, USHORT midi_root_note);
	~Track();

	static const USHORT kNoNextPosition = 255;	// Represents that no next_cursor_position is set and Track_Direction shoudl be used to determine cursor movement

	inline void SetMidiManager(MidiManager * p_midi_manager) { p_midi_manager_ = p_midi_manager;  }

	// ENABLE STATE
	inline bool GetEnableState(USHORT step) { return steps_[step].Enabled; }
	inline void ToggleEnableState(USHORT step) { steps_[step].Enabled = !(steps_[step].Enabled); }

	// SKIP STATE
	inline bool GetSkipState(USHORT step) { return steps_[step].Skip; }
	inline void ToggleSkipState(USHORT step) {
		// If we are trying to disable the last step to skip then bail out.
		if (steps_[step].Skip == false && num_steps_ == 1) { return; }

		// Toggle the skip state and update the number of steps.
		steps_[step].Skip = !(steps_[step].Skip);
		num_steps_ += (steps_[step].Skip ? -1 : 1);
	}
	
	// MIDI
	inline void SetMidiRootNote(USHORT midi_root_note) { midi_root_note_ = midi_root_note; }

	// ACCENT
	inline bool GetAccent(USHORT step) { return steps_[step].Accent; };
	inline void ToggleAccent(USHORT step) {	steps_[step].Accent = !(steps_[step].Accent); };
	inline USHORT GetAccentVelocity(USHORT step) { return steps_[step].AccentVelocity; }
	inline void SetAccentVelocity(USHORT step, USHORT accent_velocity) { steps_[step].AccentVelocity = accent_velocity; }

	// CHANCE
	inline bool GetChance(USHORT step) { return steps_[step].Chance; }
	inline void ToggleChance(USHORT step) { steps_[step].Chance = !(steps_[step].Chance); }
	inline USHORT GetChanceAmount(USHORT step) { return steps_[step].ChanceAmount; }
	inline void SetChanceAmount(USHORT step, USHORT chance_amount) { steps_[step].ChanceAmount = chance_amount; }

	// RETRIGGER
	inline bool GetRetrigger(USHORT step) { return steps_[step].Retrigger; }
	inline void ToggleRetrigger(USHORT step) { steps_[step].Retrigger = !(steps_[step].Retrigger); }
	inline USHORT GetRetriggerAmount(USHORT step) { return steps_[step].RetriggerAmount; }
	inline void SetRetriggerAmount(USHORT step, USHORT retrigger_amount) { steps_[step].RetriggerAmount = retrigger_amount; }

	// NOTE
	inline bool GetNote(USHORT step) { return steps_[step].Note; }
	inline void ToggleNote(USHORT step) { steps_[step].Note = !(steps_[step].Note); }
	inline USHORT GetNoteScaleOffset(USHORT step) { return steps_[step].NoteScaleOffset; }
	inline void SetNoteScaleOffset(USHORT step, USHORT note_scale_offset) { steps_[step].NoteScaleOffset = note_scale_offset; }

	// TIMING
	inline void StepJump(USHORT cursor_position) { next_cursor_position_ = cursor_position; }
	inline void SetTrackDirection(TrackDirection direction) { direction_ = direction; }
	inline USHORT GetCursorPosition() { return cursor_position_; };
	void ProcessPulse(ULONG pulse);
};

#endif