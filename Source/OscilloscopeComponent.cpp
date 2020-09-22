/*
  ==============================================================================

    OscilloscopeComponent.cpp
    Created: 20 Sep 2020 2:07:44pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "OscilloscopeComponent.h"

ScopeComponent::ScopeComponent(ScopeSource* src)
{
    source = src;
    pointsPerBuffer = 65;
    startTimerHz(24);
}

void ScopeComponent::setFrameLength()
{
    int samplesPerBuffer = source->bufferSize;
    double sampleRate = source->sampleRate;
    currentFundamental = source->fundamental;
    samplesPerFrame = 8 * (sampleRate + 1) / ((currentFundamental) + 1);
    iBuffersPerFrame = (int)(samplesPerFrame / samplesPerBuffer);
    fBuffersPerFrame = (float)(samplesPerFrame / samplesPerBuffer);
    totalBuffersToLoad = iBuffersPerFrame + 2;
    if(frameReady == false)
        frameReady = true;
}

void ScopeComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::slategrey);
    if(frameReady)
    {
        buffersToDraw = source->outputBuffersThisFrame;
        source->storedBufferLimit = ceil(fBuffersPerFrame);
        std::vector<float> pointYValues;
        int indexInBuffer = 0;
        juce::AudioBuffer<float>* currentBuffer;
            if(buffersToDraw.size() >= iBuffersPerFrame)
            {
            for(int i = 0; i < floor(fBuffersPerFrame); ++i)
            {
                auto newBuffer = juce::AudioBuffer<float>(buffersToDraw[i]);
                currentBuffer = &newBuffer;
                indexInBuffer = 0;
                samplesPerPoint = (currentBuffer->getNumSamples() / pointsPerBuffer);
                
                for(int k = 0; k < currentBuffer->getNumSamples(); ++k)
                {
                    double sum = 0.0f;
                    for(int n = 0; n < samplesPerPoint; ++n)
                    {
                        sum += currentBuffer->getSample(0, k);
                        ++indexInBuffer;
                    }
                  
                    float pointValue = (sum / samplesPerPoint);
                    pointYValues.push_back(pointValue);
                }
            }
                //================================
                auto pointsPerFrame = iBuffersPerFrame * pointsPerBuffer;
                juce::Path trace;
                trace.preallocateSpace((4 * pointsPerFrame) + 8);
                
                
                g.setColour(juce::Colours::orange);
                juce::Rectangle<float> area = getLocalBounds().toFloat();
                float amplitude = 110;
                
                float xDelta = area.getWidth() / pointsPerFrame;
                float lastX = area.getX();
                float lastY = area.getHeight() / 2.0f;
                std::vector<float> pointsToRender;
                int startIndex = 0;
                bool indexFound = false;
                for(int i = 0; i < pointsPerFrame; ++i)
                {
                    if(indexFound == false)
                    {
                    if(fabs(pointYValues[i]) > 0.08)
                    {
                        startIndex = i;
                        indexFound = true;
                    }
                    }
                    if(indexFound)
                        break;
                }
                for(int i = 0; i < pointsPerFrame; ++i)
                {
                    pointsToRender.push_back(pointYValues[ i + startIndex]);
                }
                for(int i = 0; i < pointsPerFrame; ++i)
                {
                    float newX = xDelta * i ;
                    float newY = (pointsToRender[i] * amplitude) + (area.getHeight() / 2);
                    if(i == 0)
                    {
                        trace.startNewSubPath(newX, newY);
                    }
                    else
                    {
                        trace.lineTo(newX, newY);
                    }
                    lastX = newX;
                    lastY = newY;
                }
                trace.lineTo(area.getRight(), area.getHeight());
                trace.lineTo(area.getX(), area.getHeight());
                trace.closeSubPath();
                
                auto strokeType = juce::PathStrokeType(1.0f);
                
                g.strokePath(trace, strokeType);
        }
    }
}
