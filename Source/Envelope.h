/*
  ==============================================================================

    Envelope.h
    Created: 21 Apr 2020 5:41:52pm
	Author:  Kartikay
	Description: Envelope class for graphics and parameters controls 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*!
 Envelope Class for UI
*/
class Envelope    : public Component
{
public:
    Envelope(CloveAudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;
    

private:
	//Sliders for Attack,Decay,Sustain and Release
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    
	//Slider Attachments to varibles
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> decayVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseVal;
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	CloveAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
