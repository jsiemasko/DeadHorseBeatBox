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

	void DisplayController::DisplayTrack(USHORT y, USHORT track_number)
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

	void DisplayController::DisplayTrackPlayingBargraph(USHORT y, USHORT graph_height)
	{
		view.DisplayBargraphBackground(y, graph_height);
		for (USHORT track = 0; track < NUM_OF_TRACKS; track++)
		{
			Midi::MidiEvent& r_event = p_midi_manager_->GetEvent(track);
			bool event_playing = r_event.Playing;
			if (event_playing)
			{
				track_playing_bargraph_[track] = (r_event.Velocity > 100) ? graph_height : graph_height * 0.7;
			}
			view.DisplayBar(16, 28, track, track_playing_bargraph_[track], event_playing);
		}
	}

	void DisplayController::ProcessTrackPlayingBargraphFall()
	{
		//Check if we need to process a fall of bargraph
		if (millis() - last_bargraph_update_ > fall_speed)
		{
			for (USHORT track = 0; track < NUM_OF_TRACKS; track++)
			{
				Midi::MidiEvent& r_event = p_midi_manager_->GetEvent(track);
				if (!r_event.Playing && track_playing_bargraph_[track] > 0) { track_playing_bargraph_[track] = track_playing_bargraph_[track] - 1; }
			}
			last_bargraph_update_ = millis();
		}
	}

	void DisplayController::DisplaySingleTrackView(USHORT track_number)
	{
		view.DisplayPageHeader(0, 0, p_grid_->GetCurrentEditParameter(), p_clock_->GetTempo());
		DisplayTrackPlayingBargraph(16, 28);
		view.DisplayTrackNumbers(44, track_number);
		DisplayTrack(52, track_number);
	}

	void DisplayController::DisplayFourTrackView(USHORT track_number)
	{
		USHORT first_display_track = (track_number / 4) * 4;
		view.DisplayPageHeader(0, 0, p_grid_->GetCurrentEditParameter(), p_clock_->GetTempo());
		DisplayTrack(14, first_display_track);
		DisplayTrack(24, first_display_track + 1);
		view.DisplayTrackNumbers(35, track_number);
		DisplayTrack(43, first_display_track + 2);
		DisplayTrack(53, first_display_track + 3);
	}

	void DisplayController::UpdateDisplay(ULONG pulse)
	{
		ProcessTrackPlayingBargraphFall();
		view.ClearBuffer();
		DisplaySingleTrackView(p_pattern_->GetCurrentTrackNumber());
		//DisplayFourTrackView(p_pattern_->GetCurrentTrackNumber());
		view.SendBuffer();
	}
}
