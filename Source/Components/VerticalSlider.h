#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

class VerticalSlider : public juce::Slider
{
  public:
    VerticalSlider() : juce::Slider(juce::Slider::SliderStyle::LinearBarVertical,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox)
    {}
};
