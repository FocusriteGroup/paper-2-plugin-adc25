/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MVMFilterAudioProcessorEditor::MVMFilterAudioProcessorEditor (MVMFilterAudioProcessor& p, 
                                                              juce::AudioProcessorValueTreeState& v,
                                                              juce::MidiKeyboardState& keyboardState)
    : AudioProcessorEditor (&p)
    , audioProcessor (p)
    , m_valueTreeState(v)
    , m_keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    for (int i=0; i<MVMFilterAudioProcessor::NumberOfFilters; i++)
    {
        auto indexString = "_" + std::to_string(i);
        m_harmonicGainSliderAttachements[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(v, MVMFilterAudioProcessor::HarmonicGainID + indexString, m_harmonicGainSliders[i]);
        addAndMakeVisible (&m_harmonicGainSliders[i]);
        m_harmonicGainSliders[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        m_harmonicGainSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        m_harmonicGainLabels[i].setText("HG" + std::to_string(i), juce::dontSendNotification);
        m_harmonicGainLabels[i].attachToComponent(&m_harmonicGainSliders[i], false);

        m_harmonicDecaySliderAttachements[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(v, MVMFilterAudioProcessor::HarmonicDecayID + indexString, m_harmonicDecaySliders[i]);
        addAndMakeVisible (&m_harmonicDecaySliders[i]);
        m_harmonicDecaySliders[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        m_harmonicDecaySliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        m_harmonicDecayLabels[i].setText("HD" + std::to_string(i), juce::dontSendNotification);
        m_harmonicDecayLabels[i].attachToComponent(&m_harmonicDecaySliders[i], false);
    }

    addAndMakeVisible (m_keyboardComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 400);
}

MVMFilterAudioProcessorEditor::~MVMFilterAudioProcessorEditor()
{
}

//==============================================================================
void MVMFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void MVMFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto localBounds = getLocalBounds();
    auto footerBounds = localBounds.removeFromBottom(FooterHeight);
    m_keyboardComponent.setBounds(footerBounds);
        
    juce::FlexBox sliderGainsFlexBox;
    sliderGainsFlexBox.flexDirection = juce::FlexBox::Direction::row;
    sliderGainsFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    sliderGainsFlexBox.alignItems = juce::FlexBox::AlignItems::center;
    for (int i=0; i<MVMFilterAudioProcessor::NumberOfFilters; i++)
    {
        sliderGainsFlexBox.items.add(juce::FlexItem(m_harmonicGainSliders[i]).withWidth(SliderWidth).withHeight(SliderHeight));
    }

    auto harmonicGainBounds = localBounds.removeFromLeft(localBounds.getWidth() / 2);
    harmonicGainBounds.removeFromLeft(MarginWidth);
    harmonicGainBounds.removeFromRight(MarginWidth);

    sliderGainsFlexBox.performLayout(harmonicGainBounds);

    juce::FlexBox sliderDecaysFlexBox;
    sliderDecaysFlexBox.flexDirection = juce::FlexBox::Direction::row;
    sliderDecaysFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    sliderDecaysFlexBox.alignItems = juce::FlexBox::AlignItems::center;
    for (int i=0; i<MVMFilterAudioProcessor::NumberOfFilters; i++)
    {
        sliderDecaysFlexBox.items.add(juce::FlexItem(m_harmonicDecaySliders[i]).withWidth(SliderWidth).withHeight(SliderHeight));
    }

    localBounds.removeFromLeft(MarginWidth);
    localBounds.removeFromRight(MarginWidth);
    sliderDecaysFlexBox.performLayout(localBounds);
}

