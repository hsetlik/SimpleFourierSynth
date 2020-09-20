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
        newFreq(freq);
        newAmp(amp);
    }
    maxiOsc osc;
    float frequency = 0.0f;
    float amplitude = 0.0f;
    void newFreq(float freq)
    {
        frequency = freq;
    }
    void newAmp(float amp)
    {
        amplitude = amp;
    }
    double getOscSample()
    {
        double fullAmp = osc.sinewave(frequency);
        return fullAmp * amplitude;
    }
};

class FSynthProcessor
{
public:
    enum seriesType
        { //these have to be in the same order as the StringArray in the processor so the labels match
            square,
            triangle,
            saw
        };
    FSynthProcessor(int startingN, seriesType defaultType);
    ~FSynthProcessor(){}
    void setPitch(double newPitch)
    {
        fundamental = newPitch;
    }
    void setType(seriesType newType)
    {
        currentType = newType;
    }
    void setN(int newN)
    {
        audPartials = newN;
    }
    void updateFreqs(seriesType alg)
    {
        if(oscs[0]->frequency != fundamental)
        {
        switch(alg)
        {
            case square:
            {
                squareFreqSeries();
                break;
            }
            case triangle:
            {
                triangleFreqSeries();
                break;
            }
            case saw:
            {
                sawFreqSeries();
                break;
            }
        }
        }
    }
    void triangleFreqSeries();
    void sawFreqSeries();
    void squareFreqSeries();
    void updateAmps(seriesType alg)
    {
        if(oscs[0]->frequency != fundamental)
        {
        switch(alg)
        {
            case square:
            {
                squareAmpSeries();
                break;
            }
            case triangle:
            {
                triangleAmpSeries();
                break;
            }
            case saw:
            {
                sawAmpSeries();
                break;
            }
        }
        }
    }
    void triangleAmpSeries();
    void sawAmpSeries();
    void squareAmpSeries();
    double getSample();
    //data
    int audPartials;
    int maxPartials = 40;
    seriesType currentType;
    double fundamental;
    maxiEnv mEnv;
    maxiEnv vEnv;
    juce::OwnedArray<Oscillator> oscs;
    //Oscillator statOscs[40];
};
