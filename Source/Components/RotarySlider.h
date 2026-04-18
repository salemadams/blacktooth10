#pragma once
#include "LookAndFeel/RotarySliderLnF.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySlider : public juce::Slider
{
  public:
    RotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                  juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
      setLookAndFeel(&lnf);
    }

    ~RotarySlider()
    {
      setLookAndFeel(nullptr);
    }

  private:
    RotarySliderLnF lnf;
};
