/*
  ==============================================================================

    waveTableSynth.h
    Created: 21 Apr 2020 5:41:52pm
    Author:  Kartikay
	Description: Code to generate a Wavetable for wave synthesis.

  ==============================================================================
*/
//Include all libraries
#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillators.h"

///Class for synthesizing a wavetable 
class waveTableSynth 
{
public:
	// Default Constructor for the class
	waveTableSynth();

	// Set Sample Rate- Input: (float) SR
	void setSampleRate(float SR);

	// Set Frequency- Input: (float) frequency, (int) index of wave
	void setFrequency(float frequency, int typeWave);
	
	// Intialize the waveTable
	void waveTableInit(int typeWave);
	
	/* Get sample for the required wave 
	Input: (int) index of wave
	Output: (float) Sample Value
	*/
	float getOscVal(int tyeWave);
	float getSample(int typeWave);

	// Clear index to read to zero 
	void clearCurrentIndex();
	
	// Modify the waveTable
	void waveTableMod(float modVal,int typeWave,float freq);
	
private:
	const static int oscCount = 7;
	Array<float> waveTable[oscCount];						//Making a waveTable
	const unsigned int tableSize = 1 << 16;		         	//Initializing a size for the table
	float sampleRate;								        // Sample Rate
	float tableDelta[oscCount];								// skip size in table
	int currentIndex[oscCount]; 							// current index of skip

	//Setting up intended Oscillators
	SinOsc osc1,sub1;
	SawOsc osc2,sub2;
	TriOsc osc3,sub3;
	SquareOsc osc4;


};