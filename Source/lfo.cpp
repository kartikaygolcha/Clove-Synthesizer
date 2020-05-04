/*
  ==============================================================================

	lfo.cpp
	Created: 22 Apr 2020 2:37:03pm
	Author:  Kartikay

  ==============================================================================
*/

#include "lfo.h"
lfo::lfo(CloveAudioProcessor& p) :
	processor(p)
{
	
	setSize(200, 200);

	//Setting items and attaching box to pointer
	oscType.addItem("Sin", 1);
	oscType.addItem("Saw", 2);
	oscType.addItem("Triangle", 3);
	oscType.addItem("Square", 4);
	oscType.setJustificationType(Justification::centred);
	addAndMakeVisible(&oscType);
	oscTypeVal = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.oscParam, "oscTypeLFO", oscType);



	
	// Initiliazing the Sliders
	MI.setRange(0.0f, 1.0f);
	MI.setValue(0.0);
	addAndMakeVisible(&MI);
	MIVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam, "MILFO", MI);


	Freq.setRange(0.01f, 30.0f);
	Freq.setValue(1.0);
	addAndMakeVisible(&Freq);
	freqVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam, "freqValLFO", Freq);
	





}

void lfo::paint(Graphics& g)
{
	//background ui stuff
	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	g.drawText("LFO", titleArea, Justification::centredTop);
	g.drawText("MI", 40, 60, 20, 20, Justification::centred);
	g.drawText("LFO Freq", 90, 60, 80, 20, Justification::centred);
	juce::Rectangle <float> area(25, 25, 150, 150);

	g.setColour(Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);

}

void  lfo::resized()
{
	// Setting bounds
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	oscType.setBounds(area.removeFromTop(20));
	MI.setBounds(40, 80, 20, 80);

	Freq.setBounds(80, 80, 100,80);
}