#ifndef _CLOCK_h
#define _CLOCK_h

#include "DHBB_Options.h"
#include "arduino.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

class Clock {
 private:
	 float bpm_ = DEFAULT_TEMPO;
	 volatile ULONG pulse_ = 0; //Volitile pulse updated by the interupt
	 ULONG target_pulse_ = 0; //Non-volitile copy of pulse.
	 ULONG current_pulse_ = 0; //Pulse that is currently being processed
	 ULONG previous_pulse_ = 0; //Last pulse that was processed
	 static const UINT kPulsePerQuarterNote = STEPS_PER_QUARTER_NOTE * PULSE_PER_STEP;
	 static const UINT kMicrosecPerMin = 60000000;

 public:
	 //Constructors
	 Clock(float bpm);
	 ~Clock();

	 //Tempo
	 void SetTempo(float bpm);
	 inline void OffsetTempo(float bpm_offset) { bpm_ += (bpm_offset / 4); }
	 inline float GetTempo() const { return bpm_; }
	 inline long GetPeriod() const { return kMicrosecPerMin / (kPulsePerQuarterNote * bpm_); }
	 
	 //Timing
	 inline void IncrementPulse() { pulse_++; }
	 void UpdateTargetPulse();
	 inline bool IsNewPulseReadyToProcess() { return (current_pulse_ != previous_pulse_); }
	 inline void MarkCurrentPulseProcessed() { previous_pulse_ = current_pulse_; }
	 void UpdateCurrentPulse();
	 inline long GetLag() { return target_pulse_ - current_pulse_; };
	 inline long GetCurrentPulse() { return current_pulse_; }
};

#endif
