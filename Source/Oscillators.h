/*
 ==============================================================================
 
    Oscillators.h
	Created: 21 Apr 2020 5:41:52pm
	Author:  Kartikay
	Description: Oscillators
 ==============================================================================
 */

#pragma once

#include <cmath>  // for sin function
#define TWOPI 6.283185307179586476925286766559

/// Base class for oscillators (0-1 phasor)
class Phasor
{
public:
    Phasor()  {};
    
    ~Phasor() {};
    
    /// Note: 0-1
    double process()
    {
        phase += phaseDelta;
        
        if (phase > 1.0)
            phase -=1;
        
        return output(phase);
    }
    
    /// Override for the actual output based on the 0-1 phase
    virtual float output(float p)
    {
        return p;
    }
    
    /// Set the sample rate first
    void setSampleRate(float SR)
    {
        sampleRate = SR;
    }
    /// Make sure the sample rate is set first!
    void setFrequency(float freq)
    {
        frequency = freq;
        phaseDelta = frequency / sampleRate;
    }
	float getFrequency()
	{
		return frequency;
	}
    void setPhase(float p)
    {
        phase = p;
    }
    void offsetPhase(float o)
    {
        phase += o;
    }
    
private:
    float frequency;
    float sampleRate;
    double phase = 0.0;
    double phaseDelta;
};


// CHILD Class
class TriOsc : public Phasor
{
    float output(float p) override
    {
        return 3.5f*(fabsf(p - 0.5f) - 0.5f);
    }
};
// CHILD Class
class SawOsc : public Phasor
{
	float output(float p) override
	{
		return lowpass(1-(p-0.5f));
	}
	float lowpass(float inVal)
	{
		float outVal = previousVal + (1.0f - smoothing) * (inVal - previousVal);
		previousVal = outVal;
		return outVal;
	}

	void setSmoothing(float inSmooth) { smoothing = inSmooth; }
private:
	float pulseWidth = 0.5f;
	float previousVal = 0.0f;
	float smoothing = 0.5f;
};

// CHILD Class
class SinOsc : public Phasor
{
    float output(float p) override
    {
        return std::sin(p * TWOPI);
    }
};

// CHILD Class
class SquareOsc : public Phasor
{
public:
    float output(float p) override
    {
        float outVal = 0.5;
        if (p > pulseWidth)
            outVal = -0.5;
        return outVal;
    }
    void setPulseWidth(float pw)
    {
        pulseWidth = pw;
    }
private:
    float pulseWidth = 0.5f;
};





