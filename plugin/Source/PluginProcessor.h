/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <array>
#include <atomic>
#include <memory>

#include "../../source/MVMFilter.h"

//==============================================================================
/**
*/
class MVMFilterAudioProcessor  : public juce::AudioProcessor, private juce::MidiKeyboardStateListener
{
public:
    //==============================================================================
    MVMFilterAudioProcessor();
    ~MVMFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    static constexpr const char * TauID = "tauID";
    static constexpr const char * FsID = "fsID";
    
private:
    //==============================================================================
    
    static constexpr double InitCutoffFrequency{1000.0};
    static constexpr double InitTau{0.1};
    static constexpr double InitSampleRate{48000};
    
    juce::AudioParameterFloat * m_tau;
    float m_cutoffFrequency{1000.0f};
    std::array<MVMFilter, 2> m_filter;
    std::unique_ptr<juce::AudioProcessorValueTreeState> m_parameters;
    std::array<std::atomic<bool>, 2> m_ping{false};
    
    juce::MidiKeyboardState m_keyboardState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MVMFilterAudioProcessor)
};
