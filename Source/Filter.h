/*
  ==============================================================================

    Filter.h
    Created: 21 Apr 2020 5:41:52pm
    Author:  Kartikay
	Description: Filter Interface

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*! 
Filter Classs for UI
*/
class Filter    : public Component
{
public:
    Filter(CloveAudioProcessor&);
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;

private:
	// Slider for Filter CutOff and Res
    Slider filterCutoff;
    Slider filterRes;
    
	//Box for Selecting the type of Filter
    ComboBox filterMenu;
    
	//Pointers to Parameters for each of the values 
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterVal;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resVal;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	CloveAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
