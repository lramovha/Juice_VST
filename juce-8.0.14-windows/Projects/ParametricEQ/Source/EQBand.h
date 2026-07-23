/*
  ==============================================================================

    EQBand.h
    Created: 23 Jul 2026 5:17:18pm
    Author:  lasyr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class EQBand
{
public:
    EQBand();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setFrequency(float freq);
    void setGain(float gainDB);
    void setQ(float q);

private:
    void updateCoefficients();

    juce::dsp::IIR::Filter<float> filter;
    juce::dsp::IIR::Coefficients<float>::Ptr coefficients;

    float currentFreq = 1000.0f;
    float currentGain = 0.0f;
    float currentQ = 1.0f;
    double currentSampleRate = 44100.0;
};
