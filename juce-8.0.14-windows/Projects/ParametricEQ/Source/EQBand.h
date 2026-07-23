/*
  ==============================================================================

    EQBand.h
    Created: 23 Jul 2026 5:17:18pm
    Author:  lasyr

    Declaration of a single parametric EQ (peaking) filter band.

    This class wraps JUCE's IIR filter and provides methods to:
        - Prepare the filter for audio processing
        - Process audio blocks
        - Reset the filter state
        - Adjust frequency, gain, and Q factor

    Each parameter change automatically updates the filter coefficients.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
// Represents one adjustable parametric EQ band.
class EQBand
{
public:
    //--------------------------------------------------------------------------
    // Creates an EQ band with default values:
    // Frequency : 1000 Hz
    // Gain      : 0 dB
    // Q         : 1.0
    EQBand();

    //--------------------------------------------------------------------------
    // Prepares the filter before audio processing begins.
    // Called from the plugin's prepareToPlay() function.
    void prepare(const juce::dsp::ProcessSpec& spec);

    //--------------------------------------------------------------------------
    // Processes one block of audio.
    // The audio block is modified in place.
    void process(juce::dsp::AudioBlock<float>& block);

    //--------------------------------------------------------------------------
    // Clears the filter's internal state.
    void reset();

    //--------------------------------------------------------------------------
    // Sets the centre frequency (Hz) of the EQ band.
    void setFrequency(float freq);

    //--------------------------------------------------------------------------
    // Sets the gain (dB) applied at the centre frequency.
    void setGain(float gainDB);

    //--------------------------------------------------------------------------
    // Sets the Q (quality factor), controlling the filter bandwidth.
    void setQ(float q);

private:
    //--------------------------------------------------------------------------
    // Recalculates the filter coefficients whenever a parameter changes.
    void updateCoefficients();

    //--------------------------------------------------------------------------
    // JUCE IIR filter used to process the audio.
    juce::dsp::IIR::Filter<float> filter;

    // Shared pointer containing the current filter coefficients.
    juce::dsp::IIR::Coefficients<float>::Ptr coefficients;

    //--------------------------------------------------------------------------
    // Current filter parameters.

    // Centre frequency in Hertz.
    float currentFreq = 1000.0f;

    // Gain in decibels.
    float currentGain = 0.0f;

    // Quality factor (controls bandwidth).
    float currentQ = 1.0f;

    // Current sample rate supplied by the audio host.
    double currentSampleRate = 44100.0;
};