/*
  ==============================================================================

    EQBand.cpp
    Created: 23 Jul 2026 5:17:18pm
    Author:  lasyr

    Implementation of a single parametric EQ (peaking) filter band.
    This class wraps a JUCE IIR filter and provides methods for
    changing the filter's frequency, gain, and Q factor.

  ==============================================================================
*/

#include "EQBand.h"

//==============================================================================
/*
    Constructor

    Creates a default peaking filter so that the filter always has
    valid coefficients before prepare() is called.

    Default values:
        Sample Rate : 44100 Hz
        Frequency   : 1000 Hz
        Q           : 1.0
        Gain        : 0 dB (linear gain = 1.0)
*/
EQBand::EQBand()
{
    coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        44100.0,     // Default sample rate
        1000.0f,     // Centre frequency
        1.0f,        // Q factor
        1.0f);       // Linear gain (0 dB)

    // Assign the coefficients to the filter
    filter.coefficients = coefficients;
}

//==============================================================================
/*
    Prepares the filter before audio processing begins.

    This should be called once from the plugin's prepareToPlay() method.

    Parameters:
        spec - Contains sample rate, block size, and number of channels.
*/
void EQBand::prepare(const juce::dsp::ProcessSpec& spec)
{
    // Store the actual sample rate supplied by the host
    currentSampleRate = spec.sampleRate;

    // Prepare the internal filter for processing
    filter.prepare(spec);

    // Recalculate coefficients using the correct sample rate
    updateCoefficients();
}

//==============================================================================
/*
    Processes one block of audio.

    The incoming audio block is modified in place.
*/
void EQBand::process(juce::dsp::AudioBlock<float>& block)
{
    filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
/*
    Clears the filter's internal delay buffers.

    Useful when playback stops or transport position changes.
*/
void EQBand::reset()
{
    filter.reset();
}

//==============================================================================
/*
    Sets the centre frequency of the EQ band.

    The coefficients are only recalculated if the value changes
    by more than 0.01 Hz to avoid unnecessary work.
*/
void EQBand::setFrequency(float freq)
{
    if (std::abs(freq - currentFreq) > 0.01f)
    {
        currentFreq = freq;
        updateCoefficients();
    }
}

//==============================================================================
/*
    Sets the gain of the EQ band in decibels.

    Positive values boost the selected frequency.
    Negative values cut the selected frequency.
*/
void EQBand::setGain(float gainDB)
{
    if (std::abs(gainDB - currentGain) > 0.01f)
    {
        currentGain = gainDB;
        updateCoefficients();
    }
}

//==============================================================================
/*
    Sets the Q (quality factor) of the filter.

    Higher Q:
        - Narrower frequency band
        - More surgical EQ

    Lower Q:
        - Wider frequency band
        - Smoother EQ curve
*/
void EQBand::setQ(float q)
{
    if (std::abs(q - currentQ) > 0.01f)
    {
        currentQ = q;
        updateCoefficients();
    }
}

//==============================================================================
/*
    Rebuilds the filter coefficients using the current settings.

    This function is called whenever:
        - Frequency changes
        - Gain changes
        - Q changes
        - Sample rate changes

    Gain is stored in decibels but JUCE expects linear gain,
    so it is converted using Decibels::decibelsToGain().
*/
void EQBand::updateCoefficients()
{
    coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        currentSampleRate,
        currentFreq,
        currentQ,
        juce::Decibels::decibelsToGain(currentGain));

    // Apply the updated coefficients to the filter
    filter.coefficients = coefficients;
}