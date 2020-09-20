/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleFourierSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::ComboBox::Listener
{
public:
    SimpleFourierSynthAudioProcessorEditor (SimpleFourierSynthAudioProcessor&);
    ~SimpleFourierSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* box) override;

private:
    SimpleFourierSynthAudioProcessor& audioProcessor;
    
    juce::Slider maSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> maSliderAttach;
    juce::Slider mdSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mdSliderAttach;
    juce::Slider msSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> msSliderAttach;
    juce::Slider mrSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mrSliderAttach;
    
    juce::Slider vaSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vaSliderAttach;
    juce::Slider vdSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vdSliderAttach;
    juce::Slider vsSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vsSliderAttach;
    juce::Slider vrSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vrSliderAttach;
    
    juce::ComboBox typeChoice;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeChoiceAttach;
    
    juce::Slider nSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> nSliderAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleFourierSynthAudioProcessorEditor)
};
