/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ParametricEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ParametricEQAudioProcessorEditor(ParametricEQAudioProcessor&);
    ~ParametricEQAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ParametricEQAudioProcessor& audioProcessor;

    // Sliders for Peak band (we'll expand this)
    juce::Slider peakFreqSlider, peakGainSlider, peakQSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakQAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricEQAudioProcessorEditor)
};

