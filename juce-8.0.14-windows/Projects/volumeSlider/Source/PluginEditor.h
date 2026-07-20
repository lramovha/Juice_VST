/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class VolumeSliderAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    VolumeSliderAudioProcessorEditor(VolumeSliderAudioProcessor&);
    ~VolumeSliderAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is the glue that connects our editor to the processor
    VolumeSliderAudioProcessor& audioProcessor;

    // GUI Components
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeSliderAudioProcessorEditor)
};
