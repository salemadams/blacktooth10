#pragma once

#include "LookAndFeel/Palette.h"
#include "LookAndFeel/PedalLnF.h"
#include "LookAndFeel/VerticalSliderLnF.h"
#include "VerticalSlider.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "../Globals/Band.h"
#include "../PluginProcessor.h"

#include <algorithm>
#include <array>
#include <cstdlib>

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
      for (auto* s : getSliders())
        addAndMakeVisible(s);
    }

    void paint(juce::Graphics& g) override
    {
      auto bounds = getLocalBounds().toFloat();

      auto labelRow = bounds.removeFromBottom((float) kLabelHeight);
      auto dbColumn = bounds.removeFromLeft((float) kDbColumnWidth);
      bounds.removeFromRight((float) kRightPad);

      paintDbScale(g, dbColumn);
      paintFrequencyLabels(g, labelRow);
    }

    void resized() override
    {
      auto bounds = getLocalBounds();
      bounds.removeFromBottom(kLabelHeight);
      bounds.removeFromLeft(kDbColumnWidth);
      bounds.removeFromRight(kRightPad);

      const int sliderWidth = bounds.getWidth() / 10;
      const int totalUsed = sliderWidth * 10;
      const int centringOffset = (bounds.getWidth() - totalUsed) / 2;
      bounds.removeFromLeft(centringOffset);

      for (auto* s : getSliders())
      {
        auto column = bounds.removeFromLeft(sliderWidth);
        s->setBounds(column.reduced(kSliderHorizInset, 0));
      }
    }

  private:
    static constexpr int kLabelHeight      = 18;
    static constexpr int kDbColumnWidth    = 20;
    static constexpr int kRightPad         = 4;
    static constexpr int kSliderHorizInset = 4;

    void paintDbScale(juce::Graphics& g, juce::Rectangle<float> column)
    {
      const float halfThumb   = VerticalSliderLnF::kThumbHeight * 0.5f;
      const float trackTop    = column.getY() + halfThumb;
      const float trackBottom = column.getBottom() - halfThumb;
      const float trackRange  = trackBottom - trackTop;

      const std::array<int, 13> dbValues = {
        12, 10, 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -12
      };
      const std::array<int, 3> annotatedMagnitudes = { 0, 6, 12 };

      for (int db : dbValues)
      {
        const float t = (12.0f - (float) db) / 24.0f;
        const float y = trackTop + t * trackRange;

        const bool isMajor = (db == 0) || std::abs(db) == 6 || std::abs(db) == 12;
        const float tickLen = isMajor ? 6.0f : 3.0f;

        g.setColour(Palette::silkscreenDim);
        g.fillRect(column.getRight() - tickLen, y - 0.5f, tickLen - 1.0f, 1.0f);

        const int absDb = std::abs(db);
        const bool annotate = std::any_of(annotatedMagnitudes.begin(),
                                          annotatedMagnitudes.end(),
                                          [absDb](int m){ return m == absDb; });
        if (annotate)
        {
          juce::String label = juce::String(absDb);
          PedalLnF::drawSilkscreen(g, label,
                                   { column.getX(), y - 7.0f,
                                     column.getWidth() - 7.0f, 14.0f },
                                   juce::Justification::centredRight,
                                   10.0f, false, Palette::silkscreenDim);
        }
      }
    }

    void paintFrequencyLabels(juce::Graphics& g, juce::Rectangle<float> row)
    {
      row.removeFromLeft((float) kDbColumnWidth);
      row.removeFromRight((float) kRightPad);

      static const std::array<const char*, 10> labels = {
          "31", "63", "125", "250", "500", "1k", "2k", "4k", "8k", "16k"
      };

      const int sliderWidth = (int) row.getWidth() / 10;
      const int totalUsed = sliderWidth * 10;
      const int centringOffset = ((int) row.getWidth() - totalUsed) / 2;
      row.removeFromLeft((float) centringOffset);

      for (size_t i = 0; i < labels.size(); ++i)
      {
        juce::Rectangle<float> cell(row.getX() + (float) sliderWidth * (float) i,
                                    row.getY(),
                                    (float) sliderWidth, row.getHeight());

        PedalLnF::drawSilkscreen(g, labels[i], cell,
                                 juce::Justification::centred,
                                 10.5f, true, Palette::silkscreen);
      }
    }

    std::array<VerticalSlider*, 10> getSliders()
    {
      return {
        &bandOneSlider, &bandTwoSlider, &bandThreeSlider, &bandFourSlider, &bandFiveSlider,
        &bandSixSlider, &bandSevenSlider, &bandEightSlider, &bandNineSlider, &bandTenSlider
      };
    }

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
