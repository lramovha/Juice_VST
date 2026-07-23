/*
  ==============================================================================

    This file implements the graphical user interface (GUI) for the
    Parametric EQ plugin.

    The editor is responsible for:
        - Creating and displaying controls
        - Connecting controls to plugin parameters
        - Drawing the interface
        - Positioning components when the window is resized

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
// Constructor
//
// Creates the editor, initialises all controls, connects them to the
// AudioProcessorValueTreeState, and sets the window size.
ParametricEQAudioProcessorEditor::ParametricEQAudioProcessorEditor(ParametricEQAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    //--------------------------------------------------------------------------
    // Helper lambda used to configure each rotary slider.
    auto setupSlider = [&](juce::Slider& slider, int decimalPlaces = 2)
        {
            // Display the slider as a rotary knob.
            slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

            // Display the current value below the knob.
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);

            // Add the slider to the editor.
            addAndMakeVisible(slider);
        };

    // Configure each EQ control.
    setupSlider(peakFreqSlider, 1);     // Frequency
    setupSlider(peakGainSlider, 2);     // Gain
    setupSlider(peakQSlider, 2);        // Q

    //--------------------------------------------------------------------------
    // Attach each slider to its corresponding parameter in the
    // AudioProcessorValueTreeState.
    //
    // These attachments keep the GUI and processor synchronized
    // automatically.

    peakFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.parameters,
            "peak_freq",
            peakFreqSlider);

    peakGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.parameters,
            "peak_gain",
            peakGainSlider);

    peakQAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.parameters,
            "peak_q",
            peakQSlider);

    // Set the initial plugin window size.
    setSize(720, 420);
}

//==============================================================================
// Destructor
//
// Smart pointers automatically clean up the slider attachments.
ParametricEQAudioProcessorEditor::~ParametricEQAudioProcessorEditor() = default;

//==============================================================================
// Paints the plugin interface.
//
// Called whenever the window needs repainting.
void ParametricEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill the background with a dark grey colour.
    g.fillAll(juce::Colour(0xFF1E1E1E));

    // Draw the plugin title.
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(24.0f, juce::Font::bold));

    g.drawFittedText(
        "Parametric EQ",
        getLocalBounds().removeFromTop(50),
        juce::Justification::centred,
        1);
}

//==============================================================================
// Positions all GUI components.
//
// Called automatically whenever the editor is resized.
void ParametricEQAudioProcessorEditor::resized()
{
    // Create a working area with padding around the edges.
    auto area = getLocalBounds().reduced(40);

    // Reserve space at the top for the title.
    area.removeFromTop(60);

    //--------------------------------------------------------------------------
    // Arrange the sliders using JUCE's FlexBox layout system.

    juce::FlexBox fb;

    // Place components in a horizontal row.
    fb.flexDirection = juce::FlexBox::Direction::row;

    // Evenly distribute the controls.
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    // Add the three rotary sliders.
    fb.items.add(
        juce::FlexItem(peakFreqSlider)
        .withFlex(1)
        .withMargin(10));

    fb.items.add(
        juce::FlexItem(peakGainSlider)
        .withFlex(1)
        .withMargin(10));

    fb.items.add(
        juce::FlexItem(peakQSlider)
        .withFlex(1)
        .withMargin(10));

    // Apply the layout to the available area.
    fb.performLayout(area);
}