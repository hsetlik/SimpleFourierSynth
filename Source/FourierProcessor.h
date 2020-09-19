/*
  ==============================================================================

    FourierProcessor.h
    Created: 18 Sep 2020 3:28:07pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "maximilian.h"

class Partial
{
public:
    Partial(int ind, float startFreq, float startAmplitude)
    {
        pIndex = ind;
        frequency = startFreq;
        amplitude = startAmplitude;
    }
    ~Partial() {}
    void setFrequency(float newFrequency)
    {
        frequency = newFrequency;
    }
    void setAmplitude(float newAmplitude)
    {
        amplitude = newAmplitude;
    }
    float getSample()
    {
        float oscSample = oscillator.sinewave(frequency);
        return oscSample * amplitude;
    }
    int pIndex;
    float frequency;
    float amplitude;
    maxiOsc oscillator;
    juce::String idString;
};

class Series
{
public:
    enum seriesType
    {
        square,
        triangle,
        saw
    };
    //functions
    Series(int startingNumPartials, seriesType type);
    ~Series() {}
    void createSquarePartials();
    void createSawPartials();
    void createTrianglePartials();
    void setPitch(double pitch)
    {
        fundamental = pitch;
    }
    void setNumPartials(int newN)
    {
        numPartials = newN;
    }
    double getSampleFromSeries();
    //data
    float fundamental;
    seriesType currentType;
    maxiEnv modEnv;
    maxiEnv volEnv;
    int numPartials;
    std::vector<Partial> currentPartials;
    
};
