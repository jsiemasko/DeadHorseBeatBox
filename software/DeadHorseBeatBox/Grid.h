#ifndef _GRID_h
#define _GRID_h

#include "DHBB_Options.h"
#include "Button.h"
#include <Encoder.h>
#include "Led.h"
#include "Display.h"
#include "arduino.h"
#include <Wire.h>
#include "Adafruit_Trellis.h"
#include "Pattern.h"
#include "Track.h"
#include "Clock.h"

typedef unsigned short int USHORT;
typedef unsigned int ULONG;
typedef unsigned int UINT;

enum GridMode {
	//Select Modes
	kGridModeSelectFunction,
	kGridModeSelectTrack,
	kGridModeSelectTrackFunction,

	//Track Edit Modes
	kGridModeSingleTrackEdit,
	kGridModeFourTrackEdit
	
	/*
	RETHINKING ALL THESE MODES
	kGridModeBinaryNoteEdit,
	kGridModeStepRetrigger,
	kGridModeStepGateLength,
	kGridModeStepMidiNote,
	kGridModeStepStepJump,
	kGridModeTrackMute,
	kGridModeTrackDirection,
	kGridModeTrackClearToggle,
	kGridModeTrackClearSkip,
	kGridModeTrackClearVelocity,
	kGridModeTrackClearTrigger,
	kGridModeTrackClearGateLength,
	kGridModeTrackClearMidiNote,
	kGridModeSyncAllChannels
	*/
};

class Grid
{
 private:
	 Display * p_display_ = 0; 			//Display pointer, for changing OLED modes to match grid mode
	 Pattern * p_pattern_ = 0;			//Pointer to the current pattern object
	 Clock * p_clock_ = 0;				//Pointer to the current clock object
	 ULONG current_pulse_ = 0;			//Current pulse that is being processed
	 bool trellis_led_buffer_[32];		//Holds the current state of the LEDs before writing
	 long throttle_previous_ms_ = 0;	//Throttle counter for trellis button reads
	 USHORT button_renumber_[32] = {	//Used to fix the button numbering for our Trellis arrangement
		  0,  1,  2,  3,  8,  9, 10, 11,
		 16, 17, 18, 19, 24, 25, 26, 27,
		  4,  5,  6,  7, 12, 13, 14, 15,
		 20, 21, 22, 23, 28, 29, 30, 31
	 };
	 USHORT led_renumber_[32] = {		//Used to fix the LED numbering for our Trellis arrangement
		 0,  1,  2,  3, 16, 17, 18, 19,
		 4,  5,  6,  7, 20, 21, 22, 23,
		 8,  9, 10, 11, 24, 25, 26, 27,
		12, 13, 14, 15, 28, 29, 30, 31
	 };
	 //Trellis Setup
	 Adafruit_Trellis trellis_matrix0_ = Adafruit_Trellis();
	 Adafruit_Trellis trellis_matrix1_ = Adafruit_Trellis();
	 Adafruit_TrellisSet trellis_ = Adafruit_TrellisSet(&trellis_matrix0_, &trellis_matrix1_);
	 
	 //Button Setup
	 Button track_select_button_ = Button(TRACK_SELECT_BTN_PIN);
	 Led track_select_led_ = Led(TRACK_SELECT_LED_PIN);
	 Button function_select_button_ = Button(FUNCTION_SELECT_BTN_PIN);
	 Led function_select_led_ = Led(FUNCTION_SELECT_LED_PIN);
	 
	 //Encoder Setup
	 Button encoder_button_ = Button(ENCODER_BTN_PIN);
	 Encoder encoder_ =  Encoder(ENCODER_PIN_1, ENCODER_PIN_2);
	 long encoder_change_amount_ = 0;

	 //Current grid mode and grid mode to return to after an action
	 GridMode default_grid_mode_ = kGridModeFourTrackEdit;
	 GridMode current_grid_mode_ = kGridModeFourTrackEdit;

	 //Function to get a LED numebr based on row and column 
	 inline USHORT GetGridNumber(USHORT row, USHORT col) { return col + (row * TRELLIS_BUTTONS_PER_ROW); }
	 	 
	 void DisplayPlayingTracks();
	 void DisplaySingleTrackEditMode();
	 void DisplayFourTrackEditMode();
	 void DisplayFunctionSelectMenu();
	 void ClearGrid();
	 void WriteCurrentPattern();
	 void UpdateSelectButtonDisplay();
	 bool LedLightPattern(USHORT LightParam);

 public:
	Grid();
	~Grid();
	inline void SetDisplay(Display * p_display) { p_display_ = p_display; }
	inline void SetPattern(Pattern * p_pattern) { p_pattern_ = p_pattern; }
	inline void SetClock(Clock * p_clock) { p_clock_ = p_clock; }
	inline void SetGridMode(GridMode grid_mode) { current_grid_mode_ = grid_mode; }
	inline GridMode GetGridMode() { return current_grid_mode_; }
	void ProcessGridButton(USHORT button_num);
	void ReadSwitches();
	void UpdateDisplay(ULONG pulse);
};


#endif

