// 
// 
// 

#include "Pattern.h"


Pattern::Pattern(MidiManager * p_midi_manager){
	p_midi_manager_ = p_midi_manager;
	for (USHORT trackNum = 0; trackNum < NUM_OF_TRACKS; trackNum++) {
		tracks_[trackNum].SetMidiManager(p_midi_manager);
	}

	if (GENERATE_TEST_BEAT) {
		tracks_[0].ToggleEnableState(0);
		tracks_[0].ToggleAccent(0);
		tracks_[0].ToggleEnableState(3);
		tracks_[0].ToggleEnableState(4);
		tracks_[0].ToggleAccent(4);
		tracks_[0].ToggleEnableState(6);
		tracks_[0].ToggleEnableState(8);
		tracks_[0].ToggleAccent(8);
		tracks_[0].ToggleEnableState(11);
		tracks_[0].ToggleEnableState(12);
		tracks_[0].ToggleAccent(12);
		tracks_[0].ToggleEnableState(14);

		tracks_[1].ToggleEnableState(4);
		tracks_[1].ToggleAccent(4);
		tracks_[1].ToggleEnableState(12);
		tracks_[1].ToggleAccent(12);
		tracks_[1].ToggleProbability(12);
		tracks_[1].ToggleEnableState(13);
		tracks_[1].ToggleProbability(13);
		tracks_[1].ToggleEnableState(14);
		tracks_[1].ToggleProbability(14);
		tracks_[1].ToggleEnableState(15);
		tracks_[1].ToggleProbability(15);


		tracks_[2].ToggleEnableState(0);
		tracks_[2].ToggleAccent(0);
		tracks_[2].ToggleEnableState(1);
		tracks_[2].ToggleEnableState(3);
		tracks_[2].ToggleEnableState(4);
		tracks_[2].ToggleAccent(4);
		tracks_[2].ToggleEnableState(8);
		tracks_[2].ToggleAccent(8);
		tracks_[2].ToggleEnableState(9);
		tracks_[2].ToggleEnableState(12);
		tracks_[2].ToggleAccent(12);
		tracks_[2].ToggleEnableState(14);

		tracks_[3].ToggleEnableState(2);
		tracks_[3].ToggleEnableState(6);
		tracks_[3].ToggleEnableState(10);
		tracks_[3].ToggleEnableState(13);
		tracks_[3].ToggleEnableState(15);
	}
}

Pattern::~Pattern(){ }

void Pattern::ProcessPulse(ULONG pulse){
	for (USHORT trackNum = 0; trackNum < NUM_OF_TRACKS; trackNum++) {
		tracks_[trackNum].ProcessPulse(pulse);
	}
}