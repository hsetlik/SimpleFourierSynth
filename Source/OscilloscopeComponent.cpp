/*
  ==============================================================================

    OscilloscopeComponent.cpp
    Created: 20 Sep 2020 2:07:44pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "OscilloscopeComponent.h"

Oscilloscope::Oscilloscope(ScopeSource* src)
{
    source = src;
    startTimerHz(16);
    blocksPerFrameWidth = 250; //this gets set as a constant, should control resolution;
}

void Oscilloscope::scaleFrameToHz(float fundamental, double sampleRate)
{
    int totalSamplesInFrame = 150;
    if(fundamental != 0)
        totalSamplesInFrame = (int)(sampleRate / fundamental);
    printf("samples in frame: %d\n", totalSamplesInFrame);
    samplesPerBlock = totalSamplesInFrame / blocksPerFrameWidth;
    source->setSizes(samplesPerBlock, blocksPerFrameWidth);
}
void Oscilloscope::resized()
{
    
}
void Oscilloscope::timerCallback()
{
    repaint();
}
float Oscilloscope::getYPosFromLevel(float level, float aHeight)
{
    float yCenter = aHeight / 2.0f;
    float maxAmplitude = 3.0f * (aHeight / 8.0f); //reserves 1/8 of the window on top and bottom for empty space
    return yCenter + (level * maxAmplitude);
}

void Oscilloscope::paint(juce::Graphics& g)
{
    scaleFrameToHz(source->voiceFundamental, source->processorSampleRate);
    juce::Rectangle<float> area = getLocalBounds().toFloat();
    g.fillAll(bkgndColor);
    g.setColour(wvColor);
    std::deque<float> blockLevels = source->frameBlocks; //copy the source data into a new list
    //source->frameBlocks.clear();
    
    float lastX = area.getX();
    float lastY;
    if(blockLevels.size() != 0)
        lastY = getYPosFromLevel(blockLevels[0], area.getHeight());
    else
        lastY = 0.0f;
    auto blockPointSpacing = area.getWidth() / blocksPerFrameWidth;
    for(int i = 0; i <  blocksPerFrameWidth; ++i)
    {
        float newX = i * blockPointSpacing;
        float newY;
        if(blockLevels.size() != 0)
        {
            newY = getYPosFromLevel(blockLevels[i], area.getHeight());
            printf("Y: %f\n", newY);
        }
        else
        {
            newY = 0.0f;
            printf("no signal\n");
        }
        
        g.drawLine(lastX, lastY, newX, newY);
        lastX = newX;
        lastY = newY;
    }
}
