#pragma once

#include "VerticalSlider.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "../Globals/Band.h"
#include "../PluginProcessor.h"
class EQSliders : public juce::Component
{
  public:
    EQSliders(TenBandAudioProcessor& p) :
      bandOneSliderAttachment(p.apvts, bandOne.id, bandOneSlider),
      bandTwoSliderAttachment(p.apvts, bandTwo.id, bandTwoSlider),
      bandThreeSliderAttachment(p.apvts, bandThree.id, bandThreeSlider),
      bandFourSliderAttachment(p.apvts, bandFour.id, bandFourSlider),
      bandFiveSliderAttachment(p.apvts, bandFive.id, bandFiveSlider),
      bandSixSliderAttachment(p.apvts, bandSix.id, bandSixSlider),
      bandSevenSliderAttachment(p.apvts, bandSeven.id, bandSevenSlider),
      bandEightSliderAttachment(p.apvts, bandEight.id, bandEightSlider),
      bandNineSliderAttachment(p.apvts, bandNine.id, bandNineSlider),
      bandTenSliderAttachment(p.apvts, bandTen.id, bandTenSlider)
    {
      addAndMakeVisible(bandOneSlider);
      addAndMakeVisible(bandTwoSlider);
      addAndMakeVisible(bandThreeSlider);
      addAndMakeVisible(bandFourSlider);
      addAndMakeVisible(bandFiveSlider);
      addAndMakeVisible(bandSixSlider);
      addAndMakeVisible(bandSevenSlider);
      addAndMakeVisible(bandEightSlider);
      addAndMakeVisible(bandNineSlider);
      addAndMakeVisible(bandTenSlider);
    }

    void resized() override
    {
      auto bounds = getLocalBounds();
      const int sliderWidth = bounds.getWidth() / 10;

      bandOneSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandTwoSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandThreeSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandFourSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandFiveSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandSixSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandSevenSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandEightSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandNineSlider.setBounds(bounds.removeFromLeft(sliderWidth));
      bandTenSlider.setBounds(bounds);
    }

  private:
    VerticalSlider bandOneSlider;
    VerticalSlider bandTwoSlider;
    VerticalSlider bandThreeSlider;
    VerticalSlider bandFourSlider;
    VerticalSlider bandFiveSlider;
    VerticalSlider bandSixSlider;
    VerticalSlider bandSevenSlider;
    VerticalSlider bandEightSlider;
    VerticalSlider bandNineSlider;
    VerticalSlider bandTenSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment bandOneSliderAttachment,
               bandTwoSliderAttachment,
               bandThreeSliderAttachment,
               bandFourSliderAttachment,
               bandFiveSliderAttachment,
               bandSixSliderAttachment,
               bandSevenSliderAttachment,
               bandEightSliderAttachment,
               bandNineSliderAttachment,
               bandTenSliderAttachment;
};
