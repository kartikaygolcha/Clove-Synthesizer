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
class oscGui2 :public Component
{
public:

	oscGui2(CloveAudioProcessor&);
	//Graphics
	void paint(Graphics&) override;
	void resized() override;


private:



	Slider detune{ Slider::LinearHorizontal ,Slider::TextBoxBelow };
	Slider MI{ Slider::LinearHorizontal ,Slider::TextBoxBelow };
	Slider octave{ Slider::LinearHorizontal,Slider::TextBoxBelow };
	Slider volume{ Slider::LinearBarVertical ,Slider::TextBoxLeft };
	ComboBox oscType;
	ComboBox modType;

	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> oscTypeVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> detuneVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> MIVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> octaveVal;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeVal;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	CloveAudioProcessor& processor;

};
