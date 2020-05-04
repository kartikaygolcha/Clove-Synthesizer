#pragma once
/*
  ==============================================================================

	waveTableSynth.h
	Created: 21 Apr 2020 5:41:52pm
	Author:  Kartikay

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
/*!
lfo Class for UI
*/
class lfo :public Component
{
public:

	lfo(CloveAudioProcessor&);
	//Graphics
	void paint(Graphics&) override;
	void resized() override;


private:

	// Sliders and Combo Box 
	Slider MI{ Slider::LinearBarVertical ,Slider::TextBoxBelow };
	Slider Freq{ Slider::Rotary,Slider::TextBoxBelow };
	ComboBox oscType;

	// Pointers for all slider and boxes
	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> oscTypeVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> MIVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> freqVal;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	CloveAudioProcessor& processor;

};
