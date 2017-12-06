// Track.h

#ifndef _TRACK_h
#define _TRACK_h

#include "DHBB_Options.h"
#include "arduino.h"

#include "MidiManager.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

enum Direction {
	kDirectionForward,
	kDirectionBackwards,
	kDirectionRandom,
	kDirectionRandomWalk
};

struct Step {
	bool Enabled = false;
	bool Trigger = true;
	USHORT MidiRootNote = 60;
	USHORT Accent = 3;
	USHORT GateLength = 48;
	USHORT Probability = 4; 
	USHORT BurstMultiplier = 1;
};

class Track{
private:
	USHORT num_steps_to_trigger = STEPS_PER_PATTERN;
	MidiManager * p_midi_manager_ = 0;
	USHORT midi_channel = 0;
	Direction direction_ = kDirectionForward;
	USHORT cursor_position_ = 0;
	USHORT next_cursor_position_ = 0;
	Step steps_[STEPS_PER_PATTERN];
	void Advance();
	USHORT track_num_ = 0;

public:
	Track(USHORT track_num, USHORT midi_root_note);
	~Track();

	inline void SetMidiManager(MidiManager * p_midi_manager) { p_midi_manager_ = p_midi_manager;  }

	static const USHORT kNoNextPosition = 255;

	//Trigger State
	inline bool GetTriggerState(USHORT step) { return steps_[step].Trigger; }
	inline void ToggleTriggerState(USHORT step) {
		steps_[step].Trigger = !(steps_[step].Trigger);
		num_steps_to_trigger += (steps_[step].Trigger ? 1 : -1);
	}
	
	//Enable State
	inline bool GetEnableState(USHORT step) { return steps_[step].Enabled; }
	inline void ToggleEnableState(USHORT step) { steps_[step].Enabled = !(steps_[step].Enabled); }

	//MIDI
	void SetMidiRootNote(USHORT midi_root_note);
	inline void SetMidiRootNote(USHORT step, USHORT midi_root_note) { steps_[step].MidiRootNote = midi_root_note; };
	
	inline USHORT GetAccent(USHORT step) { return steps_[step].Accent; };
	inline void ToggleAccent(USHORT step) {	steps_[step].Accent = ((steps_[step].Accent) % 4) + 1; };
	
	//Probability
	inline USHORT GetProbability(USHORT step) { return steps_[step].Probability; }
	inline void ToggleProbability(USHORT step) { steps_[step].Probability = ((steps_[step].Probability) % 4) + 1; } //Probability ranges from 1-4.

	//Burst Multiplier
	void SetBurstMultiplier(USHORT burst_multiplier);
	inline void SetBurstMultiplier(USHORT step, USHORT burst_multiplier) { steps_[step].BurstMultiplier = burst_multiplier; }

	//Timing
	inline void StepJump(USHORT cursor_position) { next_cursor_position_ = cursor_position; }
	inline void SetDirection(Direction direction) { direction_ = direction; }
	inline USHORT GetCursorPosition() { return cursor_position_; };
	void ProcessPulse(ULONG pulse);
};

#endif

//Clock Division