/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
MVMFilterAudioProcessor::MVMFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    m_parameters = std::make_unique<juce::AudioProcessorValueTreeState> (*this,
                                                                         nullptr,
                                                                         juce::Identifier ("MVMFilter"),
                                                                         juce::AudioProcessorValueTreeState::ParameterLayout {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {TauID, 1},
                                                    "Tau",
                                                    juce::NormalisableRange<float> (0.0f, 1.0f), static_cast<float>(InitTau))});
    
    m_tau = dynamic_cast<juce::AudioParameterFloat *> (m_parameters->getParameter (TauID));

    m_keyboardState.addListener(this);
}
 
MVMFilterAudioProcessor::~MVMFilterAudioProcessor()
{
    m_keyboardState.removeListener (this);
}

//==============================================================================
const juce::String MVMFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MVMFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MVMFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MVMFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MVMFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MVMFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MVMFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MVMFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MVMFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void MVMFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MVMFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MVMFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MVMFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MVMFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (const auto metadata : midiMessages)
    {
         const auto midiMessage = metadata.getMessage();
         if (midiMessage.isNoteOn())
         {
             handleNoteOn(&m_keyboardState, midiMessage.getChannel(), midiMessage.getNoteNumber(), midiMessage.getFloatVelocity());
         }
         else if (midiMessage.isNoteOff())
         {
             handleNoteOff(&m_keyboardState, midiMessage.getChannel(), midiMessage.getNoteNumber(), midiMessage.getFloatVelocity());
         }
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
        
    m_filter[0].set(m_cutoffFrequency, m_tau->get());
    m_filter[1].set(m_cutoffFrequency, m_tau->get());
        
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
        {
            if(m_ping[channel])
            {
                channelData[sampleIndex] = m_filter[channel].process(1.0f);
                m_ping[channel] = false;
            }
            else
            {
                channelData[sampleIndex] = m_filter[channel].process(0.0f);
            }
        }
    }
}

//==============================================================================
bool MVMFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MVMFilterAudioProcessor::createEditor()
{
    return new MVMFilterAudioProcessorEditor (*this, *m_parameters, m_keyboardState);
}

//==============================================================================
void MVMFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MVMFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MVMFilterAudioProcessor();
}


//==============================================================================
void MVMFilterAudioProcessor::handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    for(auto& pingChannel : m_ping)
    {
        if(!pingChannel)
        {
            pingChannel = true;
        }
    }

    m_cutoffFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

void MVMFilterAudioProcessor::handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    for(auto& pingChannel : m_ping)
    {
        pingChannel = false;
    }
}

