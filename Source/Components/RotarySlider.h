#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySlider : public juce::Slider
{
  public:
    RotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                  juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    {}
};
