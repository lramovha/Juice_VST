/*
  ==============================================================================

    PluginEditor.h

    Declares the graphical user interface (GUI) for the Parametric EQ plugin.

    The editor is responsible for:
        - Displaying the plugin controls
        - Connecting GUI components to audio parameters
        - Drawing the interface
        - Managing the layout of components

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Main editor class for the Parametric EQ plugin.
class ParametricEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    //--------------------------------------------------------------------------
    // Creates the editor and initialises all GUI components.
    ParametricEQAudioProcessorEditor(ParametricEQAudioProcessor&);

    // Destroys the editor and releases any owned resources.
    ~ParametricEQAudioProcessorEditor() override;

    //--------------------------------------------------------------------------
    // Paints the plugin window.
    void paint(juce::Graphics&) override;

    // Positions and resizes all GUI components.
    void resized() override;

private:
    //--------------------------------------------------------------------------
    // Reference to the plugin processor.
    // Used to access the AudioProcessorValueTreeState and plugin data.
    ParametricEQAudioProcessor& audioProcessor;

    //--------------------------------------------------------------------------
    // Rotary sliders controlling the Peak EQ band.

    // Centre frequency (Hz)
    juce::Slider peakFreqSlider;

    // Gain (dB)
    juce::Slider peakGainSlider;

    // Quality factor (Q)
    juce::Slider peakQSlider;

    //--------------------------------------------------------------------------
    // Slider attachments.
    //
    // These automatically synchronise each slider with its corresponding
    // AudioProcessorValueTreeState parameter.

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        peakFreqAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        peakGainAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        peakQAttachment;

    // Prevent copying and enable JUCE's memory leak detection.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricEQAudioProcessorEditor)
};