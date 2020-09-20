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
class Oscillator
{
public:
    Oscillator(float freq, float amp)
    {
        frequency = freq;
        amplitude = amp;
    }
    ~Oscillator() {}
    maxiOsc osc;
    float frequency;
    float amplitude;
    double getOscSample()
    {
        auto fullAmp = osc.sinewave(frequency);
        return fullAmp * amplitude;
    }
};

class FSynthProcessor
{
public:
    enum seriesType
        {
            saw,
            square,
            triangle
        };
    FSynthProcessor(int startingN, seriesType defaultType);
    ~FSynthProcessor(){}
    void setPitch(double newPitch)
    {
        fundamental = newPitch;
    }
    void createSquareOscs();
    void updateSquareOvertones();
    void createTriangleOscs();
    void updateTriangleOvertones();
    void createSawOscs();
    void updateSawOvertones();
    void createOscSeries();
    void updateOscSeries();
    void updateNumPartials();
    double getSample();
    //data
    seriesType currentType;
    int numPartials;
    double fundamental;
    maxiEnv mEnv;
    maxiEnv vEnv;
    std::vector<Oscillator> oscs;
};
