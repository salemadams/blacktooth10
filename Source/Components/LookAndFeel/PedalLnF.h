#pragma once
#include "BinaryData.h"
#include "Palette.h"
#include "juce_graphics/juce_graphics.h"

namespace PedalLnF
{
    inline juce::Typeface::Ptr angelWishTypeface()
    {
        static juce::Typeface::Ptr tf =
            juce::Typeface::createSystemTypefaceFor(BinaryData::AngelWish_ttf,
                                                    BinaryData::AngelWish_ttfSize);
        return tf;
    }

    inline void drawChassis(juce::Graphics& g, juce::Rectangle<float> bounds)
    {
        juce::ColourGradient body(Palette::chassisTop,    bounds.getX(), bounds.getY(),
                                  Palette::chassisBottom, bounds.getX(), bounds.getBottom(),
                                  false);
        body.addColour(0.55, Palette::chassisMid);
        g.setGradientFill(body);
        g.fillRect(bounds);
    }

    inline void drawScrew(juce::Graphics& g, juce::Rectangle<float> area, float rotationRadians)
    {
        const auto centre = area.getCentre();
        const float r = area.getWidth() * 0.5f;

        g.setColour(juce::Colour(0x55000000));
        g.fillEllipse(area.translated(0.0f, 1.5f));

        juce::ColourGradient body(Palette::screwLight, centre.x - r * 0.6f, centre.y - r * 0.6f,
                                  Palette::screwDark,  centre.x + r,        centre.y + r,
                                  true);
        body.addColour(0.45, Palette::screwMid);
        g.setGradientFill(body);
        g.fillEllipse(area);

        g.setColour(juce::Colour(0xff2a2a2a));
        g.drawEllipse(area.reduced(0.5f), 1.0f);

        juce::Path slot;
        const float slotLen = r * 1.4f;
        const float slotThk = juce::jmax(1.0f, r * 0.22f);
        slot.addRoundedRectangle(-slotLen * 0.5f, -slotThk * 0.5f, slotLen, slotThk, slotThk * 0.3f);

        auto transform = juce::AffineTransform::rotation(rotationRadians).translated(centre.x, centre.y);
        g.setColour(Palette::screwDark);
        g.fillPath(slot, transform);

        g.setColour(juce::Colour(0x66ffffff));
        g.fillEllipse(centre.x - r * 0.35f, centre.y - r * 0.55f, r * 0.45f, r * 0.25f);
    }

    inline void drawLed(juce::Graphics& g, juce::Point<float> centre, float radius)
    {
        g.setColour(Palette::ledRedGlow);
        g.fillEllipse(centre.x - radius * 2.2f, centre.y - radius * 2.2f,
                      radius * 4.4f, radius * 4.4f);

        juce::ColourGradient lens(Palette::ledRedCore, centre.x - radius * 0.3f, centre.y - radius * 0.3f,
                                  Palette::ledRed,    centre.x + radius,        centre.y + radius,
                                  true);
        g.setGradientFill(lens);
        g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

        g.setColour(juce::Colours::white.withAlpha(0.85f));
        g.fillEllipse(centre.x - radius * 0.45f, centre.y - radius * 0.6f,
                      radius * 0.5f, radius * 0.35f);
    }

    inline juce::Font silkscreenFont(float height, bool bold = false)
    {
        auto options = juce::FontOptions(height);
        if (bold)
            options = options.withStyle("Bold");
        juce::Font f{ options };
        f.setExtraKerningFactor(0.08f);
        return f;
    }

    inline void drawSilkscreen(juce::Graphics& g, const juce::String& text,
                               juce::Rectangle<float> area, juce::Justification just,
                               float height, bool bold = false,
                               juce::Colour colour = Palette::silkscreen)
    {
        g.setFont(silkscreenFont(height, bold));
        g.setColour(colour);
        g.drawText(text, area, just, false);
    }
}
