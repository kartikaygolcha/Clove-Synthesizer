/*
  ==============================================================================

    waveTableSynth.cpp
    Created: 22 Apr 2020 2:37:03pm
    Author:  Kartikay
    Description: Code to generate a Wavetable for wave synthesis.
  ==============================================================================
*/

#include "waveTableSynth.h"


waveTableSynth::waveTableSynth() 
{
	// Initialize the SampleRate for each oscillator as the table size 
	// and set freq 1.0f to get one cycle of each wave 
	osc1.setSampleRate(tableSize);
	osc1.setFrequency(1.0f);
	osc2.setSampleRate(tableSize);
	osc2.setFrequency(1.0f);
	osc3.setSampleRate(tableSize);
	osc3.setFrequency(1.0f);
	osc4.setSampleRate(tableSize);
	osc4.setFrequency(1.0f);
	sub1.setSampleRate(tableSize);
	sub1.setFrequency(1.0f);
	sub2.setSampleRate(tableSize);
	sub2.setFrequency(1.0f);
	sub3.setSampleRate(tableSize);
	sub3.setFrequency(1.0f);
	for (int i = 0; i < oscCount; i++)
	{
		waveTableInit(i);
		currentIndex[i] = 0;
		tableDelta[i] = 0;
	}
}

/*!

Initialize WaveTable

@params: Input modVal-> Amount of Modulation


*/
void waveTableSynth::waveTableInit(int typeWave)
{
	// Fill Values in the table
	float val;
	waveTable[typeWave].clear();
	for (int j = 0; j < tableSize; j++)
	{
		if (typeWave == 0) val = osc1.process();
		else if (typeWave == 1) val = osc2.process();
		else if (typeWave == 2) val = osc3.process();
		else if (typeWave == 3) val = osc4.process();
		else if (typeWave == 4) val = sub1.process();
		else if (typeWave == 5) val = sub2.process();
		else if (typeWave == 6) val = sub3.process();
		waveTable[typeWave].add(val);
	}
}

/*!

This function is used to set the Color modification of the wave

@params: Input modVal-> Amount of Modulation
@params: Input typeWave-> index of wave
@params: Input freq-> frequency of current wave

*/

void waveTableSynth::waveTableMod(float modVal,int typeWave,float freq)
{
	//Add Modulation to wave by changing the parent wave
	switch (typeWave) {
	case 3: if (modVal > 0 && modVal <=1) osc4.setPulseWidth(modVal);
			else osc4.setPulseWidth(0.5f);
			osc4.setFrequency(1.0f); 
			waveTableInit(typeWave); 
			break;
	case 1: setFrequency(freq * (1 + modVal),5); break;
	case 0: setFrequency(freq * (1 + modVal),4); break;
	case 2: setFrequency(freq * (1 + modVal),6); break;
	}
}

// set Sample Rate->SR
void waveTableSynth::setSampleRate(float SR)
{
	// set Sample Rate
	sampleRate = SR; 

}

/*!
 
 Set Frequency of the wave

@params: Input Frequency -> frequency of the wave
@params : typeWave-> index of wave
*/
void waveTableSynth::setFrequency(float frequency, int typeWave)
{
	//set tableDelta as skip size along the table for the required frequency
	auto tableSizeOverSampleRate = tableSize / sampleRate;
	tableDelta[typeWave] = frequency * tableSizeOverSampleRate;
	
}

/*!

This function is used to get the next value of waveTable object 

@params: Input typeWave-> index of wave
@Params: Output -> current oscillator value
*/

float waveTableSynth::getSample(int typeWave)
{
	float val=0;
	
	switch (typeWave)
	{

	case 0: val = (getOscVal(0) + getOscVal(4)) / 2.0f; break;
	case 1: val = (getOscVal(1) + getOscVal(5)) / 2.0f; break;
	case 2: val = (getOscVal(2) + getOscVal(6)) / 2.0f; break;
	case 3: val = (getOscVal(3)); break;

	
	}
	return val;
}

/*!

This function is used to get the next value of the oscillator 

@params: Input typeWave-> index of wave
@Params: Output -> current oscillator value
*/
float waveTableSynth::getOscVal(int typeWave)
{
	//Checking the type of wave using its index and doing linear interpolation
	auto index0 = (unsigned int)currentIndex[typeWave];
	auto index1 = index0 + 1;
	auto frac = currentIndex[typeWave] - (float)index0;
	auto* table = waveTable[typeWave].begin();
	auto value0 = table[index0];
	auto value1 = table[index1];
	auto currentSample = value0 + frac * (value1 - value0);
	currentIndex[typeWave] += tableDelta[typeWave];
	if ((currentIndex[typeWave]) > tableSize - 1)
		currentIndex[typeWave] -= tableSize;
	jassert(currentIndex[typeWave] <= tableSize);

	return currentSample;
}


//Clear current Index for all waves
void waveTableSynth::clearCurrentIndex()
{ 
	
	for (int i = 0; i < oscCount; i++)
		currentIndex[i] = 0;
}