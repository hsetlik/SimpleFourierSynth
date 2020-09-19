/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleFourierSynthAudioProcessorEditor::SimpleFourierSynthAudioProcessorEditor (SimpleFourierSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    
    maSlider.setSliderStyle(juce::Slider::Rotary);
    maSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    maSlider.setRange(1.0f, 20000.0f);
    maSlider.setValue(20.0f);
    maSlider.addListener(this);
    maSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mAttack",
                                                                                  maSlider));
    
    mdSlider.setSliderStyle(juce::Slider::Rotary);
    mdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mdSlider.setRange(1.0f, 20000.0f);
    mdSlider.setValue(85.0f);
    mdSlider.addListener(this);
    mdSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mDecay",
                                                                                  maSlider));
    
    msSlider.setSliderStyle(juce::Slider::Rotary);
    msSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    msSlider.setRange(0.0f, 1.0f);
    msSlider.setValue(0.6f);
    msSlider.addListener(this);
    msSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mSustain",
                                                                                  maSlider));
    
    mrSlider.setSliderStyle(juce::Slider::Rotary);
    mrSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mrSlider.setRange(1.0f, 20000.0f);
    mrSlider.setValue(150.0f);
    mrSlider.addListener(this);
    mrSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mRelease",
                                                                                  maSlider));
    
    vaSlider.setSliderStyle(juce::Slider::Rotary);
    vaSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vaSlider.setRange(1.0f, 20000.0f);
    vaSlider.setValue(20.0f);
    vaSlider.addListener(this);
    vaSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vAttack",
                                                                                  maSlider));
    
    vdSlider.setSliderStyle(juce::Slider::Rotary);
    vdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vdSlider.setRange(1.0f, 20000.0f);
    vdSlider.setValue(85.0f);
    vdSlider.addListener(this);
    vdSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vDecay",
                                                                                  maSlider));
    
    vsSlider.setSliderStyle(juce::Slider::Rotary);
    vsSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vsSlider.setRange(0.0f, 1.0f);
    vsSlider.setValue(0.6f);
    vsSlider.addListener(this);
    vsSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vSustain",
                                                                                  maSlider));
    
    vrSlider.setSliderStyle(juce::Slider::Rotary);
    vrSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vrSlider.setRange(1.0f, 20000.0f);
    vrSlider.setValue(150.0f);
    vrSlider.addListener(this);
    vrSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vRelease",
                                                                                  maSlider));
    
    addAndMakeVisible(&typeChoice);
    typeChoice.addItem("Square Wave", 0);
    typeChoice.addItem("Triangle Wave", 1);
    typeChoice.addItem("Sawtooth Wave", 2);
    typeChoiceAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "typeChoice", typeChoice));
    
}

SimpleFourierSynthAudioProcessorEditor::~SimpleFourierSynthAudioProcessorEditor()
{
}

//==============================================================================
void SimpleFourierSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


}

void SimpleFourierSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SimpleFourierSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    
}

void SimpleFourierSynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    
}
