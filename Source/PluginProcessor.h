/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "ChainSettings.h"

//==============================================================================
/**
*/
class TenBandAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TenBandAudioProcessor();
    ~TenBandAudioProcessor() override;

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

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", createParameterLayout()};

    void updateSettings();
    void updateGain(ChainSettings& chainSettings);
    void updateCoefficients(ChainSettings& chainSettings);

private:
    enum EQChainPositions {
      BandOne,
      BandTwo,
      BandThree,
      BandFour,
      BandFive,
      BandSix,
      BandSeven,
      BandEight,
      BandNine,
      BandTen,
    };

    enum SignalChainPositions {
      PreGain,
      EQ,
      PostGain
    };

    using BandFilter = juce::dsp::IIR::Filter<float>;

    using Gain = juce::dsp::Gain<float>;

    using EQChain = juce::dsp::ProcessorChain<BandFilter, BandFilter, BandFilter, BandFilter, BandFilter, BandFilter, BandFilter, BandFilter, BandFilter, BandFilter>;

    using MonoChain = juce::dsp::ProcessorChain<Gain, EQChain, Gain>;

    MonoChain mLeftChain, mRightChain;

    double mLastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TenBandAudioProcessor)
};
