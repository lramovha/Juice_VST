/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

ParametricEQAudioProcessor::ParametricEQAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("PARAMETRICEQ"),
        createParameterLayout())
{
}

ParametricEQAudioProcessor::~ParametricEQAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout ParametricEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "peak_freq", 1 }, "Peak Frequency",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 0.0f, 0.3f), 1000.0f, "Hz"));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "peak_gain", 1 }, "Peak Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "peak_q", 1 }, "Peak Q",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 1.0f));

    return layout;
}

void ParametricEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;

    peakBand.prepare(spec);
}

void ParametricEQAudioProcessor::releaseResources()
{
    peakBand.reset();
}

void ParametricEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);

    // Update parameters smoothly
    peakBand.setFrequency(*parameters.getRawParameterValue("peak_freq"));
    peakBand.setGain(*parameters.getRawParameterValue("peak_gain"));
    peakBand.setQ(*parameters.getRawParameterValue("peak_q"));

    peakBand.process(block);
}

juce::AudioProcessorEditor* ParametricEQAudioProcessor::createEditor()
{
    return new ParametricEQAudioProcessorEditor(*this);
}

bool ParametricEQAudioProcessor::hasEditor() const { return true; }
const juce::String ParametricEQAudioProcessor::getName() const { return JucePlugin_Name; }
bool ParametricEQAudioProcessor::acceptsMidi() const { return false; }
bool ParametricEQAudioProcessor::producesMidi() const { return false; }
bool ParametricEQAudioProcessor::isMidiEffect() const { return false; }
double ParametricEQAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int ParametricEQAudioProcessor::getNumPrograms() { return 1; }
int ParametricEQAudioProcessor::getCurrentProgram() { return 0; }
void ParametricEQAudioProcessor::setCurrentProgram(int) {}
const juce::String ParametricEQAudioProcessor::getProgramName(int) { return {}; }
void ParametricEQAudioProcessor::changeProgramName(int, const juce::String&) {}
void ParametricEQAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void ParametricEQAudioProcessor::setStateInformation(const void*, int) {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParametricEQAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQAudioProcessor();
}
