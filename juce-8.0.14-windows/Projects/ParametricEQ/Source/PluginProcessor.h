/*
  ==============================================================================

    PluginProcessor.h

    Declares the main audio processor for the Parametric EQ plugin.

    The processor is responsible for:
        - Managing plugin parameters
        - Preparing DSP components
        - Processing incoming audio
        - Saving and restoring plugin state
        - Creating the plugin editor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EQBand.h"

//==============================================================================
// Main audio processing class.
//
// This class is created by the audio host (FL Studio, Reaper, Ableton, etc.)
// and performs all real-time DSP operations.
class ParametricEQAudioProcessor : public juce::AudioProcessor
{
public:
    //--------------------------------------------------------------------------
    // Creates and initialises the plugin processor.
    ParametricEQAudioProcessor();

    // Destroys the processor and releases resources.
    ~ParametricEQAudioProcessor() override;

    //--------------------------------------------------------------------------
    // Called before audio playback begins.
    // Used to initialise DSP objects.
    void prepareToPlay(double sampleRate,
        int samplesPerBlock) override;

    // Called when playback stops.
    // Releases or resets DSP resources.
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations

    //--------------------------------------------------------------------------
    // Determines which channel layouts the plugin supports.
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

#endif

    //--------------------------------------------------------------------------
    // Processes one block of incoming audio.
    void processBlock(juce::AudioBuffer<float>&,
        juce::MidiBuffer&) override;

    //--------------------------------------------------------------------------
    // Creates the plugin's graphical user interface.
    juce::AudioProcessorEditor* createEditor() override;

    // Returns true because this plugin provides a custom editor.
    bool hasEditor() const override;

    //--------------------------------------------------------------------------
    // Returns the plugin name displayed by the host.
    const juce::String getName() const override;

    //--------------------------------------------------------------------------
    // MIDI capability information.
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;

    // Returns the plugin's tail length.
    double getTailLengthSeconds() const override;

    //--------------------------------------------------------------------------
    // Program (preset) management.
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index,
        const juce::String& newName) override;

    //--------------------------------------------------------------------------
    // Saves and restores the plugin state.
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data,
        int sizeInBytes) override;

    //--------------------------------------------------------------------------
    // Stores and manages all plugin parameters.
    //
    // Provides automatic synchronisation between the processor,
    // editor, and host automation.
    juce::AudioProcessorValueTreeState parameters;

private:
    //--------------------------------------------------------------------------
    // Creates and registers all plugin parameters.
    juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    //--------------------------------------------------------------------------
    // Single parametric EQ (peaking) filter.
    //
    // Future versions may include multiple EQ bands.
    EQBand peakBand;

    // Prevent copying and enable JUCE's memory leak detection.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        ParametricEQAudioProcessor)
};