/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleFourierSynthAudioProcessorEditor::SimpleFourierSynthAudioProcessorEditor(SimpleFourierSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor(p), scope(&audioProcessor.scopeSource)
{
    addAndMakeVisible(&scope);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    addAndMakeVisible(&maSlider);
    maSlider.setSliderStyle(juce::Slider::Rotary);
    maSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    maSlider.setRange(1.0f, 20000.0f);
    maSlider.setValue(20.0f);
    maSlider.addListener(this);
    maSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mAttack",
                                                                                  maSlider));
    addAndMakeVisible(&mdSlider);
    mdSlider.setSliderStyle(juce::Slider::Rotary);
    mdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mdSlider.setRange(1.0f, 20000.0f);
    mdSlider.setValue(85.0f);
    mdSlider.addListener(this);
    mdSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mDecay",
                                                                                  mdSlider));
    addAndMakeVisible(&msSlider);
    msSlider.setSliderStyle(juce::Slider::Rotary);
    msSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    msSlider.setRange(0.0f, 1.0f);
    msSlider.setValue(0.6f);
    msSlider.addListener(this);
    msSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mSustain",
                                                                                  msSlider));
    addAndMakeVisible(&mrSlider);
    mrSlider.setSliderStyle(juce::Slider::Rotary);
    mrSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mrSlider.setRange(1.0f, 20000.0f);
    mrSlider.setValue(150.0f);
    mrSlider.addListener(this);
    mrSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "mRelease",
                                                                                  mrSlider));
    addAndMakeVisible(&vaSlider);
    vaSlider.setSliderStyle(juce::Slider::Rotary);
    vaSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vaSlider.setRange(1.0f, 20000.0f);
    vaSlider.setValue(20.0f);
    vaSlider.addListener(this);
    vaSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vAttack",
                                                                                  vaSlider));
    addAndMakeVisible(&vdSlider);
    vdSlider.setSliderStyle(juce::Slider::Rotary);
    vdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vdSlider.setRange(1.0f, 20000.0f);
    vdSlider.setValue(85.0f);
    vdSlider.addListener(this);
    vdSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vDecay",
                                                                                  vdSlider));
    addAndMakeVisible(&vsSlider);
    vsSlider.setSliderStyle(juce::Slider::Rotary);
    vsSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vsSlider.setRange(0.0f, 1.0f);
    vsSlider.setValue(0.6f);
    vsSlider.addListener(this);
    vsSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vSustain",
                                                                                  vsSlider));
    addAndMakeVisible(&vrSlider);
    vrSlider.setSliderStyle(juce::Slider::Rotary);
    vrSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    vrSlider.setRange(1.0f, 20000.0f);
    vrSlider.setValue(150.0f);
    vrSlider.addListener(this);
    vrSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "vRelease",
                                                                                  vrSlider));
    addAndMakeVisible(&nSlider);
    nSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    nSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    nSlider.setRange(1.0f, 100.0f);
    nSlider.setValue(10.0f);
    nSlider.addListener(this);
    nSliderAttach.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "partialCount", nSlider));
    
    
    addAndMakeVisible(&typeChoice);
    typeChoice.addItem("Square Wave", 1);
    typeChoice.addItem("Triangle Wave", 2);
    typeChoice.addItem("Sawtooth Wave", 3);
    typeChoiceAttach.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "typeChoice", typeChoice));
    
}

SimpleFourierSynthAudioProcessorEditor::~SimpleFourierSynthAudioProcessorEditor()
{
}

//==============================================================================
void SimpleFourierSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //get data from the processor to the scope here
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


}

void SimpleFourierSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int n = getWidth() / 24;
    maSlider.setBounds(3 * n, n, 3 * n, 3 * n);
    mdSlider.setBounds(8 * n, n, 3 * n, 3 * n);
    msSlider.setBounds(13 * n, n, 3 * n, 3 * n);
    mrSlider.setBounds(18 * n, n, 3 * n, 3 * n);
    
    scope.setBounds(16 * n, 10 * n, 7 * n, 7 * n);
    
    vaSlider.setBounds(3 * n, 6 * n, 3 * n, 3 * n);
    vdSlider.setBounds(8 * n, 6 * n, 3 * n, 3 * n);
    vsSlider.setBounds(13 * n, 6 * n, 3 * n, 3 * n);
    vrSlider.setBounds(18 * n, 6 * n, 3 * n, 3 * n);
    
    nSlider.setBounds(3 * n, 11 * n, 10 * n, 2 * n);
    
    typeChoice.setBounds(3 * n, 14 * n, 6 * n, 2 * n);
    
}

void SimpleFourierSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    
}

void SimpleFourierSynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    
}

