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
		tracks_[0].GetStep(0).ToggleEnableState();
		tracks_[0].GetStep(0).ToggleAccentState();
		tracks_[0].GetStep(3).ToggleEnableState();
		tracks_[0].GetStep(4).ToggleEnableState();
		tracks_[0].GetStep(4).ToggleAccentState();
		tracks_[0].GetStep(6).ToggleEnableState();
		tracks_[0].GetStep(8).ToggleEnableState();
		tracks_[0].GetStep(8).ToggleAccentState();
		tracks_[0].GetStep(11).ToggleEnableState();
		tracks_[0].GetStep(12).ToggleEnableState();
		tracks_[0].GetStep(12).ToggleAccentState();
		tracks_[0].GetStep(14).ToggleEnableState();

		tracks_[1].GetStep(4).ToggleEnableState();
		tracks_[1].GetStep(4).ToggleRetriggerState();
		tracks_[1].GetStep(4).ToggleAccentState();
		tracks_[1].GetStep(12).ToggleEnableState();
		tracks_[1].GetStep(12).ToggleAccentState();
		tracks_[1].GetStep(13).ToggleEnableState();
		tracks_[1].GetStep(13).ToggleRetriggerState();
		tracks_[1].GetStep(13).ToggleChanceState();
		tracks_[1].GetStep(14).ToggleEnableState();
		tracks_[1].GetStep(14).ToggleChanceState();
		tracks_[1].GetStep(15).ToggleEnableState();
		tracks_[1].GetStep(15).ToggleChanceState();

		tracks_[2].GetStep(0).ToggleEnableState();
		tracks_[2].GetStep(0).ToggleAccentState();
		tracks_[2].GetStep(1).ToggleEnableState();
		tracks_[2].GetStep(3).ToggleEnableState();
		tracks_[2].GetStep(4).ToggleEnableState();
		tracks_[2].GetStep(4).ToggleAccentState();
		tracks_[2].GetStep(8).ToggleEnableState();
		tracks_[2].GetStep(8).ToggleAccentState();
		tracks_[2].GetStep(9).ToggleEnableState();
		tracks_[2].GetStep(12).ToggleEnableState();
		tracks_[2].GetStep(12).ToggleAccentState();
		tracks_[2].GetStep(14).ToggleEnableState();

		tracks_[3].GetStep(2).ToggleEnableState();
		tracks_[3].GetStep(6).ToggleEnableState();
		tracks_[3].GetStep(10).ToggleEnableState();
		tracks_[3].GetStep(13).ToggleEnableState();
		tracks_[3].GetStep(15).ToggleEnableState();
		tracks_[3].GetStep(15).ToggleSkipState();
	}
}

Pattern::~Pattern(){ }

void Pattern::ProcessPulse(ULONG pulse){
	for (USHORT trackNum = 0; trackNum < NUM_OF_TRACKS; trackNum++) {
		tracks_[trackNum].ProcessPulse(pulse);
	}
}