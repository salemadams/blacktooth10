#pragma once

#include "Components/EQSliders.h"
#include "Components/RotarySlider.h"
#include "PluginProcessor.h"

class TenBandAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TenBandAudioProcessorEditor (TenBandAudioProcessor&);
    ~TenBandAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TenBandAudioProcessor& audioProcessor;

    RotarySlider preGainRotarySlider;
    RotarySlider postGainRotarySlider;

    EQSliders eqSliders;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment preGainSliderAttachment,
               postGainSliderAttachment;

    std::vector<juce::Component*> getComponents();

    void paintHeader(juce::Graphics& g, juce::Rectangle<float> area);
    void paintKnobLabels(juce::Graphics& g);
    void paintKnobAnnotations(juce::Graphics& g, juce::Slider& knobSlider,
                              const juce::String& mainLabel);
    void paintCornerScrews(juce::Graphics& g, juce::Rectangle<float> chassis);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TenBandAudioProcessorEditor)
};
