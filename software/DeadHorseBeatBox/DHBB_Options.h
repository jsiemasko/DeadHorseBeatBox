#ifndef DHBB_OPTIONS_H_
#define DHBB_OPTIONS_H_

//Precalculating some values here to save time

/* -- PATTERN & TRACK -- */
#define NUM_OF_TRACKS 16
#define STEPS_PER_PATTERN 16
#define STEPS_PER_QUARTER_NOTE 4

/* -- CLOCK -- */
#define DEFAULT_TEMPO 98
#define PULSE_PER_STEP 12
#define PULSE_PER_QUARTER_NOTE 48 // (PULSE_PER_STEP * STEPS_PER_QUARTER_NOTE)

/* -- DISPLAY -- */
#define DISPLAY_SPLASH_SCREEN_DISPLAY_TIME 2000
#define DISPLAY_REFRESH_FRAMES 1
#define DISPLAY_PIN_CS 37
#define DISPLAY_PIN_DC 38
#define DISPLAY_PIN_RESET 39

/* -- TRELLIS -- */
#define TRELLIS_BRIGHTNESS 1
#define TRELLIS_NUM_OF_GRIDS 2
#define TRELLIS_ADDRESS_1 0x71
#define TRELLIS_ADDRESS_2 0x70
#define TRELLIS_BUTTONS_PER_GRID 16
#define TRELLIS_NUM_OF_ROWS 2
#define TRELLIS_READ_THROTTLE_IN_MS 30	// MS to wait between reads of Trellis buttons.  Too fast and things get wonky, per Adafruit's example code
#define TRELLIS_NUM_OF_BUTTONS 32		// (TRELLIS_NUM_OF_GRIDS * TRELLIS_BUTTONS_PER_GRID)
#define TRELLIS_BUTTONS_PER_ROW 16		// (TRELLIS_NUM_OF_BUTTONS / TRELLIS_NUM_OF_ROWS)

/* -- Track Select Button -- */
#define TRACK_SELECT_BTN_PIN 9
#define TRACK_SELECT_LED_PIN 7

/* -- Function Select Button -- */
#define FUNCTION_SELECT_BTN_PIN 10

/* -- Encoder -- */
#define ENCODER_BTN_PIN 6
#define ENCODER_PIN_1 21
#define ENCODER_PIN_2 22


/* -- Tempo LED -- */
#define TEMPO_LED_PIN 8
#define TEMPO_LED_BLINK_FREQUENCY 4

#endif
