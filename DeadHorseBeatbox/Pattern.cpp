// 
// 
// 

#include "Pattern.h"


Pattern::Pattern(MidiManager * p_midi_manager){
	p_midi_manager_ = p_midi_manager;
	for (USHORT trackNum = 0; trackNum < NUM_OF_TRACKS; trackNum++) {
		_tracks[trackNum].SetMidiManager(p_midi_manager);
	}
}

Pattern::~Pattern(){ }

void Pattern::ProcessPulse(ULONG pulse){
	for (USHORT trackNum = 0; trackNum < NUM_OF_TRACKS; trackNum++) {
		_tracks[trackNum].ProcessPulse(pulse);
	}
}