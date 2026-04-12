#include "juce_audio_processors/juce_audio_processors.h"
#include "ChainSettings.h"
#include "Globals/GainRange.h"
#include "Globals/Band.h"

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts) {
  ChainSettings settings;

  settings.preGain       = apvts.getRawParameterValue(PreGainId)->load();
  settings.postGain      = apvts.getRawParameterValue(PostGainId)->load();
  settings.bandOneGain   = apvts.getRawParameterValue(bandOne.id)->load();
  settings.bandTwoGain   = apvts.getRawParameterValue(bandTwo.id)->load();
  settings.bandThreeGain = apvts.getRawParameterValue(bandThree.id)->load();
  settings.bandFourGain  = apvts.getRawParameterValue(bandFour.id)->load();
  settings.bandFiveGain  = apvts.getRawParameterValue(bandFive.id)->load();
  settings.bandSixGain   = apvts.getRawParameterValue(bandSix.id)->load();
  settings.bandSevenGain = apvts.getRawParameterValue(bandSeven.id)->load();
  settings.bandEightGain = apvts.getRawParameterValue(bandEight.id)->load();
  settings.bandNineGain  = apvts.getRawParameterValue(bandNine.id)->load();
  settings.bandTenGain   = apvts.getRawParameterValue(bandTen.id)->load();

  return settings;
}
