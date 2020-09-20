/*
  ==============================================================================

    FourierProcessor.cpp
    Created: 18 Sep 2020 3:28:07pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "FourierProcessor.h"


FSynthProcessor::FSynthProcessor(int startingN, seriesType defaultType)
{
    audPartials = startingN;
    currentType = defaultType;
    for(int i = 0; i < maxPartials; ++i)
    {
        oscs.add(new Oscillator(0.0, 0.0));
    }
}

void FSynthProcessor::triangleFreqSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = (2.0f * i) + 1.0f;
        oscs[i]->newFreq(fundamental * fFactor * 2.0f);
    }
}

void FSynthProcessor::triangleAmpSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = (2.0f * i) + 1.0f;
        auto fNewAmp = (pow(-1.0f, i) / pow(fFactor, 2.0f));
        oscs[i]->newAmp(fNewAmp);
    }
}

void FSynthProcessor::squareFreqSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = (2.0f * i) + 1.0f;
        oscs[i]->newFreq(fundamental *  fFactor);
    }
}

void FSynthProcessor::squareAmpSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = (2.0f * i) + 1.0f;
        oscs[i]->newAmp(1.0f / fFactor);
    }
}

void FSynthProcessor::sawFreqSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = (1.0f + i);
        oscs[i]->newFreq(fundamental * fFactor);
    }
}

void FSynthProcessor::sawAmpSeries()
{
    for(int i = 0; i < oscs.size(); ++i)
    {
        auto fFactor = 2.0f * (i + 1.0f);
        oscs[i]->newAmp(1.0f / fFactor);
    }
}


double FSynthProcessor::getSample()
{
    updateAmps(currentType);
    updateFreqs(currentType);
    double sum = 0.0f;
    for(int i = 0; i < audPartials; ++i)
    {
        sum += oscs[i]->getOscSample();
    }
    auto postEnv = vEnv.adsr(sum, vEnv.trigger);
    return postEnv;
}
