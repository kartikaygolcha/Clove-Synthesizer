/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
//Include all headers and gui objects 
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope.h"
#include "Filter.h"
#include "oscGui.h"
#include "oscGui2.h"
#include "lfo.h"
//==============================================================================
/*!
Main Editor
*/
class CloveAudioProcessorEditor : public AudioProcessorEditor, public Timer
{
public:
	CloveAudioProcessorEditor(CloveAudioProcessor&);
	~CloveAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void styleMenuChanged()
	{
		//set the chord style 
		processor.setchord(styleMenu.getSelectedId());
		textLabel.setFont(textFont);
	}
private:
	// To get focus onf the keyboard component 
	void timerCallback() override
	{
		keyboardComponent.grabKeyboardFocus();
		stopTimer();
	}
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	CloveAudioProcessor& processor;
	MidiKeyboardComponent keyboardComponent;
	
	//GUI Elements
	Label textLabel{ {}, "Select Chord Type." };
	Font textFont{ 12.0f };
	ComboBox styleMenu;
	Envelope envGui;
	Filter filterGui;
	oscGui oscgui;
	oscGui2 oscgui2;
	lfo lFO;
	ToggleButton Chord_style{ "Chord Mode" };
	ToggleButton Reverb_style{ "Reverb" };
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> chordButton;
	ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> ReverbButton;
	String chord_names[6] = { "Major","Minor","Sus2","sus4","Maj7th","Dom7th" };
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CloveAudioProcessorEditor)
};
