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
struct Oscillator
{
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
enum seriesType
    {
        saw,
        square,
        triangle
    };
public:
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
    double getSample();
    //data
    seriesType currentType;
    int numPartials;
    double fundamental;
    maxiEnv mEnv;
    maxiEnv vEnv;
    std::vector<Oscillator> oscs;
};
