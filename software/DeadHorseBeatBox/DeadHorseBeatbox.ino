#include <MIDI.h>
#include <Encoder.h>
#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "Grid.h"
#include <TimerThree.h>
#include <Arduino.h>
#include "Display/DisplayController.h"
#include "Midi/MidiManager.h"
#include "Song/Clock.h"
#include "Song/Step.h"
#include "Song/Pattern.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

Midi::MidiManager midi_manager = Midi::MidiManager(&MIDI);
Song::Clock clock = Song::Clock(CLOCK_DEFAULT_TEMPO);
Song::Pattern pattern = Song::Pattern(&midi_manager);
Grid grid = Grid(&midi_manager, &pattern, &clock);
Display::DisplayController display = Display::DisplayController(&midi_manager, &grid, &pattern, &clock);

void setup(void) 
{
	//Serial for debugging
	Serial.begin(9600);
	Serial.println("");
	Serial.println("Deadhorse Beatbox Start");
		
	//Set up the timer interupt
	Timer3.initialize(clock.GetPeriod());
	Timer3.attachInterrupt(clockPulse);
	clock.SetBpmUnchanged();
	clock.TogglePlayState();
}

void clockPulse(void) 
{	
	//Timer3 interrupt calls this function, if we are playing we increment the target pulse
	if (clock.GetPlayState()) 
	{
		clock.IncrementPulse();
	}
}

void loop(void) 
{
	clock.UpdateTargetPulse();
	
	//Check for BPM change
	if (clock.IsChangedBpm())
	{
		Timer3.setPeriod(clock.GetPeriod());
		clock.SetBpmUnchanged();
	}

	grid.ReadSwitches(); //Check grid for button presses, grid class handles internal throttling so we don't check too frequently
	ProcessPulse();
	UpdateDisplays();
	
	//If we are not playing then shut down all notes
	if (!clock.GetPlayState()) 
	{ 
		midi_manager.AllNotesOff(); 
	}
	clock.UpdateCurrentPulse();
}

void ProcessPulse() 
{
	long current_pulse = clock.GetCurrentPulse();
	if (clock.IsNewPulseReadyToProcess()) 
	{
		clock.MarkCurrentPulseProcessed();
		pattern.ProcessPulse(current_pulse);
		midi_manager.ProcessPulse(current_pulse);
	}
}

void UpdateDisplays() 
{
	long current_pulse = clock.GetCurrentPulse();
	bool is_playing = clock.GetPlayState();
	//Update all displays unless we are lagging, ensure displays still update even when the clock is stopped
	if (clock.GetLag() < 2 || current_pulse % PULSE_PER_STEP == 0 || !is_playing) 
	{
		display.UpdateDisplay(current_pulse);
		if ((current_pulse % 2 == 0) || !is_playing) 
		{
			grid.UpdateDisplay(current_pulse);
		}
	}
}
