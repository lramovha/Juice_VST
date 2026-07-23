/*
  ==============================================================================

    PluginProcessor.cpp

    Implements the main audio processing class for the Parametric EQ plugin.

    Responsibilities:
        - Create and register plugin parameters
        - Prepare DSP objects before playback
        - Process incoming audio
        - Manage plugin lifecycle
        - Create the plugin editor

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor
//
// Initialises the audio processor, configures the input/output buses,
// and creates the AudioProcessorValueTreeState that stores all plugin
// parameters.
ParametricEQAudioProcessor::ParametricEQAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this,
        nullptr,
        juce::Identifier("PARAMETRICEQ"),
        createParameterLayout())
{
}

//==============================================================================
// Destructor
ParametricEQAudioProcessor::~ParametricEQAudioProcessor() = default;

//==============================================================================
// Creates all parameters used by the plugin.
//
// These parameters automatically appear in the GUI through
// AudioProcessorValueTreeState attachments.
juce::AudioProcessorValueTreeState::ParameterLayout ParametricEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Helper to add standard band parameters
    auto addBand = [&](const juce::String& idPrefix, const juce::String& name, float defaultFreq, bool hasGain = true)
    {
        layout.add (std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID {idPrefix + "_freq", 1}, name + " Frequency",
            juce::NormalisableRange<float>(20.0f, 20000.0f, 0.0f, 0.3f), defaultFreq, "Hz"));

        if (hasGain)
        {
            layout.add (std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID {idPrefix + "_gain", 1}, name + " Gain",
                juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));
        }

        layout.add (std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID {idPrefix + "_q", 1}, name + " Q",
            juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 1.0f));
    };

    // High Pass Filter (no gain)
    addBand("hp", "HP", 20.0f, false);

    // Low Shelf
    addBand("lowShelf", "Low Shelf", 200.0f);

    // Peak
    addBand("peak", "Peak", 1000.0f);

    // High Shelf
    addBand("highShelf", "High Shelf", 5000.0f);

    // Low Pass Filter (no gain)
    addBand("lp", "LP", 20000.0f, false);

    return layout;
}

//==============================================================================
// Called before playback begins.
//
// Prepares all DSP objects using the sample rate, block size,
// and channel count supplied by the host.
void ParametricEQAudioProcessor::prepareToPlay(double sampleRate,
    int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;

    // Prepare the peak EQ band.
    peakBand.prepare(spec);
}

//==============================================================================
// Called when playback stops.
//
// Resets the internal state of the DSP objects.
void ParametricEQAudioProcessor::releaseResources()
{
    peakBand.reset();
}

//==============================================================================
// Main audio processing function.
//
// Called once for every block of incoming audio.
void ParametricEQAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer&)
{
    // Prevent CPU slowdowns caused by denormal floating-point numbers.
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //--------------------------------------------------------------------------
    // Clear any unused output channels.

    for (auto i = totalNumInputChannels;
        i < totalNumOutputChannels;
        ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // Wrap the audio buffer as a DSP AudioBlock.
    juce::dsp::AudioBlock<float> block(buffer);

    //--------------------------------------------------------------------------
    // Read the latest parameter values from the ValueTreeState.

    peakBand.setFrequency(
        *parameters.getRawParameterValue("peak_freq"));

    peakBand.setGain(
        *parameters.getRawParameterValue("peak_gain"));

    peakBand.setQ(
        *parameters.getRawParameterValue("peak_q"));

    // Process the audio through the EQ.
    peakBand.process(block);
}

//==============================================================================
// Creates the plugin's graphical editor.
juce::AudioProcessorEditor*
ParametricEQAudioProcessor::createEditor()
{
    return new ParametricEQAudioProcessorEditor(*this);
}

//==============================================================================
// Returns true because this plugin has a custom GUI.
bool ParametricEQAudioProcessor::hasEditor() const
{
    return true;
}

//==============================================================================
// Returns the plugin name.
const juce::String ParametricEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================
// MIDI support.
bool ParametricEQAudioProcessor::acceptsMidi() const
{
    return false;
}

bool ParametricEQAudioProcessor::producesMidi() const
{
    return false;
}

bool ParametricEQAudioProcessor::isMidiEffect() const
{
    return false;
}

//==============================================================================
// This plugin produces no tail after playback stops.
double ParametricEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
// Program (preset) management.
//
// Only a single default program is currently supported.
int ParametricEQAudioProcessor::getNumPrograms()
{
    return 1;
}

int ParametricEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParametricEQAudioProcessor::setCurrentProgram(int)
{
}

const juce::String ParametricEQAudioProcessor::getProgramName(int)
{
    return {};
}

void ParametricEQAudioProcessor::changeProgramName(
    int,
    const juce::String&)
{
}

//==============================================================================
// Saves the plugin state.
//
// Parameter saving will be implemented later.
void ParametricEQAudioProcessor::getStateInformation(
    juce::MemoryBlock&)
{
}

//==============================================================================
// Restores the plugin state.
//
// Parameter loading will be implemented later.
void ParametricEQAudioProcessor::setStateInformation(
    const void*,
    int)
{
}

#ifndef JucePlugin_PreferredChannelConfigurations

//==============================================================================
// Restricts the plugin to stereo input/output.
bool ParametricEQAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet()
        == juce::AudioChannelSet::stereo();
}

#endif

//==============================================================================
// Factory function.
//
// Called by the host to create a new plugin instance.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQAudioProcessor();
}