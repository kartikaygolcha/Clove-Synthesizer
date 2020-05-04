/*
  ==============================================================================
	PluginProcessor.h
    Created: 21 Apr 2020 5:41:52pm
	Author:  Kartikay
	Description: Header File for the plugin class.


  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MySynthesiser.h"

//==============================================================================
/**
*/

class CloveAudioProcessor : public AudioProcessor
{
public:

	//==============================================================================
	CloveAudioProcessor();
	~CloveAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	
	// Updating the Filter Coefficients
	void updateFilter();

	//Setting the chord variables accordin to given chord type
	void setchord(int chordnum);
	
	//================================================================================
	void processMidiInfo(MidiBuffer& buffer);

	/*Maintainin a KeyboardState for the Keyboard Component at Bottom*/
	MidiKeyboardState keyboardState;

	// State for various parameters. Though it should not be public, 
	// but done so for easier coding the Gui Parameters, instead it was better to use 
	// a function to return their pointers
	AudioProcessorValueTreeState tree, oscParam, oscParam2;
	
	//Bool to check if chord style is required or not
	bool chord_style = false;
	bool Reverb_State = false;
	
private:

	// Creating a lock for the critical section of adding midi
	CriticalSection lock;

	// Setting various parameters for chords
	const static int chordCount = 6;
	String chord_names[chordCount] = { "Major","Minor","Sus2","sus4","Maj7th","Dom7th" };
	int chord_1_arr[chordCount] = { 4,3,2,5,4,4 };
	int chord_2_arr[chordCount] = { 7,7,7,7,7,7 };
	int chord_3_arr[chordCount] = { 0,0,0,0,11,10 };
	int chord1 = 4, chord2 = 7, chord3 = 0, num_chord = 2;
	Array<int> addedMidi,incomeMidi;
	
	// Number of voices for Synth and defining the synth variable
	int voiceCount = 16;
	Synthesiser synth;

	// sampleRate
	float sampleRate;

	// Structures for sharing of pointers between processor and synth
	Envelope_note* env;
	OscillatorType* osc1, * osc2;
	LFO* l1;

	float* masterVol;

	// Effects
	Reverb rvb;
	IIRFilter filterLeft, filterRight;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CloveAudioProcessor)
};
