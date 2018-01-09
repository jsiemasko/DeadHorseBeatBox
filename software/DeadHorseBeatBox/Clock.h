#ifndef _CLOCK_h
#define _CLOCK_h

#include "DHBB_Options.h"
#include "DHBB_Types.h"
#include "arduino.h"

class Clock {
 private:
	 float bpm_ = DEFAULT_TEMPO;
	 bool bpm_changed_ = 0;			//Flag to mark bpm as changed so that Timer can update
	 volatile ULONG pulse_ = 0;		//Volitile pulse updated by the interupt
	 ULONG target_pulse_ = 0;		//Non-volitile copy of pulse.
	 ULONG current_pulse_ = 0;		//Pulse that is currently being processed
	 ULONG previous_pulse_ = 0;		//Last pulse that was processed
	 static const UINT kMicrosecPerMin = 60000000;
	 static const UINT kMinBpm = 1;
	 static const UINT kMaxBpm = 500;

 public:
	 //Constructors
	 Clock(float bpm);
	 ~Clock();

	 //Tempo
	 void SetTempo(float bpm);
	 void OffsetTempo(float bpm_offset);
	 inline bool IsChangedBpm() { return bpm_changed_; }
	 inline void SetBpmUnchanged() { bpm_changed_ = false; }; //indicate that the timer is in sync with the current BPM
	 inline float GetTempo() const { return bpm_; }
	 inline long GetPeriod() const { return kMicrosecPerMin / (PULSE_PER_QUARTER_NOTE * bpm_); }
	 
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
