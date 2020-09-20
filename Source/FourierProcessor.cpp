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
    numPartials = startingN;
    currentType = defaultType;
    
}


void FSynthProcessor::createSquareOscs()
{
    oscs.clear();
    for(int i = 0; i < 100; ++i)
    {
        auto fFactor = (2 * i) + 1;
        auto frequency = fundamental * fFactor;
        auto amplitude = (float)(1 / fFactor);
        Oscillator* newOsc = new Oscillator(frequency, amplitude);
        oscs.push_back(*newOsc);
    }
}
void FSynthProcessor::updateSquareOvertones()
{
    auto oscCount = oscs.size();
    for(int i = 0; i < oscCount; ++i)
    {
        auto fFactor = (2 * i) + 1;
        oscs[i].frequency = (fundamental * fFactor);
        oscs[i].amplitude = (float)(1.0f / fFactor);
    }
    
}
void FSynthProcessor::createTriangleOscs()
{
    oscs.clear();
    for(int i = 0; i < 100; ++i)
    {
        auto fFactor = (2 * i) + 1;
        auto frequency = fFactor * fundamental;
        auto amplitude = pow(-1.0f, i) / pow(fFactor, 2.0f);
        Oscillator* newOsc = new Oscillator(frequency, amplitude);
        oscs.push_back(*newOsc);
    }
    
}
void FSynthProcessor::updateTriangleOvertones()
{
    auto oscCount = oscs.size();
    for(int i = 0; i < oscCount; ++i)
    {
        auto fFactor = (2 * i) + 1;
        oscs[i].frequency = fundamental * fFactor;
        oscs[i].amplitude = pow(-1 , i) * pow(fFactor, 2);
    }
}
void FSynthProcessor::createSawOscs()
{
    oscs.clear();
    for(int i = 0; i < 100; ++i)
    {
        auto frequency = i * fundamental;
        auto amplitude = 1 / ((2 * i) + 1);
        Oscillator* newOsc = new Oscillator(frequency, amplitude);
        oscs.push_back(*newOsc);
    }
    
}
void FSynthProcessor::updateSawOvertones()
{
    auto oscCount = oscs.size();
    for(int i = 0; i < oscCount; ++i)
    {
        oscs[i].frequency = i * fundamental;
        oscs[i].amplitude = 1 / ((2 * i) + 1);
    }
    
}
void FSynthProcessor::createOscSeries()
{
    if(oscs.size() < 1) //call this for each sample but only create a new series if there are no oscillators yet
    {
        switch(currentType)
        {
            case saw:
            {
                createSawOscs();
                break;
            }
            case square:
            {
                createSquareOscs();
                break;
            }
            case triangle:
            {
                createTriangleOscs();
                break;
            }
        }
    }
}

void FSynthProcessor::updateOscSeries()
{
    if(oscs.size() != 0)
    {
        switch(currentType)
        {
            case square:
            {
                updateSquareOvertones();
                break;
            }
            case saw:
            {
                updateSawOvertones();
                break;
            }
            case triangle:
            {
                updateTriangleOvertones();
                break;
            }
        }
    }
}
double FSynthProcessor::getSample()
{
    double output = 0.0f;
    createOscSeries();
    updateOscSeries();
    for(int i = 0; i < numPartials; ++i)
    {
        double newVal = oscs[i].getOscSample();
        output += newVal;
    }
    double postEnv = vEnv.adsr(output, vEnv.trigger);
    return postEnv;
}
