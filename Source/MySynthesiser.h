/*
  ==============================================================================

    MySynthesiser.h
     Created: 21 Apr 2020 5:41:52pm
    Author:  Kartikay
	Description: Code to generate a Wavetable for wave synthesis.

  ==============================================================================
*/

#pragma once
//Include WaveTable and Oscillators
#include "Oscillators.h"
#include "waveTableSynth.h"

// Defining structures for easy transfer of pointers
struct Envelope_note {
	float* attack, * decay, * release, * sustain;
};
struct OscillatorType {
	float* waveType, * detune, * MI, * volume, * octave, * modtype;
};
struct LFO {
	float * waveType,* MI, * freqVal;
};

// ===========================
// ===========================
// SOUND
class MySynthSound : public SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// Synthesiser Voice - My synth code goes in here

/*!
 @class MySynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MySynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 */
class MySynthVoice : public SynthesiserVoice
{
public:
    MySynthVoice() {}

	//--------------------------------------------------------------------------
/**
 Initialize each voice 

 @params: sample Rate, type: float 
 */
	void init(float sampleRate)
	{
		//Setting Sample Rate for all oscillators
		env.setSampleRate(sampleRate);
		wt1.setSampleRate(sampleRate);
		wt2.setSampleRate(sampleRate);
		lfo1.setSampleRate(sampleRate);

		//Setting inital reverb parameters
		Reverb::Parameters reverbParams;
		reverbParams.dryLevel = 0.8f;
		reverbParams.wetLevel = 0.3f;
		reverbParams.roomSize = 0.95f;
		rvb.reset();
		rvb.setParameters(reverbParams);
	
		//Setting inital Envelope parameters
		envParams.attack =0.0;
		envParams.decay = 0.5f;
		envParams.sustain = 0.0f;
		envParams.release = 0.0f;
		env.setParameters(envParams);
		
		//Setting initial freq to zero
		freq = 0;
	}
    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
		//setting flags for a note to be playing or ending
        playing = true;
		ending = false;

		//Finding the frequency of the note
		freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	

