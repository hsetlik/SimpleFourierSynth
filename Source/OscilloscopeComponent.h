/*
  ==============================================================================

    OscilloscopeComponent.h
    Created: 20 Sep 2020 2:07:44pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Oscilloscope : public juce::AudioVisualiserComponent
{
public:
    Oscilloscope() : juce::AudioVisualiserComponent(1)
    {
        setBufferSize(64);
        setSamplesPerBlock(85);
        setColours(bkgndColor, wvColor);
    }
private:
    juce::Colour bkgndColor = juce::Colours::darkgrey;
    juce::Colour wvColor = juce::Colours::orange;
};


