/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Globals/Band.h"
#include "Globals/GainRange.h"

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
    std::make_unique<juce::AudioParameterFloat>(PreGainId, PreGainName, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandOne.id, bandOne.name, juce::NormalisableRange<float>(MinGain, MaxGain ,GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandTwo.id, bandTwo.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandThree.id, bandThree.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandFour.id, bandFour.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandFive.id, bandFive.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandSix.id, bandSix.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandSeven.id, bandSeven.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandEight.id, bandEight.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandNine.id, bandNine.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(bandTen.id, bandTen.name, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
    std::make_unique<juce::AudioParameterFloat>(PostGainId, PostGainName, juce::NormalisableRange<float>(MinGain, MaxGain, GainInterval, GainSkew), GainDefault),
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
  mLastSampleRate = sampleRate;

  juce::dsp::ProcessSpec spec;

  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = 1;

  mLeftChain.prepare(spec);
  mRightChain.prepare(spec);

  updateSettings();
}

void TenBandAudioProcessor::updateSettings() {
  auto chainSettings = getChainSettings(apvts);
  updateGain(chainSettings);
  updateCoefficients(chainSettings);
}

void TenBandAudioProcessor::updateGain(ChainSettings& chainSettings) {
  mRightChain.get<SignalChainPositions::PreGain>().setGainDecibels(chainSettings.preGain);
  mRightChain.get<SignalChainPositions::PostGain>().setGainDecibels(chainSettings.postGain);

  mLeftChain.get<SignalChainPositions::PreGain>().setGainDecibels(chainSettings.preGain);
  mLeftChain.get<SignalChainPositions::PostGain>().setGainDecibels(chainSettings.postGain);
}

void TenBandAudioProcessor::updateCoefficients(ChainSettings& chainSettings) {
  using Coefficients = juce::dsp::IIR::Coefficients<float>;
  using Decibels = juce::Decibels;

  auto bandOneCoeffs   = Coefficients::makePeakFilter(mLastSampleRate, bandOne.centreFreq,   bandOne.qFactor,   Decibels::decibelsToGain(chainSettings.bandOneGain));
  auto bandTwoCoeffs   = Coefficients::makePeakFilter(mLastSampleRate, bandTwo.centreFreq,   bandTwo.qFactor,   Decibels::decibelsToGain(chainSettings.bandTwoGain));
  auto bandThreeCoeffs = Coefficients::makePeakFilter(mLastSampleRate, bandThree.centreFreq, bandThree.qFactor, Decibels::decibelsToGain(chainSettings.bandThreeGain));
  auto bandFourCoeffs  = Coefficients::makePeakFilter(mLastSampleRate, bandFour.centreFreq,  bandFour.qFactor,  Decibels::decibelsToGain(chainSettings.bandFourGain));
  auto bandFiveCoeffs  = Coefficients::makePeakFilter(mLastSampleRate, bandFive.centreFreq,  bandFive.qFactor,  Decibels::decibelsToGain(chainSettings.bandFiveGain));
  auto bandSixCoeffs   = Coefficients::makePeakFilter(mLastSampleRate, bandSix.centreFreq,   bandSix.qFactor,   Decibels::decibelsToGain(chainSettings.bandSixGain));
  auto bandSevenCoeffs = Coefficients::makePeakFilter(mLastSampleRate, bandSeven.centreFreq, bandSeven.qFactor, Decibels::decibelsToGain(chainSettings.bandSevenGain));
  auto bandEightCoeffs = Coefficients::makePeakFilter(mLastSampleRate, bandEight.centreFreq, bandEight.qFactor, Decibels::decibelsToGain(chainSettings.bandEightGain));
  auto bandNineCoeffs  = Coefficients::makePeakFilter(mLastSampleRate, bandNine.centreFreq,  bandNine.qFactor,  Decibels::decibelsToGain(chainSettings.bandNineGain));
  auto bandTenCoeffs   = Coefficients::makePeakFilter(mLastSampleRate, bandTen.centreFreq,   bandTen.qFactor,   Decibels::decibelsToGain(chainSettings.bandTenGain));

  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandOne>().coefficients   = *bandOneCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandTwo>().coefficients   = *bandTwoCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandThree>().coefficients = *bandThreeCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandFour>().coefficients  = *bandFourCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandFive>().coefficients  = *bandFiveCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandSix>().coefficients   = *bandSixCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandSeven>().coefficients = *bandSevenCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandEight>().coefficients = *bandEightCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandNine>().coefficients  = *bandNineCoeffs;
  *mRightChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandTen>().coefficients   = *bandTenCoeffs;

  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandOne>().coefficients   = *bandOneCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandTwo>().coefficients   = *bandTwoCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandThree>().coefficients = *bandThreeCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandFour>().coefficients  = *bandFourCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandFive>().coefficients  = *bandFiveCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandSix>().coefficients   = *bandSixCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandSeven>().coefficients = *bandSevenCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandEight>().coefficients = *bandEightCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandNine>().coefficients  = *bandNineCoeffs;
  *mLeftChain.get<SignalChainPositions::EQ>().get<EQChainPositions::BandTen>().coefficients   = *bandTenCoeffs;
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateSettings();

    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    mLeftChain.process(leftContext);
    mRightChain.process(rightContext);
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