		//modulating the ADSR parameters	
		envParams.attack = *this->e2->attack;
		envParams.decay = *this->e2->decay;
		envParams.sustain = *this->e2->sustain;
		envParams.release = *this->e2->release;
		env.setParameters(envParams);
		env.reset();
		env.noteOn();

    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
		if (allowTailOff)
		{
			env.noteOff();
			ending = true;
		}
		else
		{
			 clearCurrentNote();
			 playing = false;
		}
	}
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block:
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {

		if (playing) // check to see if this voice should be playing
		{
			changePitch();
			float n;
			// iterate through the necessary number of samples (from startSample up to startSample + numSamples)
			for (int sampleIndex = startSample; sampleIndex < (startSample + numSamples); sampleIndex++)
			{
				// find envelope Value
				float envVal = env.getNextSample();
				// What mod type is being received from osc2/lfo to osc 1
				current_val = getOutputValue();
				float currentSample = (current_val * envVal);
				// for each channel, write the currentSample float to the output
				for (int chan = 0; chan < outputBuffer.getNumChannels(); chan++)
				{
					// The output sample is scaled by 0.2 so that it is not too loud by default
					outputBuffer.addSample(chan, sampleIndex, currentSample * *masterVol);
					
				}
				// Check if note is almost died out
				if (ending)
				{
					if (envVal < 0.0001f)
					{
						clearCurrentNote();
						playing = false;

					}

				}
			}

			
		}
    }

	/*!
	
	Change various attributes like Pitch using detune and octave and apply Wave Modulation	
	
	*/
	void changePitch()
	{
		//Change the frequency to a new frequency depending upon the detune in cents and change octave
		newfreq = exp(log(freq) + *detune * log(2) / 1200.0f);
		newfreq2 = exp(log(freq) + *detune2 * log(2) / 1200.0f);
		newfreq *= (pow(2, *octave));
		newfreq2 *= (pow(2, *octave2));

		//Set Frequency for both of the oscillators and LFO
		wt1.setFrequency(newfreq, *waveType);
		wt2.setFrequency(newfreq2, *waveType2);
		lfo1.setFrequency(*freqLFO, *LFOtype);

		//Add Color Modulation for WaveTable object 1
		wt1.waveTableMod(*MI, *waveType, newfreq);
	
	}

	/*!
	
	Compares the Modulation type and calculates the output value of both oscillators
	
	*/
	
	float getOutputValue()
	{
		switch ((int)* modtype)
		{

			// No Modulation
		case 0:  osc_val2 = wt2.getSample(*waveType2); osc_val1 = wt1.getSample(*waveType); break;

			// FM Modulation from OSc2	
		case 1: osc_val2 = wt2.getSample(*waveType2);
			wt1.setFrequency(newfreq * (1 + 0.5f* *MI2 * osc_val2), *waveType);
			osc_val1 = wt1.getSample(*waveType);
			wt1.setFrequency(newfreq, *waveType); break;

			// AM Modulation from Osc2 
		case 2:
			osc_val2 = wt2.getSample(*waveType2);
			osc_val1 = wt1.getSample(*waveType) * (osc_val2 + *MI2) / 2; break;


			//RM Modulation from Osc2 	
		case 3:  osc_val2 = wt2.getSample(*waveType2);
			osc_val1 = wt1.getSample(*waveType) * osc_val2 * *MI2; break;

			//AM Modulation from LFO
		case 4:  osc_val2 = wt2.getSample(*waveType2);
			osc_val1 = wt1.getSample(*waveType) * (lfo1.getSample(*LFOtype) + *miLFO) / 2; break;

			//FM Modulation from LFO
		case 5:
			osc_val2 = wt2.getSample(*waveType2);
			wt1.setFrequency(newfreq * (1 + 0.5f* *miLFO * lfo1.getSample(*LFOtype)), *waveType);
			osc_val1 = wt1.getSample(*waveType);
			wt1.setFrequency(newfreq, *waveType); break;
		}

		//return the final value
		float value = (osc_val1 * *volume + osc_val2 * *volume2) / 2;
		return value;
	
	}
    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of MySynthSound
     */
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<MySynthSound*> (sound) != nullptr;
    }
	void setParameters(Envelope_note* e1,OscillatorType* osc1, OscillatorType* osc2,LFO *l2,float *gain)
	{
		masterVol = gain;
		
		//Getting pointers for envelope, Oscillators and LFO's
		this->e2 = e1;
		this->osc1 = osc1;
		volume = osc1->volume;
		MI = osc1->MI;
		waveType = osc1->waveType;
		detune = osc1->detune;
		octave = osc1->octave;
		modtype = osc1->modtype;
		
		this->osc2 = osc2;
		volume2 = osc2->volume;
		MI2 = osc2->MI;
		waveType2 = osc2->waveType;
		detune2 = osc2->detune;
		octave2 = osc2->octave;
	
		this->l1 = l2;
		miLFO = l1->MI;
		freqLFO = l1->freqVal;
		LFOtype = l1->waveType;
	}
	
	
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    // Set up any necessary variables here
    //Flags for checking if a note is playing or stopped
	bool playing = false;
	bool ending = false;

	ADSR::Parameters envParams;  // Envelope Parameters
	ADSR env;					 // Envelope
	Envelope_note* e2;
	
	Reverb rvb;					//Rever

	//creating a wavetable variables and associated pointers
	waveTableSynth wt1, wt2, lfo1;
	OscillatorType *osc1, *osc2;
	LFO* l1;

	// Though there is no need to use these pointers as we aleady have them in
	// structure object, they are used for easier coding
	float* waveType, * detune, * MI, * volume, * octave, * modtype;
	float* waveType2, * detune2, * MI2, * volume2, * octave2;
	float* LFOtype, * miLFO, * freqLFO;

	float *masterVol;
    /// Variables for storing temporary variables
	float current_val, newfreq, newfreq2, osc_val1, osc_val2,freq;

};
