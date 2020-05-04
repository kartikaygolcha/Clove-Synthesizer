/*
  ==============================================================================

    Flanger.h
    Class file for a Flanger/Chorus and a Delay Line 
  ==============================================================================
*/
//Inclde Oscillators Class 
#include "Oscillators.h"

/**
Class Flanger for adding flanging
*/
class Flanger
{
public:
	//Set initial flanger parameters
	Flanger() : osc1()
	{
		osc1.setSampleRate(sampleRate);
		osc1.setFrequency(freq1);
		osc2.setSampleRate(sampleRate);
		osc2.setFrequency(freq2);
		clear();
	};

	/// Clearing the Buffer
	void clear()
	{
		buffer1 = new float[bufferSize];
		for (int i = 0; i < bufferSize; i++)
			buffer1[i] = 0;
	}
	
	/// Process a sample and add flanger to it
	float process(float input)
	{
		float delay2 = 20 * (1 + (osc2.process()));
		float delay  = 10 * (1 + (osc1.process()));
		float fb  = interpolate(delay);
		float fb2 = interpolate(delay2);
		float out = input + (gain * fb) + (gain2 * fb2);
		return (out/3.0f);

	}
	
	/// Adding sample to a parallel buffer
	void addSampleToBuffer(float data) {
		buffer1[writePos] = data;
		writePos++;
		writePos %= bufferSize;
	}
	
	/// Read Sample from Delay Line @params delay-> Delay from writePos in Samples
	float getSample(int delay) {

		if (writePos - delay < 0)
			return buffer1[bufferSize + writePos - delay];
		else
			return buffer1[writePos - delay];		
	}
	
	/// Interpolate Samples @params-> idx
	float interpolate(float idx)
	{
		float b = getSample((int)idx);
		float a = getSample((int)idx + 1);
		return (((1-idx + (int)idx)) * (b - a) + a);
	}
	
	/// Set Gain for two oscillators @params g-> Gain 1 or g1-> Gain2
	void setGain(float g, float g1)
	{
		gain = g;
		gain2 = g1;

	}
	
	/// Set Sample Rate
	void setSampleRate(float sr)
    {
        sampleRate=sr;
        
    }
    
	/// Set BufferSize
    	void setBufferSize(int buff_size)
    {
		delete buffer1;
		bufferSize=buff_size ;
		clear();        
    }

	/// Set Gain for two oscillators
		void setFreuency(float fr1, float fr2)
		{
			freq1 = fr1;
			freq2 = fr2;
			osc1.setSampleRate(sampleRate);
			osc1.setFrequency(freq1);
			osc2.setSampleRate(sampleRate);
			osc2.setFrequency(freq2);
		
		}
		
private:
	SinOsc osc1, osc2;                  // Sin Oscillator Instances
	float sampleRate=44100.0f;					// Sample Rate
	float freq1=0.5f,freq2=2.0f;	    // Frequency for two Oscillators
	float gain = 0.6f, gain2 = 0.7f;	//Gains for each osc
	int writePos = 0;					//index for write pointer
	int bufferSize = 44100;				//bufferSize in samples
	float* buffer1;						//Uninitialized Buffer pointer
};

/// Delay Line Class 
class DelayLine
{
public:
	/// store a value in the delay and retrieve the sample at the current read position
	float process(float inSamp)
	{
		float outVal = readVal();
		writeVal(inSamp + feedback * outVal);     // note the feedback here, scaling the output back in to the delay
		return outVal;
	}

	/// read a value from the buffer at the read head position, then increment and wrap the readPos
	float readVal()
	{
		float outVal = buffer[readPos];
		readPos++;
		readPos %= size;
		return outVal;
	}

	/// write a value to the buffer at the write head position, then increment and wrap the writePos
	void writeVal(float inSamp)
	{
		buffer[writePos] = inSamp;
		writePos++;
		writePos %= size;
	}

	/// set the actual delay time in samples
	void setDelayTimeInSamples(int delTime)
	{
		delayTimeInSamples = delTime;
		readPos = writePos - delayTimeInSamples;
		while (readPos < 0)
			readPos += size;
	}

	/// initialise the float array to a given maximum size (your specified delay time must always be less than this)
	void setSize(int newSize)
	{
		size = newSize;
		buffer = new float[size];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = 0.0f;
		}
	}

	void setFeedback(float fb)
	{
		feedback = fb;
		// check we're not going to get crazy positive feedback:
		if (feedback > 1)
			feedback = 1.0f;
		if (feedback < 0)
			feedback = 0.0f;
	}


private:
	float* buffer;              // the actual buffer - not yet initialised to a size
	int size;                   // buffer size in samples

	int writePos = 0;            // write head location - should always be 0 ≤ writePos < size
	int readPos = 0.0f;        // read head location - should always be 0 ≤ readPos < size

	int delayTimeInSamples;

	float feedback = 0.0f;       // how much of the delay line to mix with the input?

};
#pragma once
