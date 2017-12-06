#include "Clock.h"

Clock::Clock(float bpm) { 
	SetTempo(bpm);
}

Clock::~Clock(){ }

void Clock::SetTempo(float bpm) {
	bpm_ = bpm;
	bpm_changed_ = true;
	Serial.print("Clock tempo is:");
	Serial.println(bpm_);
	Serial.print("Clock period is:");
	Serial.println(GetPeriod());
}

void Clock::OffsetTempo(float bpm_offset) {
	// Divide by 4 here because each click of the encoder sends 4 messages
	bpm_ += (bpm_offset / 4);
	bpm_changed_ = true;

	// Clip bpm within our min and max
	if (bpm_ > kMaxBpm) {
		bpm_ = kMaxBpm;
	}
	else if (bpm_ < kMinBpm) {
		bpm_ = kMinBpm;
	}
}


void Clock::UpdateTargetPulse(){
	//Clock is getting it's pulse value incremented by the timer interrupt
	//Store a copy here so it does not get changed by an interupt
	target_pulse_ = pulse_;

	//Check for lag and print it to the serial port every quarter note if it exists
	ULONG lag = target_pulse_ - current_pulse_;
	if ((lag > 1) && (target_pulse_ % PULSE_PER_QUARTER_NOTE == 0)){
		Serial.print("Lag: ");
		Serial.println(lag);
	}
}

void Clock::UpdateCurrentPulse() {
	//If we are less then the target increment the current pulse and it will get processed
	//on the next iteration
	if (current_pulse_ < target_pulse_) {
		current_pulse_++;
	}
}
