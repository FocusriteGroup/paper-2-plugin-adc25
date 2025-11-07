/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include <array>
#include <memory>

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
    
    static constexpr int SliderWidth = 100;
    static constexpr int SliderHeight = 200;

    static constexpr int FooterHeight = 100;
    static constexpr int HeaderHeight = 100;

    static constexpr int MarginWidth = 20;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MVMFilterAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& m_valueTreeState;
    
    juce::ComboBox m_sourceChoice;
    juce::Label m_sourceChoiceLabel;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment m_comboBoxAttachement;
    
    using SliderAttachements = std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, MVMFilterAudioProcessor::NumberOfFilters>;
    std::array<juce::Slider, MVMFilterAudioProcessor::NumberOfFilters> m_harmonicGainSliders;
    std::array<juce::Label, MVMFilterAudioProcessor::NumberOfFilters> m_harmonicGainLabels;
    SliderAttachements m_harmonicGainSliderAttachements;

    std::array<juce::Slider, MVMFilterAudioProcessor::NumberOfFilters> m_harmonicDecaySliders;
    std::array<juce::Label, MVMFilterAudioProcessor::NumberOfFilters> m_harmonicDecayLabels;
    SliderAttachements m_harmonicDecaySliderAttachements;

    juce::MidiKeyboardComponent m_keyboardComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MVMFilterAudioProcessorEditor)
};
