/*
  ==============================================================================

    OscilloscopeComponent.h
    Created: 20 Sep 2020 2:07:44pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ScopeSource
{
public:
    ScopeSource()
    {
        
    }
    ~ScopeSource() {}
    void addSampleToBlock(float newSample)
    {
        blockSamples.push_back(newSample);
        if(blockSamples.size() >= samplesPerBlock)
        {
            addBlockToFrame();
            blockSamples.clear();
        }
    }
    void addBlockToFrame()
    {
        double sum = 0.0f;
        for(auto i : blockSamples)
        {
            sum += blockSamples[i];
        }
        float blockToAdd = (sum / blockSamples.size());
        workingFrameBlocks.push_back(blockToAdd);
        if(workingFrameBlocks.size() == blocksPerFrameWidth)
        {
            workingFrameBlocks.pop_front();
            frameBlocks = workingFrameBlocks;
        }
    }
    void setSizes(int spb, int bpf)
    {
        samplesPerBlock = spb;
        blocksPerFrameWidth = bpf;
    }
    std::deque<float> workingFrameBlocks;
    std::deque<float> frameBlocks;
    double voiceFundamental;
    double processorSampleRate;
private:
    std::vector<float> blockSamples;
    int samplesPerBlock;
    int blocksPerFrameWidth;
    
};


class Oscilloscope : public juce::Timer, public juce::Component
{
public:
    Oscilloscope(ScopeSource* src);
    ~Oscilloscope() {};
    void scaleFrameToHz(float fundamental, double sampleRate);
    void resized() override;
    void timerCallback() override;
    void paint(juce::Graphics& g) override;
    float getYPosFromLevel(float level, float aHeight);
private:
    int samplesPerBlock;
    int blocksPerFrameWidth;
    ScopeSource* source;
    juce::Colour bkgndColor = juce::Colours::darkgrey;
    juce::Colour wvColor = juce::Colours::orange;
};


