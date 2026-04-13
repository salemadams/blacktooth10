/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "Globals/GainRange.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TenBandAudioProcessorEditor::TenBandAudioProcessorEditor (TenBandAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), eqSliders(p),
    preGainSliderAttachment(audioProcessor.apvts, PreGainId, preGainRotarySlider),
    postGainSliderAttachment(audioProcessor.apvts, PostGainId, postGainRotarySlider)
{
    for(auto* comp : getComponents())
    {
      addAndMakeVisible(comp);
    }
    setSize (480, 480);
}

TenBandAudioProcessorEditor::~TenBandAudioProcessorEditor()
{
}

//==============================================================================
void TenBandAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //
    // g.setColour (juce::Colours::white);
    // g.setFont (juce::FontOptions (15.0f));
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TenBandAudioProcessorEditor::resized()
{
  auto bounds = getLocalBounds();
  const int totalHeight = bounds.getHeight();

  auto headerArea = bounds.removeFromTop(totalHeight * 0.33);
  auto footerArea = bounds.removeFromBottom(totalHeight * 0.10);
  auto bodyArea   = bounds;

  auto preGainArea  = bodyArea.removeFromLeft(60);
  auto postGainArea = bodyArea.removeFromRight(60);
  auto eqArea       = bodyArea;

  const int knobSize = std::min(preGainArea.getWidth(), 60);
  preGainRotarySlider.setBounds(preGainArea.withSizeKeepingCentre(knobSize, knobSize));
  postGainRotarySlider.setBounds(postGainArea.withSizeKeepingCentre(knobSize, knobSize));
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
