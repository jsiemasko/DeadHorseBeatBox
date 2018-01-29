// Pattern.h

#ifndef _PATTERN_h
#define _PATTERN_h

#include "../DHBB_Options.h"
#include "../DHBB_Types.h"
#include "arduino.h"
#include "Track.h"

namespace Song {
	class Pattern
	{
	public:
		Pattern(Midi::MidiManager * p_midi_manager);
		~Pattern();


		//inline bool GetTrackNotePlaying(USHORT track) { return p_midi_manager_->EventPlaying(track); }

		inline Track& GetTrack(USHORT track_num) { return tracks_[track_num]; }

		void ProcessPulse(ULONG pulse);

		inline USHORT GetCurrentTrackNumber() { return current_track_; }
		inline Track& GetCurrentTrack() { return tracks_[current_track_]; }
		inline USHORT GetSelectedStepNumber() { return selected_step_; }
		inline void SetCurrentTrack(USHORT current_track) { current_track_ = current_track; }
		inline void IncrementCurrentTrack() { current_track_ = (current_track_ + 1) % NUM_OF_TRACKS; }
		inline void DecrementCurrentTrack() {
			if (current_track_ == 0) {
				current_track_ = NUM_OF_TRACKS - 1;
			}
			else {
				current_track_ = current_track_ - 1;
			}
		}
	private:
		Track tracks_[NUM_OF_TRACKS] = {
			{ 0, 36 },	{ 1, 38 },	{ 2, 42 },	{ 3, 46 },
			{ 4, 37 },	{ 5, 39 },	{ 6, 40 },	{ 7, 49 },
			{ 8, 53 },	{ 9, 41 },	{ 10, 43 },	{ 11, 56 },
			{ 12, 54 }, { 13, 55 },	{ 14, 60 },	{ 15, 61 }
		};
		USHORT current_track_ = 0;
		USHORT selected_step_ = 0;
	};
}
#endif