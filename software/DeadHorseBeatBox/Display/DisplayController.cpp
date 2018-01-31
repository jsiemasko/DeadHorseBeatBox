#include "DisplayController.h"

namespace Display 
{

	DisplayController::DisplayController(Midi::MidiManager * p_midi_manager, Grid * p_grid, Song::Pattern * p_pattern, Song::Clock * p_clock)
	{
		p_midi_manager_ = p_midi_manager;
		p_grid_ = p_grid;
		p_pattern_ = p_pattern;
		p_clock_ = p_clock;

		view.DisplaySplashScreen();
	}

	DisplayController::~DisplayController() { }

	void DisplayController::DisplayTrack(USHORT track_number, USHORT y)
	{
		view.DisplayStepBackground(y);
		Song::Track& r_current_track = p_pattern_->GetTrack(track_number);
		for (USHORT step_number = 0; step_number < STEPS_PER_PATTERN; step_number++) {
			Song::Step& r_step = r_current_track.GetStep(step_number);
			if (!r_step.GetSkipState())
			{
				view.DisplayStepBox(step_number, y, r_step.GetEnableState(), r_step.GetAccentState(), r_step.GetChanceState(), r_step.GetRetriggerState());
			}
		}
		view.DisplayStepCursor(r_current_track.GetCursorPosition(), y);
	}

	void DisplayController::DisplaySingleTrackView(USHORT track_number)
	{
		view.DisplayPageHeader(0, 0, p_grid_->GetCurrentEditParameter(), p_clock_->GetTempo());
		DisplayTrack(track_number, 48);
	}

	void DisplayController::DisplayFourTrackView(USHORT track_number)
	{
		USHORT first_display_track = (track_number / 4) * 4;
		view.DisplayPageHeader(0, 0, p_grid_->GetCurrentEditParameter(), p_clock_->GetTempo());
		DisplayTrack(first_display_track, 15);
		DisplayTrack(first_display_track + 1, 25);
		DisplayTrack(first_display_track + 2, 35);
		DisplayTrack(first_display_track + 3, 45);
	}

	void DisplayController::UpdateDisplay(ULONG pulse)
	{
		view.ClearBuffer();
		DisplaySingleTrackView(p_pattern_->GetCurrentTrackNumber());
		//DisplayFourTrackView(p_pattern_->GetCurrentTrackNumber());
		view.SendBuffer();
	}
}
