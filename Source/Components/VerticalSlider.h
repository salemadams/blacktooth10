#pragma once
#include "LookAndFeel/VerticalSliderLnF.h"
#include "juce_gui_basics/juce_gui_basics.h"

class VerticalSlider : public juce::Slider
{
  public:
    VerticalSlider() : juce::Slider(juce::Slider::SliderStyle::LinearVertical,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
      setLookAndFeel(&lnf);
    }

    ~VerticalSlider()
    {
      setLookAndFeel(nullptr);
    }

  private:
    VerticalSliderLnF lnf;
};
