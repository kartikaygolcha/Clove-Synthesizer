/*
  ==============================================================================

	PluginProcessor.cpp
	Created: 21 Apr 2020 5:41:52pm
	Author:  Kartikay
	Description: Class Description for the plugin class .

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CloveAudioProcessor::CloveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),        //Declaring all the ValueStateTree class Consrtuctors 
tree(*this, nullptr, "Envelope_Filter",
	{ std::make_unique<AudioParameterFloat>("attack", "Attack", NormalisableRange<float>(0.0f, 1.0f), 0.1f),
		std::make_unique<AudioParameterFloat>("decay", "Decay", NormalisableRange<float>(0.0f, 1.0f), 0.1f),
		std::make_unique<AudioParameterFloat>("sustain", "Sustain", NormalisableRange<float>(0.0f, 1.0f), 0.8f),
		std::make_unique<AudioParameterFloat>("release", "Release", NormalisableRange<float>(0.0f, 1.0f), 0.1f),
		std::make_unique<AudioParameterFloat>("filterType", "FilterType", NormalisableRange<float>(0.0f, 2.0f), 0.0f),
		std::make_unique<AudioParameterFloat>("filterCutoff", "FilterCutoff", NormalisableRange<float>(20.0f, 10000.0f), 400.0f),
		std::make_unique<AudioParameterFloat>("filterRes", "FilterRes", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
		std::make_unique<AudioParameterFloat>("masterGain", "Master Gain", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
		std::make_unique<AudioParameterBool>("Chord", "Chord",false ),
	    std::make_unique<AudioParameterBool>("Reverb", "Reverb",false),
	}),
	oscParam(*this, nullptr, "oscillator1",
		{ std::make_unique<AudioParameterFloat>("detune", "Detune_osc1", NormalisableRange<float>(-600.0f, 600.0f),0.0f),
			std::make_unique<AudioParameterFloat>("MI", "Color_osc1", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("volume", "Volume_osc1", NormalisableRange<float>(0.0f, 1.0f), 0.8f),
			std::make_unique<AudioParameterFloat>("octave", "Octave_osc1", NormalisableRange<float>(-3.00f, 3.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("oscType", "oscType_osc1", NormalisableRange<float>(0.0f, 3.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("modType", "modType_osc1", NormalisableRange<float>(0.0f, 5.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("MILFO", "mi_LFO", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("oscTypeLFO", "oscTypeLFO", NormalisableRange<float>(0.0f, 3.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("freqValLFO", "freqValLFO", NormalisableRange<float>(0.01f,30.0f), 0.01f),
		}),
	oscParam2(*this, nullptr, "oscillator2",
		{ std::make_unique<AudioParameterFloat>("detune2", "Detune_osc2", NormalisableRange<float>(-600.0f, 600.0f),0.0f),
			std::make_unique<AudioParameterFloat>("MI2", "MI_osc2", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("volume2", "Volume_osc2", NormalisableRange<float>(0.0f, 1.0f), 0.8f),
			std::make_unique<AudioParameterFloat>("octave2", "Octave_osc2", NormalisableRange<float>(-3.0f, 3.0f), 0.0f),
			std::make_unique<AudioParameterFloat>("oscType2", "oscType_osc2", NormalisableRange<float>(0.0f, 3.0f), 0.0f),


		})

#endif
{
	// Initialize all voices
	for (int i = 0; i < voiceCount; i++)
	{
		synth.addVoice(new MySynthVoice());
	}
	// Add the sound
	synth.addSound(new MySynthSound());
	
	// Get memory for each of the Structures 
	env = new Envelope_note();
	osc1 = new OscillatorType();
	osc2 = new OscillatorType();
	l1 = new LFO();

	// Assign Envelope Pointers
	env->attack = tree.getRawParameterValue("attack");
	env->decay = tree.getRawParameterValue("decay");
	env->sustain = tree.getRawParameterValue("sustain");
	env->release = tree.getRawParameterValue("release");

	// Assign OScillator 1 Pointers
	osc1->waveType = oscParam.getRawParameterValue("oscType");
	osc1->volume = oscParam.getRawParameterValue("volume");
	osc1->MI = oscParam.getRawParameterValue("MI");
	osc1->detune = oscParam.getRawParameterValue("detune");
	osc1->octave = oscParam.getRawParameterValue("octave");
	osc1->modtype = oscParam.getRawParameterValue("modType");

	// Assign OScillator 2 Pointers
	osc2->waveType = oscParam2.getRawParameterValue("oscType2");
	osc2->volume = oscParam2.getRawParameterValue("volume2");
	osc2->MI = oscParam2.getRawParameterValue("MI2");
	osc2->detune = oscParam2.getRawParameterValue("detune2");
	osc2->octave = oscParam2.getRawParameterValue("octave2");

	// Assign LFO Pointers
	l1->waveType = oscParam.getRawParameterValue("oscTypeLFO");
	l1->MI = oscParam.getRawParameterValue("MILFO");
	l1->freqVal = oscParam.getRawParameterValue("freqValLFO");

	//Pointers for chords and reverb buttons
	chord_style = tree.getRawParameterValue("Chord");
	Reverb_State = tree.getRawParameterValue("Reverb");
	setchord(1);

	//Master Volume control
	masterVol = tree.getRawParameterValue("masterGain");
}

CloveAudioProcessor::~CloveAudioProcessor()
{
}

//==============================================================================
const String CloveAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool CloveAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool CloveAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool CloveAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double CloveAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int CloveAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int CloveAudioProcessor::getCurrentProgram()
{
	return 0;
}

void CloveAudioProcessor::setCurrentProgram(int index)
{
}

const String CloveAudioProcessor::getProgramName(int index)
{
	return {};
}

void CloveAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void CloveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	this->sampleRate = sampleRate;

	// Setting Reverb Parameters
	Reverb::Parameters reverbParams;
	reverbParams.dryLevel = 0.9f;
	reverbParams.wetLevel = 0.3f;
	reverbParams.roomSize = 0.75f;
	rvb.reset();
	rvb.setParameters(reverbParams);

	//Resetting the filter
	filterLeft.reset();
	filterRight.reset();

	// Set Sample Rate for the Synth
	synth.setCurrentPlaybackSampleRate(sampleRate);

	//set SampleRate and send parameters for all the voices 
	for (int i = 0; i < voiceCount; i++)
	{
		MySynthVoice* v = dynamic_cast<MySynthVoice*>(synth.getVoice(i));
		v->setParameters(env,osc1,osc2,l1,masterVol);
		v->init(sampleRate);

	}
}

void CloveAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CloveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif
/* Update Filter coeffiencents*/
void CloveAudioProcessor::updateFilter()
{
	int menuChoice = *tree.getRawParameterValue("filterType");
	float freq = *tree.getRawParameterValue("filterCutoff");
	float res = *tree.getRawParameterValue("filterRes");

	if (menuChoice == 0)
	{
		filterLeft.setCoefficients(IIRCoefficients::makeLowPass(sampleRate,freq, res));
		filterRight.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, freq, res));
	}

	if (menuChoice == 1)
	{
		filterLeft.setCoefficients(IIRCoefficients::makeHighPass(sampleRate, freq, res));
		filterRight.setCoefficients(IIRCoefficients::makeHighPass(sampleRate, freq, res));
	}

	if (menuChoice == 2)
	{
		filterLeft.setCoefficients(IIRCoefficients::makeBandPass(sampleRate, freq, res));
		filterRight.setCoefficients(IIRCoefficients::makeBandPass(sampleRate, freq, res));

	}
}
/*!

This takes an incoming midi and adds midi events accordingly

@Params MidiBuffer-> midiMessages

*/
void CloveAudioProcessor::processMidiInfo(MidiBuffer &midiMessages)
{
	MidiMessage msg;
	int indexPos;

	//If chord button is on or off
	if (chord_style) {
		//For each midi event 
		for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, indexPos);)                          // [9]
		{
			// check in a note is pressed
			if (msg.isNoteOn())
			{
				//add the other chord events and maintain a list 
				midiMessages.addEvent(MidiMessage::noteOn(msg.getChannel(), msg.getNoteNumber() + chord1, (uint8)msg.getVelocity()), (indexPos + 1));
				addedMidi.add(msg.getNoteNumber() + chord1);

				midiMessages.addEvent(MidiMessage::noteOn(msg.getChannel(), msg.getNoteNumber() + chord2, (uint8)msg.getVelocity()), indexPos + 2);
				addedMidi.add(msg.getNoteNumber() + chord2);

				if (num_chord == 3)
				{
					midiMessages.addEvent(MidiMessage::noteOn(msg.getChannel(), msg.getNoteNumber() + chord3, (uint8)msg.getVelocity()), indexPos + 3);
					addedMidi.add(msg.getNoteNumber() + chord3);

				}

			}
			else if (msg.isNoteOff())
			{
				// Check if the midi array is not empty and size is greater than the number of added events 
				if (!addedMidi.isEmpty() && addedMidi.size()>=num_chord)
				{
					// add Note Off Messages and Remove from list
					midiMessages.addEvent(MidiMessage::noteOff(msg.getChannel(), addedMidi.getFirst(), (uint8)0), indexPos + 1);
					addedMidi.removeAndReturn(0);
					midiMessages.addEvent(MidiMessage::noteOff(msg.getChannel(), addedMidi.getFirst(), (uint8)0), indexPos + 1);
					addedMidi.removeAndReturn(0);
					if (num_chord == 3) {
						midiMessages.addEvent(MidiMessage::noteOff(msg.getChannel(), addedMidi.getFirst(), (uint8)0), indexPos + 1);
						addedMidi.removeAndReturn(0);
					}
				}
			}
			//Iterate through added midi elements so I dont process the wrong elements
			for (int g = 0; g < num_chord; g++)
			{
				it.getNextEvent(msg, indexPos);
			}
		}

	}
}

void CloveAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	//Process Keyboardstate for MidiBuffer messanges
	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

	// Iterate though the midibuffer and add events 
	// Though Its not a perfect implementation and buggy at the moment
	processMidiInfo(midiMessages);

	// Get the sample block from the synth
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	//Update Filter and Apply
	updateFilter();
	filterLeft.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
	filterRight.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());

	//Apply reverb
	if(Reverb_State)
		rvb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
	
}

//==============================================================================
bool CloveAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CloveAudioProcessor::createEditor()
{
	return new CloveAudioProcessorEditor(*this);
}

//==============================================================================
void CloveAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	// Storing the State Information
	auto state = tree.copyState();
	state.addChild(oscParam.copyState(),1,oscParam.undoManager);
	state.addChild(oscParam2.copyState(), 2, oscParam2.undoManager);
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void CloveAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	//Reading back the State Information 
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	auto c=xmlState->getChildElement(1);
	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(tree.state.getType()))
		{
			tree.replaceState(ValueTree::fromXml(*xmlState));
		}
		if (xmlState->getChildElement(1)->hasTagName(oscParam.state.getType()))
		{
			oscParam.replaceState(ValueTree::fromXml(*xmlState).getChild(1));
		}
		if (xmlState->getChildElement(2)->hasTagName(oscParam2.state.getType()))
		{
			oscParam2.replaceState(ValueTree::fromXml(*xmlState).getChild(2));
		}
	}
}

// Sets the chord values using chordnum in the list
void CloveAudioProcessor::setchord(int chordnum)
{
	if (chordnum > 4)
		num_chord = 3;
	else	
		num_chord = 2;
	
	chord1 = chord_1_arr[chordnum - 1];
	chord2 = chord_2_arr[chordnum - 1];
	chord3 = chord_3_arr[chordnum - 1];
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new CloveAudioProcessor();
}

