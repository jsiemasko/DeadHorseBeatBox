#include "Step.h"
#include <Encoder.h>
#include "Led.h"
#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "MidiManager.h"
#include "TempoLed.h"
#include "Grid.h"
#include "Pattern.h"
#include "Display.h"
#include "Clock.h"
#include <TimerThree.h>
#include <Arduino.h>

MidiManager midi_manager;
Clock clock(DEFAULT_TEMPO);
Display display;
TempoLed tempo_led(TEMPO_LED_PIN);
Pattern pattern(&midi_manager);
Grid grid;

void setup(void) {
	//Serial for debugging
	Serial.begin(9600);
	Serial.println("");
	Serial.println("Deadhorse Beatbox Start");
	
	//Setup the OLED
	display.GraphicsSetup();
	display.SplashHorse();
	
	//Grid needs referances to pattern and clock
	grid.SetPattern(&pattern);
	grid.SetClock(&clock);

	//Display needs referances to grid, pattern and clock
	display.SetGrid(&grid);
	display.SetPattern(&pattern);
	display.SetClock(&clock);

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
		if (clock.GetLag() < 2 || current_pulse % PULSE_PER_STEP == 0) {
			display.UpdateDisplay(current_pulse);
		}
		tempo_led.UpdateDisplay(current_pulse);
		if ((clock.GetLag() < 2 && current_pulse % 2 == 0) || (current_pulse % PULSE_PER_STEP == 0)) {
			grid.UpdateDisplay(current_pulse);
		}
	}
	clock.UpdateCurrentPulse();
}