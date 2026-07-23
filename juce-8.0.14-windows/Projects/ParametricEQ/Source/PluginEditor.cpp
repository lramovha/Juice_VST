/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

ParametricEQAudioProcessorEditor::ParametricEQAudioProcessorEditor(ParametricEQAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    auto setupSlider = [&](juce::Slider& slider)
        {
            slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
            addAndMakeVisible(slider);
        };

    setupSlider(peakFreqSlider);
    setupSlider(peakGainSlider);
    setupSlider(peakQSlider);

    peakFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "peak_freq", peakFreqSlider);

    peakGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "peak_gain", peakGainSlider);

    peakQAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "peak_q", peakQSlider);

    setSize(720, 420);
}

ParametricEQAudioProcessorEditor::~ParametricEQAudioProcessorEditor() = default;

void ParametricEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1E1E1E)); // dark background

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawFittedText("Parametric EQ", getLocalBounds().removeFromTop(50),
        juce::Justification::centred, 1);
}

void ParametricEQAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(40);
    area.removeFromTop(60); // space for title

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    fb.items.add(juce::FlexItem(peakFreqSlider).withFlex(1).withMargin(10));
    fb.items.add(juce::FlexItem(peakGainSlider).withFlex(1).withMargin(10));
    fb.items.add(juce::FlexItem(peakQSlider).withFlex(1).withMargin(10));

    fb.performLayout(area);
}
