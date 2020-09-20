/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::String maId = "mAttack";
    juce::String maName = "Modulation Attack";
    juce::String mdId = "mDecay";
    juce::String mdName = "Modulation Decay";
    juce::String msId = "mSustain";
    juce::String msName = "Modulation Sustain";
    juce::String mrId = "mRelease";
    juce::String mrName = "Modulation Release";
    
    juce::String vaId = "vAttack";
    juce::String vaName = "Volume Attack";
    juce::String vdId = "vDecay";
    juce::String vdName = "Volume Decay";
    juce::String vsId = "vSustain";
    juce::String vsName = "Volume Sustain";
    juce::String vrId = "vRelease";
    juce::String vrName = "Volume Release";
    
    juce::String tcId = "typeChoice";
    juce::String tcName = "Series Type";
    
    juce::StringArray stArray;
    stArray.add("Square Wave");
    stArray.add("Triangle Wave");
    stArray.add("Saw Wave");
    
    juce::String nCountId = "partialCount";
    juce::String nCountName = "Harmonic Number";
    
    juce::NormalisableRange<float> maRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(maId, maName, maRange, 20.0));
    juce::NormalisableRange<float> mdRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(mdId, mdName, mdRange, 85.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(msId, msName, 0.0, 1.0, 0.6));
    juce::NormalisableRange<float> mrRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(mrId, mrName, mrRange, 150.0));
    
    juce::NormalisableRange<float> vaRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(vaId, vaName, vaRange, 20.0));
    juce::NormalisableRange<float> vdRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(vdId, vdName, vdRange, 85.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(vsId, vsName, 0.0, 1.0, 0.6));
    juce::NormalisableRange<float> vsRange(1.0f, 20000.0f, 0.1f, 0.25f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(vrId, vrName, vsRange, 150.0));
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(tcId, tcName, stArray, 1));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(nCountId, nCountName, 1.0, 40.0, 10.0));
    
    return layout;
}

//==============================================================================
SimpleFourierSynthAudioProcessor::SimpleFourierSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr, "ALLPARAMETERS", createLayout())
#endif
{
    //six voice polyphony
    for(int i = 0; i < 6; ++i)
    {
        synth.addVoice(new FourierSynthVoice());
    }
    synth.clearSounds();
    synth.addSound(new FourierSound());
}

SimpleFourierSynthAudioProcessor::~SimpleFourierSynthAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleFourierSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleFourierSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleFourierSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleFourierSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleFourierSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleFourierSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleFourierSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleFourierSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleFourierSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleFourierSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleFourierSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SimpleFourierSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleFourierSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void SimpleFourierSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if(scopeSetup)
    {
        editorScope->setBufferSize(24);
        editorScope->setSamplesPerBlock(11);
    }
    for(int i = 0; i < synth.getNumVoices(); ++i)
       {
           //yes that is supposed to be a single '='
           if((thisVoice =  dynamic_cast<FourierSynthVoice*>(synth.getVoice(i))))
           {
                   thisVoice->setMAttack(tree.getRawParameterValue("mAttack"));
                   thisVoice->setMDecay(tree.getRawParameterValue("mDecay"));
                   thisVoice->setMSustain(tree.getRawParameterValue("mSustain"));
                   thisVoice->setMRelease(tree.getRawParameterValue("mRelease"));
                   
                   thisVoice->setVAttack(tree.getRawParameterValue("vAttack"));
                   thisVoice->setVDecay(tree.getRawParameterValue("vDecay"));
                   thisVoice->setVSustain(tree.getRawParameterValue("vSustain"));
                   thisVoice->setVRelease(tree.getRawParameterValue("vRelease"));
                   
                   thisVoice->setNumPartials(tree.getRawParameterValue("partialCount"));
                   thisVoice->setSeriesType(tree.getRawParameterValue("typeChoice"));
           }
       }
    
       buffer.clear();
       synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
       if(scopeSetup)
           editorScope->pushBuffer(buffer);
}

//==============================================================================
bool SimpleFourierSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleFourierSynthAudioProcessor::createEditor()
{
    return new SimpleFourierSynthAudioProcessorEditor (*this, editorScope);
}

//==============================================================================
void SimpleFourierSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleFourierSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleFourierSynthAudioProcessor();
}
