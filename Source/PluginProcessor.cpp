/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
TenBandAudioProcessor::TenBandAudioProcessor()
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
}

TenBandAudioProcessor::~TenBandAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout TenBandAudioProcessor::createParameterLayout() {
  return {
    std::make_unique<juce::AudioParameterFloat>("band1", "32.5Hz", juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band2", "62.5Hz",juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band3", "150Hz", juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band4", "250Hz", juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band5", "500Hz", juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band6", "1000Hz",juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band7", "2000Hz",juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band8", "4000Hz",juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band9", "8000Hz",juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("band10", "16000Hz", juce::NormalisableRange<float>(-12.f, 12.f, 0.5f, 1.f), 0.0f),
  };
}
const juce::String TenBandAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TenBandAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TenBandAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TenBandAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TenBandAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TenBandAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TenBandAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TenBandAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TenBandAudioProcessor::getProgramName (int index)
{
    return {};
}

void TenBandAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TenBandAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void TenBandAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TenBandAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TenBandAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
}

//==============================================================================
bool TenBandAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TenBandAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void TenBandAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TenBandAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TenBandAudioProcessor();
}
