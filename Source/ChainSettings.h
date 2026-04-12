#pragma once
#include "juce_audio_processors/juce_audio_processors.h"

struct ChainSettings {
  float preGain{0};
  float postGain{0};
  float bandOneGain{0};
  float bandTwoGain{0};
  float bandThreeGain{0};
  float bandFourGain{0};
  float bandFiveGain{0};
  float bandSixGain{0};
  float bandSevenGain{0};
  float bandEightGain{0};
  float bandNineGain{0};
  float bandTenGain{0};
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);
