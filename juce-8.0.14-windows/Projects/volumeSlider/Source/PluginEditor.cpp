/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VolumeSliderAudioProcessorEditor::VolumeSliderAudioProcessorEditor (VolumeSliderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Slider setup
    gainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 80, 30);
    gainSlider.setTextValueSuffix (" x");
    addAndMakeVisible (gainSlider);

    // Connect slider to parameter
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "gain", gainSlider);

    setSize (400, 300);
}

VolumeSliderAudioProcessorEditor::~VolumeSliderAudioProcessorEditor() = default;

//==============================================================================
void VolumeSliderAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("Volume Slider", getLocalBounds().removeFromTop(50),
                      juce::Justification::centred, 1);
}

void VolumeSliderAudioProcessorEditor::resized()
{
    gainSlider.setBounds (120, 80, 160, 160);
}