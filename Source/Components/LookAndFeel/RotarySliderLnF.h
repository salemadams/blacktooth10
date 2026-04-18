#pragma once
#include "BinaryData.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderLnF : public juce::LookAndFeel_V4
{
  public:
    RotarySliderLnF()
    {
      filmstrip = juce::ImageCache::getFromMemory(BinaryData::MetallicKnobStrip_png,
                                                  BinaryData::MetallicKnobStrip_pngSize);
      frameWidth = filmstrip.getWidth();
      frameHeight = frameWidth;
      numFrames = filmstrip.getHeight() / frameHeight;
    }

    void drawRotarySlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos,
                          float /*rotaryStartAngle*/,
                          float /*rotaryEndAngle*/,
                          juce::Slider& /*slider*/) override
    {
      const auto knob = juce::Rectangle<float>((float) x, (float) y,
                                               (float) width, (float) height);

      const int frameIndex = juce::jlimit(0, numFrames - 1,
                                          static_cast<int>(sliderPos * (numFrames - 1)));
      const int sourceY = frameIndex * frameHeight;

      const auto drawArea = knob.expanded(knob.getWidth() * 0.18f);
      g.drawImage(filmstrip,
                  (int) drawArea.getX(), (int) drawArea.getY(),
                  (int) drawArea.getWidth(), (int) drawArea.getHeight(),
                  0, sourceY, frameWidth, frameHeight);
    }

  private:
    juce::Image filmstrip;
    int frameWidth{0};
    int frameHeight{0};
    int numFrames{0};
};
