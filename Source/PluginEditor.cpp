/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CloveAudioProcessorEditor::CloveAudioProcessorEditor(CloveAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p), keyboardComponent(processor.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
	, envGui(p), filterGui(p), oscgui(p), oscgui2(p),lFO(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(900, 600);
	textLabel.setFont(textFont);

	//Click the chord mode or Reverb Mode on or off
	Chord_style.setToggleState(processor.chord_style,false);
	Reverb_style.setToggleState(processor.Reverb_State, false);
	Chord_style.onClick = [this] { processor.chord_style = !processor.chord_style;  };
	Reverb_style.onClick = [this] { processor.Reverb_State = !processor.Reverb_State;  };

	for (int i = 0; i < 6; i++) {
		// add items to the combo-box
		styleMenu.addItem(chord_names[i], i + 1);
	}

	styleMenu.onChange = [this] { styleMenuChanged(); };
	styleMenu.setSelectedId(1);

	//Add and make Items Visible 
	addAndMakeVisible(&envGui);
	addAndMakeVisible(&filterGui);
	addAndMakeVisible(&oscgui);
	addAndMakeVisible(&oscgui2);
	addAndMakeVisible(&lFO);
	addAndMakeVisible(keyboardComponent);
	addAndMakeVisible(textLabel);
	addAndMakeVisible(styleMenu);
	addAndMakeVisible(Chord_style);
	addAndMakeVisible(Reverb_style);

	//Attach Buttons to Parameter Value tree
	chordButton = new AudioProcessorValueTreeState::ButtonAttachment(processor.tree, "Chord", Chord_style);
	ReverbButton = new AudioProcessorValueTreeState::ButtonAttachment(processor.tree, "Reverb", Reverb_style);
}

CloveAudioProcessorEditor::~CloveAudioProcessorEditor()
{
}

//==============================================================================
void CloveAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	g.drawText("CLOVE", titleArea, Justification::centredTop);

	juce::Rectangle <float> area(25, 25, getWidth() - 50, getHeight() - 50);

	g.setColour(Colours::red);
	g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void CloveAudioProcessorEditor::resized()
{

	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	const int componentWidth = 200;
	const int componentHeight = 200;

	// PLacing all components on the GUI
	oscgui.setBounds(area.removeFromLeft(2 * componentWidth).removeFromTop(componentHeight));
	oscgui2.setBounds(area.removeFromLeft(2 * componentWidth).removeFromTop(componentHeight));
	area = getLocalBounds().reduced(40);
	area.setX(40);
	area = area.withTrimmedTop(200);
	envGui.setBounds(area.removeFromLeft(componentHeight).removeFromTop(componentWidth));
	filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	lFO.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	styleMenu.setBounds(area.removeFromLeft(componentWidth / 2).removeFromTop(componentHeight / 2).withTrimmedTop(30).withTrimmedTop(30));
	Chord_style.setBounds(area.removeFromLeft(componentWidth / 2).removeFromTop(componentHeight / 2).withTrimmedTop(30).withTrimmedTop(30));
	area = getLocalBounds().reduced(40);
	area.setX(640);
	area = area.withTrimmedTop(260);
	Reverb_style.setBounds(area.removeFromLeft(componentWidth / 2).removeFromTop(componentHeight / 2).withTrimmedTop(30).withTrimmedTop(30));
    keyboardComponent.setBounds(10, getHeight() - 100, getWidth() - 20, 100);

}
