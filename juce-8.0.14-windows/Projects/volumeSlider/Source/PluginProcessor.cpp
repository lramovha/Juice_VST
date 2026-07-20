/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

VolumeSliderAudioProcessor::VolumeSliderAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, juce::Identifier("PARAMS"),
        { std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID {"gain", 1}, "Gain", juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f) })
{
    // Safely retrieve the atomic float pointer from the APVTS
    gainParam = parameters.getRawParameterValue("gain");
}

VolumeSliderAudioProcessor::~VolumeSliderAudioProcessor() = default;

void VolumeSliderAudioProcessor::prepareToPlay(double, int) {}
void VolumeSliderAudioProcessor::releaseResources() {}

void VolumeSliderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // FIX: Use load() instead of dereferencing *gainParam directly
    float gain = gainParam->load();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            channelData[sample] *= gain;
    }
}

juce::AudioProcessorEditor* VolumeSliderAudioProcessor::createEditor()
{
    return new VolumeSliderAudioProcessorEditor(*this);
}

bool VolumeSliderAudioProcessor::hasEditor() const { return true; }
const juce::String VolumeSliderAudioProcessor::getName() const { return JucePlugin_Name; }

bool VolumeSliderAudioProcessor::acceptsMidi() const { return false; }
bool VolumeSliderAudioProcessor::producesMidi() const { return false; }
bool VolumeSliderAudioProcessor::isMidiEffect() const { return false; }
double VolumeSliderAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int VolumeSliderAudioProcessor::getNumPrograms() { return 1; }
int VolumeSliderAudioProcessor::getCurrentProgram() { return 0; }
void VolumeSliderAudioProcessor::setCurrentProgram(int) {}
const juce::String VolumeSliderAudioProcessor::getProgramName(int) { return {}; }
void VolumeSliderAudioProcessor::changeProgramName(int, const juce::String&) {}

void VolumeSliderAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VolumeSliderAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VolumeSliderAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}
#endif

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VolumeSliderAudioProcessor();
}
