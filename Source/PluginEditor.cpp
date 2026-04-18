#include "Components/LookAndFeel/Palette.h"
#include "Components/LookAndFeel/PedalLnF.h"
#include "Globals/GainRange.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <array>
#include <utility>

namespace
{
    constexpr int kWindowWidth  = 700;
    constexpr int kWindowHeight = 420;

    constexpr int kOuterMarginX = 18;
    constexpr int kOuterMarginTop = 14;
    constexpr int kOuterMarginBottom = 22;

    constexpr int kHeaderHeight = 72;
    constexpr int kKnobColumnWidth = 84;
    constexpr int kKnobSize = 78;
    constexpr int kBodyGap = 8;
    constexpr int kLabelHeight = 20;
    constexpr int kEqSideInset = 24;

    constexpr float kScrewRadius = 6.5f;
    constexpr float kScrewInset  = 12.0f;
}

TenBandAudioProcessorEditor::TenBandAudioProcessorEditor (TenBandAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), eqSliders(p),
    preGainSliderAttachment(audioProcessor.apvts, PreGainId, preGainRotarySlider),
    postGainSliderAttachment(audioProcessor.apvts, PostGainId, postGainRotarySlider)
{
    for(auto* comp : getComponents())
    {
      addAndMakeVisible(comp);
    }
    setSize (kWindowWidth, kWindowHeight);
}

TenBandAudioProcessorEditor::~TenBandAudioProcessorEditor()
{
}

void TenBandAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    PedalLnF::drawChassis(g, bounds);
    paintCornerScrews(g, bounds);

    auto contentArea = bounds.reduced((float) kOuterMarginX,
                                      (float) kOuterMarginTop);
    contentArea.removeFromBottom((float) (kOuterMarginBottom - kOuterMarginTop));

    auto headerArea = contentArea.removeFromTop((float) kHeaderHeight);
    paintHeader(g, headerArea);

    paintKnobLabels(g);
}

void TenBandAudioProcessorEditor::paintHeader(juce::Graphics& g, juce::Rectangle<float> area)
{
    juce::Font headerFont{ juce::FontOptions{ PedalLnF::angelWishTypeface() }.withHeight(54.0f) };
    g.setFont(headerFont);
    g.setColour(Palette::silkscreen);
    g.drawText("Blacktooth 10", area, juce::Justification::centred, false);
}

void TenBandAudioProcessorEditor::paintKnobLabels(juce::Graphics& g)
{
    paintKnobAnnotations(g, preGainRotarySlider,  "GAIN");
    paintKnobAnnotations(g, postGainRotarySlider, "VOLUME");
}

void TenBandAudioProcessorEditor::paintKnobAnnotations(juce::Graphics& g,
                                                       juce::Slider& knobSlider,
                                                       const juce::String& mainLabel)
{
    const auto knob = knobSlider.getBounds().toFloat();
    const auto centre = knob.getCentre();
    const auto params = knobSlider.getRotaryParameters();
    const float startA = params.startAngleRadians;
    const float endA   = params.endAngleRadians;
    const float labelR = knob.getWidth() * 0.62f;

    struct Tick { float db; const char* text; };
    const std::array<Tick, 5> ticks = {{
        { -12.0f, "12" }, { -6.0f, "6" }, { 0.0f, "0" }, { 6.0f, "6" }, { 12.0f, "12" }
    }};

    for (const auto& t : ticks)
    {
        const float proportion = (t.db + 12.0f) / 24.0f;
        const float angle = startA + proportion * (endA - startA);
        const float sn = std::sin(angle);
        const float cs = -std::cos(angle);

        juce::Rectangle<float> area(centre.x + sn * labelR - 10.0f,
                                    centre.y + cs * labelR - 7.0f,
                                    20.0f, 14.0f);
        PedalLnF::drawSilkscreen(g, t.text, area,
                                 juce::Justification::centred,
                                 10.0f, false, Palette::silkscreenDim);
    }

    juce::Rectangle<float> mainArea((float) knob.getX() - 8.0f,
                                    (float) knob.getBottom() + 12.0f,
                                    (float) knob.getWidth() + 16.0f,
                                    14.0f);
    PedalLnF::drawSilkscreen(g, mainLabel, mainArea,
                             juce::Justification::centred,
                             12.0f, true, Palette::silkscreen);
}

void TenBandAudioProcessorEditor::paintCornerScrews(juce::Graphics& g, juce::Rectangle<float> chassis)
{
    const float d = kScrewRadius * 2.0f;
    const float inset = kScrewInset;

    const std::array<std::pair<juce::Point<float>, float>, 4> screws = {{
        { { chassis.getX() + inset,              chassis.getY() + inset              },  0.55f },
        { { chassis.getRight() - inset - d,      chassis.getY() + inset              }, -0.42f },
        { { chassis.getX() + inset,              chassis.getBottom() - inset - d     }, -0.65f },
        { { chassis.getRight() - inset - d,      chassis.getBottom() - inset - d     },  0.30f }
    }};

    for (const auto& s : screws)
        PedalLnF::drawScrew(g, { s.first.x, s.first.y, d, d }, s.second);
}

void TenBandAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromLeft(kOuterMarginX);
    bounds.removeFromRight(kOuterMarginX);
    bounds.removeFromTop(kOuterMarginTop);
    bounds.removeFromBottom(kOuterMarginBottom);

    bounds.removeFromTop(kHeaderHeight + kBodyGap);

    auto knobLabelReserve = kLabelHeight;

    auto preGainArea  = bounds.removeFromLeft(kKnobColumnWidth);
    auto postGainArea = bounds.removeFromRight(kKnobColumnWidth);
    auto eqArea       = bounds.reduced(kEqSideInset, 0);

    auto seatPreGain = preGainArea.withSizeKeepingCentre(kKnobSize, kKnobSize)
                                   .translated(0, -knobLabelReserve / 2);
    auto seatPostGain = postGainArea.withSizeKeepingCentre(kKnobSize, kKnobSize)
                                     .translated(0, -knobLabelReserve / 2);

    preGainRotarySlider.setBounds(seatPreGain);
    postGainRotarySlider.setBounds(seatPostGain);

    eqSliders.setBounds(eqArea);
}

std::vector<juce::Component*> TenBandAudioProcessorEditor::getComponents() {
  return
  {
    &preGainRotarySlider,
    &postGainRotarySlider,
    &eqSliders
  };
}
