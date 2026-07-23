/*
  ==============================================================================

    EQBand.cpp
    Created: 23 Jul 2026 5:17:18pm
    Author:  lasyr

  ==============================================================================
*/

#include "EQBand.h"

EQBand::EQBand()
{
    coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.0, 1000.0f, 1.0f, 1.0f);
    filter.coefficients = coefficients;
}

void EQBand::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    filter.prepare(spec);
    updateCoefficients();
}

void EQBand::process(juce::dsp::AudioBlock<float>& block)
{
    filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void EQBand::reset()
{
    filter.reset();
}

void EQBand::setFrequency(float freq)
{
    if (std::abs(freq - currentFreq) > 0.01f)
    {
        currentFreq = freq;
        updateCoefficients();
    }
}

void EQBand::setGain(float gainDB)
{
    if (std::abs(gainDB - currentGain) > 0.01f)
    {
        currentGain = gainDB;
        updateCoefficients();
    }
}

void EQBand::setQ(float q)
{
    if (std::abs(q - currentQ) > 0.01f)
    {
        currentQ = q;
        updateCoefficients();
    }
}

void EQBand::updateCoefficients()
{
    coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        currentSampleRate, currentFreq, currentQ, juce::Decibels::decibelsToGain(currentGain));

    filter.coefficients = coefficients;
}