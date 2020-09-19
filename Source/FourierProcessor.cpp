/*
  ==============================================================================

    FourierProcessor.cpp
    Created: 18 Sep 2020 3:28:07pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "FourierProcessor.h"


Series::Series(int startingNumPartials, seriesType type)
{
    numPartials = startingNumPartials;
    currentType = type;
}


void Series::createSawPartials()
{
    currentPartials.clear();
    Partial* fundPartial = new Partial(1, fundamental, 1.0);
    currentPartials.push_back(*fundPartial);
    for(int i = 2; i < numPartials; ++i)
    {
        Partial* newPartial = new Partial(i, fundamental * i, 1.0 / i);
        currentPartials.push_back(*newPartial);
    }
}

void Series::createSquarePartials()
{
    currentPartials.clear();
    Partial* fundPartial = new Partial(1, fundamental, 1.0);
    currentPartials.push_back(*fundPartial);
    int squarePartials = numPartials * 2;
    for(int i = 2; i < squarePartials; ++i)
    {
        if(i % 2 != 0)
        {
        Partial* newPartial = new Partial(i, fundamental * i, 1.0 / i);
        currentPartials.push_back(*newPartial);
        }
    }
}

void Series::createTrianglePartials()
{
    currentPartials.clear();
    for(int i = 0; i < numPartials; ++i)
    {
        float mode = (2 * i) + 1;
        float newAmplitude = pow(-1, i) * pow(mode, 2);
        float newFrequency = fundamental * mode;
        Partial* newPartial = new Partial(i, newFrequency, newAmplitude);
        currentPartials.push_back(*newPartial);
    }
}

double Series::getSampleFromSeries()
{
    double newSample = 0.0f;
    for(int i = 0; i < numPartials; ++i)
    {
        newSample += currentPartials[i].getSample();
    }
    return newSample;
}
