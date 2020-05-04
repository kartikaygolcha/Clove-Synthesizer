/*
  ==============================================================================

	oscGui2.cpp
	Created: 22 Apr 2020 2:37:03pm
	Author:  Kartikay

  ==============================================================================
*/

#include "oscGui2.h"
oscGui2::oscGui2(CloveAudioProcessor& p) :
	processor(p)
{
	///////
	setSize(200, 200);

	//Initializing all menu types and Frequency sliders
	oscType.addItem("Sin", 1);
	oscType.addItem("Saw", 2);
	oscType.addItem("Triangle", 3);
	oscType.addItem("Square", 4);
	oscType.setJustificationType(Justification::centred);
	addAndMakeVisible(&oscType);
	oscTypeVal = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.oscParam2, "oscType2", oscType);


	detune.setRange(-600.0f, 600.0f);
	detune.setValue(0.0);
	addAndMakeVisible(&detune);
	detuneVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam2, "detune2", detune);

	MI.setRange(0.0f, 1.0f);
	MI.setValue(0.0);
	addAndMakeVisible(&MI);
	MIVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam2, "MI2", MI);

	volume.setRange(0.0f, 1.0f);
	volume.setValue(0.8f);
	addAndMakeVisible(&volume);
	volumeVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam2, "volume2", volume);

	octave.setRange(-3.0f, 3.0f, 1.0f);
	octave.setValue(0.0f);
	octave.setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection);
	addAndMakeVisible(&octave);
	octaveVal = new AudioProcessorValueTreeState::SliderAttachment(processor.oscParam2, "octave2", octave);

}

void oscGui2::paint(Graphics& g)
{
	//background ui stuff
	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	g.drawText("Osc 2", titleArea, Justification::centredTop);
	g.drawText("Volume", 300, 150, 60, 20, Justification::centredTop);
	g.drawText("Octave", 125, 95, 60, 20, Justification::centred);
	g.drawText("Detune", 240, 70, 60, 20, Justification::centredTop);
	g.drawText("MI", 135, 130, 20, 20, Justification::centred);
	juce::Rectangle <float> area(25, 25, 350, 150);

	g.setColour(Colours::yellow);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void  oscGui2::resized()
{
	//Setting bounds
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	oscType.setBounds(area.removeFromTop(20).removeFromLeft(150));
	detune.setBounds(220, 40, 100, 30);
	volume.setBounds(330, 40, 20, 100);
	octave.setBounds(40, 80, 100, 30);
	MI.setBounds(40, 120, 100, 30);

}