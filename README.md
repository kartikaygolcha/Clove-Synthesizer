# Clove-Synthesizer
                                                         CLOVE Synthesizer
Clove is a digital implementation of a basic analog synthesizer. The main idea behind such a synthesizer was to enable the user to explore the synthesis of basic sounds and play with their behaviours to create explorative sounds that can be used in sound design and composition. 

The synthesizer aim was to emulate the features that are generally available on an analog synthesizer. The design of the synthesizer is based on wavetable design, where a table stores a single period of the wave and you can change frequency and tonality by changing the skip size through the table. We have used 7 rows of waves to be used for rendering all the intended sounds.


The synthesizer has two main oscillators (Osc1 and Osc2) and one low Freq oscillator (LFO). Each oscillator has 4 types of waves namely Sin, Saw, Square and Triangle. Osc1 is the main oscillator and can be applied effects on. Each oscillator has a detune slider to change the note frequency in cents and an octave slider to change the octave thus enabling the user to create sub sounds or higher pitch sounds as well. A volume control is provided for each oscillator to change the relative levels of the output of that wave. Both the outputs are added to produce the final output and then sent to the effects chain. Osc1 has a slider for Modulation. This has been to provide some special effects like changing the pulse width of the square wave, adding an additional wave with a frequency that is higher in ratio to the original wave. Eg. A value of 0.5 will produce a sound that is the carrier frequency plus 1.5 times the carrier Frequency. This helps in creating warmer sounds. Other than that, Osc1 has a Modulation Drop box from which it can receive modulations from either the OSc2 or LFO. The Modulations can be chosen from Frequency Modulation, Ring Modulation and Amplitude Modulation. Frequency modulation is changing the frequency of the main wave according to the amplitude value of the modulating wave. Linear FM has been implemented with frequency in +/-0.5 range of the carrier frequency. The Modulation Index here scales the range of the described +/-0.5 frequency range.  The nature of sounds produced by FM by a modulating oscillator in hearing range and an LFO is very different and interesting. The next type of modulation implemented is the Amplitude Modulation; In this, the modulating wave’s value is added to 1(later halved) and then multiplied to the carrier wave. The modulating index changes the scale the modulating wave amplitude. Similar to AM, Ring modulation is also implemented. In Ring Modulation, both the amplitude of both the waves are multiplied and the Modulation Index scales this amplitude. This is different from AM, as it produces two frequencies that are the sum and the difference of the carrier and modulating wave frequency. The Modulation amount can be controlled by a slider MI in each of Osc2 and LFO wherever the modulation is coming from. The LFO has a frequency range from 0.01hz to 30Hz and can be applied to modulate Osc1. Sin/Square/Tiangle and Saw type LFO are available.


An Envelope is provided to give a control over how the note is to be played. The range for Attack, Decay and Release is from 0-1s, thus giving a good flexibility on how much control is required. A filter cascaded with reverb is implemented. The filter that is employed is an IIR filter. A drop-down menu is given to choose from high pass, low pass and band pass type of filters. Two sliders for cut-off frequency and the Q value is provided. Other than that Reverb can be added by using a button. 


An additional feature is given which allows the user to change the keyboard to a chord keyboard of their choice. User can select a chord from the given list and striking a midi note triggers the associated chord to that note. This mode can be controlled by a button. This mode is not fail proof and needs debugging due to JUCE management of midi messages.
A keyboard is provided at the bottom of the screen for user to interface it with and play with sounds even as a standalone object.


Shortcomings that will be worked on:
1)	Chord mode is buggy because of sorting of midi messages by juce
2)	No Delay based effects like Chorus/Flanger
3)	Flexibility on more type of filters
4)	Parameter control for Reverb
