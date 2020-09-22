


#pragma once
#include <JuceHeader.h>

class ScopeSource
{
public:
    //functions
    ScopeSource()
    {
    }
    ~ScopeSource()
    {
        outputBuffersThisFrame.clear();
    }
    void add(juce::AudioBuffer<float>& buffer)
    {
        auto newBuffer = buffer;
        bufferSize = buffer.getNumSamples();
        outputBuffersThisFrame.push_back(newBuffer);
    }
    //data
    double sampleRate;
    int bufferSize;
    float fundamental;
    std::deque<juce::AudioBuffer<float>> outputBuffersThisFrame; //remember to clear this at the end of each frame
    
};

class ScopeComponent : public juce::Component, public juce::Timer
{
public:
    //functions
    ScopeComponent(ScopeSource* src);
    ~ScopeComponent()
    {
        buffersToDraw.clear();
    }
    void paint(juce::Graphics& g) override;
    void setFrameLength();
    void timerCallback() override
    {
        setFrameLength();
        repaint();
    }
    //data
    bool frameReady = false;
    int pointsPerBuffer; //this can be set as a constant to control resolution
    int iBuffersPerFrame; //this should be adjusted to the frequency
    float fBuffersPerFrame;
    int totalBuffersToLoad;
    float currentFundamental;
    double samplesPerFrame;
    double samplesPerPoint;
    
    ScopeSource* source;
    std::deque<juce::AudioBuffer<float>> buffersToDraw;
};
