// MidiManager.h

#ifndef _MIDIMANAGER_h
#define _MIDIMANAGER_h

#include "DHBB_Options.h"
#include "arduino.h"
#include <vector>

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

struct MidiEvent {
	USHORT Track = 0;
	USHORT RootNote = 60;
	USHORT Channel = 0;
	USHORT Velocity = 100;
	USHORT PulseLife = PULSE_PER_STEP / 2;
	bool Playing = false;
	bool Hold = false;
};

class MidiManager{
 private:
	 MidiEvent midi_events_[NUM_OF_TRACKS];
	 void NoteOn(MidiEvent& midi_event);
	 void NoteOff(MidiEvent& midi_event);

 public:
	 MidiManager();
	~MidiManager();
	void AddEvent(MidiEvent& midi_event);
	void ProcessPulse(ULONG pulse);
	bool EventPlaying(USHORT track);
};

#endif

