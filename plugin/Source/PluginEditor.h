/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MVMFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MVMFilterAudioProcessorEditor (MVMFilterAudioProcessor&,
                                   juce::AudioProcessorValueTreeState& v,
                                   juce::MidiKeyboardState& keyboardState);
    ~MVMFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    static constexpr int ComponentWidth = 200;
    static constexpr int ComponentHeight = 40;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MVMFilterAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& m_valueTreeState;
    
    juce::Slider m_tauSlider;
    juce::Label m_tauLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment m_sliderAttachement;

    juce::MidiKeyboardComponent m_keyboardComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MVMFilterAudioProcessorEditor)
};
