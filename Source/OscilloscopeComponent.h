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
        setBufferSize(45);
        setSamplesPerBlock(100);
        setColours(bkgndColor, wvColor);
    }
    void getChannelAsLine (juce::Path& path, const juce::Range<float>* levels,
                           int numLevels, int nextSample)
    {
        path.preallocateSpace (4 * numLevels + 8);

        for (int i = 0; i < numLevels; ++i)
        {
            auto level = -(levels[(nextSample + i) % numLevels].getEnd());

            if (i == 0)
                path.startNewSubPath (0.0f, level);
            else
                path.lineTo ((float) i, level);
        }
        path.lineTo((float)(numLevels), (float)getWidth());
        /*
        for (int i = numLevels; --i >= 0;)
            path.lineTo ((float) i, -(levels[(nextSample + i) % numLevels].getStart()));
         */
        path.closeSubPath();
    }
    void paintChannel (juce::Graphics& g, juce::Rectangle<float> area, const juce::Range<float>* levels, int numLevels, int nextSample) override
    {
        juce::Path p;
        getChannelAsLine(p, levels, numLevels, nextSample);
        juce::PathStrokeType strokeType = juce::PathStrokeType(1.0f);

        g.strokePath(p, strokeType, juce::AffineTransform::fromTargetPoints (0.0f, -1.0f,               area.getX(), area.getY(),
                                                          0.0f, 1.0f,                area.getX(), area.getBottom(),
                                                          (float) numLevels, -1.0f,  area.getRight(), area.getY()));
    }
                    

private:
    juce::Colour bkgndColor = juce::Colours::darkgrey;
    juce::Colour wvColor = juce::Colours::orange;
};


