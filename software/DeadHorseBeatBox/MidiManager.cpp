// 
// 
// 

#include "MidiManager.h"

MidiManager::MidiManager(){}

MidiManager::~MidiManager() {}

void MidiManager::AddEvent(MidiEvent& r_new_midi_event){
	
	//Get a referance to the event for requested track		
	USHORT track_num = r_new_midi_event.Track;
	MidiEvent& r_midi_event = midi_events_[track_num];
	
	//If our note is currently playing turn it off unless it is being held
	//It will be turned on again during pulse processing
	if (r_midi_event.Playing == true && r_midi_event.Hold == false) { 
		NoteOff(r_midi_event); 
	}

	//Copy the parameters of the incoming note
	r_midi_event.Track		= r_new_midi_event.Track;
	r_midi_event.RootNote	= r_new_midi_event.RootNote;
	r_midi_event.Channel	= r_new_midi_event.Channel;
	r_midi_event.Velocity	= r_new_midi_event.Velocity;
	r_midi_event.PulseLife	= r_new_midi_event.PulseLife;
	r_midi_event.Hold		= r_new_midi_event.Hold;
}

void MidiManager::NoteOn(MidiEvent& r_midi_event) {
	//Turn off the note if it is already playing
	if (r_midi_event.Playing == true) { NoteOff(r_midi_event); }
	usbMIDI.sendNoteOn(
		r_midi_event.RootNote, 
		r_midi_event.Velocity, 
		r_midi_event.Channel
	);
	r_midi_event.Playing = true;
}

void MidiManager::NoteOff(MidiEvent& r_midi_event) {
	usbMIDI.sendNoteOff(
		r_midi_event.RootNote,
		r_midi_event.Velocity,
		r_midi_event.Channel
	);
	r_midi_event.Playing = false;
}

void MidiManager::ProcessPulse(ULONG pulse){
	//Iterate through all events
	for (USHORT track_num = 0; (track_num < NUM_OF_TRACKS); track_num++) {
		MidiEvent& r_midi_event = midi_events_[track_num];

		//If our note has pulses left
		if (r_midi_event.PulseLife > 0){

			//Tick off a pulse if it is playing, otherwise set it playing
			if (r_midi_event.Playing == true) {
				r_midi_event.PulseLife--;
			} else {
				NoteOn(r_midi_event);
			}
		}
		else {//Our note has no pulses left

			//Turn it off if it is playing
			if (r_midi_event.Playing == true) { NoteOff(r_midi_event); }
		}
		/*
		if (pulse % PULSE_PER_STEP == 0) {
			Serial.print(r_midi_event.Playing);
			Serial.print(" ");
		}
		*/
	}
	/*
	if (pulse % PULSE_PER_STEP == 0) {
		Serial.println("");
	}
	*/
}

bool MidiManager::EventPlaying(USHORT track)
{
	return midi_events_[track].Playing;
}
