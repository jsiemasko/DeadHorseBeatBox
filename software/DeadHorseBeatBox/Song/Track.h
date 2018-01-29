#ifndef _TRACK_h
#define _TRACK_h

#include "arduino.h"
#include "Step.h"
#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "../Midi/MidiManager.h"

namespace Song {
	// Track direction controls direction of track playback
	enum TrackDirection {
		kTrackDirectionForward,
		kTrackDirectionBackwards,
		kTrackDirectionRandom,
		kTrackDirectionRandomWalk
	};

	class Track {
	private:
		Step steps_[STEPS_PER_PATTERN];
		USHORT track_num_ = 0;
		Midi::MidiManager * p_midi_manager_ = 0;		// Pointer to MIDI manager to allow us to trigger MIDI notes
		USHORT cursor_position_ = 0;			// Keep track of current cursor location
		USHORT next_cursor_position_ = 0;		// If <> kNoNextPosition force to cursor this position on next advance, else use TrackDirection
		USHORT num_steps_ = STEPS_PER_PATTERN;	// Number of steps that are not set to skip, defaults to steps in pattern
		USHORT midi_channel_ = 0;				// MIDI channel is set per track
		USHORT midi_root_note_ = 60;			// MIDI root note, step offsets from this based off scale settings
		TrackDirection direction_ = kTrackDirectionForward;	// Controls how the cursor advances
		bool probability_trigger_ = true;		// Boolean results of current probability check. Storing so we check it only once per step, not on each burst.
		void Advance();							// Advance per track direction setting

	public:
		Track(USHORT track_num, USHORT midi_root_note);
		~Track();
		static const USHORT kNoNextPosition = 255;	// Represents that no next_cursor_position is set and Track_Direction shoudl be used to determine cursor movement
		inline Step& GetStep(USHORT step_num) { return steps_[step_num]; }
		inline void SetMidiManager(Midi::MidiManager * p_midi_manager) { p_midi_manager_ = p_midi_manager; }
		inline void SetMidiRootNote(USHORT midi_root_note) { midi_root_note_ = midi_root_note; }
		inline void SetTrackDirection(TrackDirection direction) { direction_ = direction; }
		inline USHORT GetCursorPosition() { return cursor_position_; };
		inline void SetCursorPosition(USHORT cursor_position) { next_cursor_position_ = cursor_position; };
		inline void SetMidiChannel(USHORT midi_channel) { midi_channel_ = midi_channel; }
		void CalculateNumberOfSteps();
		void ProcessPulse(ULONG pulse);
	};
}
#endif