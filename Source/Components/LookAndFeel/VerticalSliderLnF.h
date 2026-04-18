#pragma once
#include "Palette.h"
#include "PedalLnF.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <cmath>

class VerticalSliderLnF : public juce::LookAndFeel_V4
{
  public:
    static constexpr float kThumbHeight = 14.0f;
    static constexpr float kThumbWidth  = 22.0f;

    int getSliderThumbRadius(juce::Slider&) override
    {
        return (int) std::ceil(kThumbHeight * 0.5f);
    }

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override
    {
        juce::Slider::SliderLayout layout;
        layout.textBoxBounds = {};
        const int halfThumb = (int) std::ceil(kThumbHeight * 0.5f);
        layout.sliderBounds = slider.getLocalBounds().reduced(0, halfThumb);
        return layout;
    }

    void drawLinearSlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos,
                          float /*minSliderPos*/,
                          float /*maxSliderPos*/,
                          juce::Slider::SliderStyle /*style*/,
                          juce::Slider& slider) override
    {
        const auto comp = slider.getLocalBounds().toFloat();
        const auto area = juce::Rectangle<float>((float) x, (float) y,
                                                 (float) width, (float) height);

        const float trackWidth = 9.0f;
        const float trackX = comp.getCentreX() - trackWidth * 0.5f;
        juce::Rectangle<float> track(trackX, comp.getY(),
                                     trackWidth, comp.getHeight());

        g.setColour(juce::Colour(0xff040404));
        g.fillRoundedRectangle(track, 2.5f);

        g.setColour(juce::Colour(0xbb000000));
        g.fillRect(track.getX() + 1.0f, track.getY(),
                   track.getWidth() - 2.0f, 1.5f);

        g.setColour(juce::Colour(0x26ffffff));
        g.fillRect(track.getX() + 1.0f, track.getBottom() - 1.0f,
                   track.getWidth() - 2.0f, 1.0f);

        g.setColour(juce::Colour(0x55ffffff));
        g.drawRoundedRectangle(track, 2.5f, 1.0f);

        const float centreY = area.getCentreY();
        g.setColour(Palette::slotCenterLine);
        g.fillRect(track.getX() - 3.0f, centreY - 0.5f, track.getWidth() + 6.0f, 1.0f);

        const float thumbX = comp.getCentreX() - kThumbWidth * 0.5f;
        const float thumbY = sliderPos - kThumbHeight * 0.5f;
        juce::Rectangle<float> thumb(thumbX, thumbY, kThumbWidth, kThumbHeight);

        g.setColour(juce::Colour(0x77000000));
        g.fillRoundedRectangle(thumb.translated(0.0f, 1.5f), 2.0f);

        juce::ColourGradient body(juce::Colour(0xff242424), 0.0f, thumb.getY(),
                                  juce::Colour(0xff080808), 0.0f, thumb.getBottom(),
                                  false);
        body.addColour(0.45, juce::Colour(0xff151515));
        g.setGradientFill(body);
        g.fillRoundedRectangle(thumb, 2.0f);

        g.setColour(juce::Colour(0x22ffffff));
        g.fillRect(thumb.getX() + 2.0f, thumb.getY() + 0.5f,
                   thumb.getWidth() - 4.0f, 1.0f);

        g.setColour(juce::Colour(0xff000000));
        g.drawRoundedRectangle(thumb, 2.0f, 1.0f);

        PedalLnF::drawLed(g, { thumb.getCentreX(), thumb.getCentreY() }, 2.2f);
    }
};
