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
#include <random>

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
    
    static constexpr const char * SourceID = "sourceID";

    static constexpr const char * SourceImpulse = "Impulse";
    static constexpr const char * SourceRandomImpulse = "Random impulse";

    static constexpr const char * HarmonicsGroupID = "HarmonicsGroupID";
    static constexpr const char * HarmonicGainID = "HarmonicGainID";
    static constexpr const char * HarmonicDecayID = "HarmonicDecayID";

    enum Source
    {
        Impulse=1,
        RandomImpulse
    };
    
    static constexpr int NumberOfFilters {10};

private:
    //==============================================================================
    static constexpr double InitCutoffFrequency{1000.0};
    static constexpr double InitTau{0.01};
    static constexpr double InitSampleRate{48000.0};


    static constexpr std::array<double, NumberOfFilters> Harmonics = {1, 3, 5, 7, 9 , 11, 13, 15, 17, 19};
    static constexpr std::array<double, NumberOfFilters> HarmonicDecays = {1, 0.8, 0.6, 0.4, 0.2, 0.1, 0.05, 0.025, 0.002, 0.001};

    std::array<juce::AudioParameterFloat*, NumberOfFilters> m_harmonicGains;
    std::array<juce::AudioParameterFloat*, NumberOfFilters> m_harmonicDecays;

    float m_cutoffFrequency{1000.0f};

    using StereoFilter =  std::array<MVMFilter, 2>;
    std::array<StereoFilter, NumberOfFilters> m_filters;
    std::unique_ptr<juce::AudioProcessorValueTreeState> m_parameters;
    std::array<std::atomic<bool>, 2> m_ping{false};
    
    juce::MidiKeyboardState m_keyboardState;

    std::mt19937 m_genValue;
    std::bernoulli_distribution m_value{0.01}; // Biased towards generating 0.0 to avoid saturating the filters

    juce::AudioParameterChoice * m_sourceParameter;
    Source m_source{Source::Impulse};
    double getNextSourceSample(int channel);
    double impulse(int channel);
    double randomImpulse(int channel);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MVMFilterAudioProcessor)
};
