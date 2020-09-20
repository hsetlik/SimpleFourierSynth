/*
  ==============================================================================

    FourierSynthVoice.h
    Created: 18 Sep 2020 11:33:30am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FourierProcessor.h"



class FourierSound : public juce::SynthesiserSound
{
    public:
    bool appliesToNote(int /*midiNoteNumber*/) //just plays this sound for any midi note
    {
        return true;
    }
    bool appliesToChannel(int /*midiChannel*/) //plays the sound on both channels
    {
        return true;
    }
};

class FourierSynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        return dynamic_cast<FourierSound*>(sound) != nullptr;
    }
    //=========Parameter Setting Callbacks===================================================
    void setMAttack(std::atomic<float>* value)
    {
        
    }
    void setMDecay(std::atomic<float>* value)
    {
        
    }
    void setMSustain(std::atomic<float>* value)
    {
    }
    void setMRelease(std::atomic<float>* value)
    {
    }
    void setVAttack(std::atomic<float>* value)
    {
    }
    void setVDecay(std::atomic<float>* value)
    {
    }
    void setVSustain(std::atomic<float>* value)
    {
       
    }
    void setVRelease(std::atomic<float>* value)
    {
       
    }
    void setSeriesType(std::atomic<float>* value)
    {
        
    }
    void setNumPartials(std::atomic<float>* value)
    {
    
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    juce::SynthesiserSound *sound,
                    int currentPitchWheelPosition)
    {
        
    }
    void stopNote (float velocity, bool allowTailOff)
    {
        
        allowTailOff = true;
        if(velocity == 0)
            clearCurrentNote();
    }
    //===========================================
    void pitchWheelMoved(int newPitchWheelVal)
    {
        
    }
    //=============================================
    void controllerMoved(int controllerNumber, int controllerValue)
    {
        
    }
    //===============================================
    void aftertouchChanged (int newAftertouchValue)
    {

    }
    //==============================================
    void channelPressureChanged (int newChannelPressureValue)
    {
        
    }
    //===============================================
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
    {
        for(int sample = 0; sample < numSamples; ++sample) //calculate all the samples for this block
         {
             double mixSample;
             if(mixSample != 0)
                 printf("actual sound\n");
             for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
             {
                 outputBuffer.addSample(channel, startSample, mixSample);
             }
         ++startSample;
         }
       
    }
    //==============================================
    void setCurrentPlaybackSampleRate (double newRate)
    {
        
    }
};

