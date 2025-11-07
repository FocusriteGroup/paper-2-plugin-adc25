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
    , m_sliderAttachement(v, MVMFilterAudioProcessor::TauID, m_tauSlider)
    , m_keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (&m_tauSlider);
    m_tauLabel.setText("Tau", juce::dontSendNotification);
    m_tauLabel.attachToComponent(&m_tauSlider, true);
    
    addAndMakeVisible (m_keyboardComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);
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

    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox.items.add(juce::FlexItem(m_tauSlider).withWidth(ComponentWidth).withHeight(ComponentHeight));
    flexBox.performLayout(localBounds);
    
    auto footerBounds = getLocalBounds().removeFromBottom(100);
    m_keyboardComponent.setBounds(footerBounds);
}

