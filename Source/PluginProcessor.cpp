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
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(maId, maName, 1.0, 20000.0, 20.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mdId, mdName, 1.0, 20000.0, 85.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(msId, msName, 0.0, 1.0, 0.6));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mrId, mrName, 1.0, 20000.0, 150.0));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(vaId, vaName, 1.0, 20000.0, 20.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(vdId, vdName, 1.0, 20000.0, 85.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(vsId, vsName, 0.0, 1.0, 0.6));
    layout.add(std::make_unique<juce::AudioParameterFloat>(vrId, vrName, 1.0, 20000.0, 150.0));
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(tcId, tcName, stArray, 0));
    
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool SimpleFourierSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleFourierSynthAudioProcessor::createEditor()
{
    return new SimpleFourierSynthAudioProcessorEditor (*this);
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
