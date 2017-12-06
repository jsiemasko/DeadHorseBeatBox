#include <Encoder.h>
#include "Led.h"
#include "DHBB_Options.h"
#include "MidiManager.h"
#include "TempoLed.h"
#include "Grid.h"
#include "Pattern.h"
#include "Display.h"
#include "Clock.h"
#include <TimerThree.h>
#include <Arduino.h>

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

MidiManager midi_manager;
Clock clock(DEFAULT_TEMPO);
Display display;
TempoLed tempo_led;
Pattern pattern(&midi_manager);
Grid grid;

void setup(void) {
	//Serial for debugging
	Serial.begin(9600);
	Serial.println("");
	Serial.println("Deadhorse Beatbox Start");
	
	//Setup the OLED
	display.GraphicsSetup();
	display.SplashHorse(5000);
	
	//Display needs referances to pattern and clock
	display.SetPattern(&pattern);
	display.SetClock(&clock);

	//Grid needs referances to pattern, display and clock
	grid.SetPattern(&pattern);
	grid.SetDisplay(&display);
	grid.SetClock(&clock);

	//Set up the time timer interupt
	Timer3.initialize(clock.GetPeriod());
	Timer3.attachInterrupt(clockPulse);
	clock.SetBpmUnchanged();
}

void clockPulse(void) {	clock.IncrementPulse(); }

void loop(void) {
	clock.UpdateTargetPulse();

	if (clock.IsChangedBpm()) {
		Timer3.setPeriod(clock.GetPeriod());
		clock.SetBpmUnchanged();
	}

	//Check grid for button presses, grid class handles internal throttling so we 
	//don't check too frequently
	grid.ReadSwitches();

	if (clock.IsNewPulseReadyToProcess()) {
		clock.MarkCurrentPulseProcessed();

		long current_pulse = clock.GetCurrentPulse();

		pattern.ProcessPulse(current_pulse);
		midi_manager.ProcessPulse(current_pulse);

		//Update all displays unless we are lagging
		if (clock.GetLag() < 10) {
			if (current_pulse % PULSE_PER_STEP == 0) { display.UpdateDisplay(current_pulse); }

			tempo_led.UpdateTempoLed(current_pulse);
		}
		if (clock.GetLag() < 10) {
			if (current_pulse % 4 == 0) {
				grid.UpdateDisplay(current_pulse);
			}
		}
	}
	clock.UpdateCurrentPulse();
}