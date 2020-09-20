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
    FourierSynthVoice() : proc(10, FSynthProcessor::square)
    {
        
    }
    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        return dynamic_cast<FourierSound*>(sound) != nullptr;
    }
    //=========Parameter Setting Callbacks===================================================
    void setMAttack(std::atomic<float>* value)
    {
        proc.mEnv.setAttack(*value);
    }
    void setMDecay(std::atomic<float>* value)
    {
        proc.mEnv.setDecay(*value);
    }
    void setMSustain(std::atomic<float>* value)
    {
        proc.mEnv.setSustain(*value);
    }
    void setMRelease(std::atomic<float>* value)
    {
        proc.mEnv.setRelease(*value);
    }
    void setVAttack(std::atomic<float>* value)
    {
        proc.vEnv.setAttack(*value);
    }
    void setVDecay(std::atomic<float>* value)
    {
        proc.vEnv.setDecay(*value);
    }
    void setVSustain(std::atomic<float>* value)
    {
        proc.vEnv.setSustain(*value);
    }
    void setVRelease(std::atomic<float>* value)
    {
        proc.vEnv.setRelease(*value);
    }
    void setSeriesType(std::atomic<float>* value)
    {
        int choiceIndex = (int)*value;
        if(choiceIndex == 1)
            proc.currentType = FSynthProcessor::saw;
        else if(choiceIndex == 2)
            proc.currentType = FSynthProcessor::square;
        else if(choiceIndex == 3)
            proc.currentType = FSynthProcessor::triangle;
    }
    void setNumPartials(std::atomic<float>* value)
    {
        proc.numPartials = (int) *value;
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    juce::SynthesiserSound *sound,
                    int currentPitchWheelPosition)
    {
        proc.fundamental = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        proc.mEnv.trigger = 1;
        proc.vEnv.trigger = 1;
        proc.createOscSeries();
    }
    void stopNote (float velocity, bool allowTailOff)
    {
        proc.mEnv.trigger = 0;
        proc.vEnv.trigger = 0;
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
             double mixSample = proc.getSample();
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
    FSynthProcessor proc;
};

