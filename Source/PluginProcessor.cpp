/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayMweAudioProcessor::DelayMweAudioProcessor(): delayBuffer_ (2,1)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	delayReadPosition_ = 0;
	delayWritePosition_ = 0;
}

DelayMweAudioProcessor::~DelayMweAudioProcessor()
{
}

//==============================================================================
const String DelayMweAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayMweAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayMweAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayMweAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayMweAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayMweAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayMweAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayMweAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayMweAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayMweAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayMweAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	delayBufferLength_ = (int)(2.0*sampleRate);
	if (delayBufferLength_ < 1)
		delayBufferLength_ = 1;

	delayBuffer_.setSize(2, delayBufferLength_);
	delayBuffer_.clear();

	delayReadPosition_ = (int)(delayWritePosition_ - sampleRate + delayBufferLength_) % delayBufferLength_;
}

void DelayMweAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayMweAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void DelayMweAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples();

	int dpr; // delay pointer read
	int dpw; // delay pointer write
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
		float* delayData = delayBuffer_.getWritePointer(channel);
		// copy the variables so that the channels are independent
		dpr = delayReadPosition_;
		dpw = delayWritePosition_;

		for (int sample = 0; sample < numSamples; ++sample) {
			const float in = channelData[sample];
			float out = 0.0;			
			//y[n] = x[n - N]
			// N : delay time
			delayData[dpw] = in;

			if (++dpr >= delayBufferLength_)
				dpr = 0;
			if (++dpw >= delayBufferLength_)
				dpw = 0;

			out = delayData[dpr];
			channelData[sample] = out;
		}
    }
	delayReadPosition_ = dpr;
	delayWritePosition_ = dpw;

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}
}

//==============================================================================
bool DelayMweAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayMweAudioProcessor::createEditor()
{
    return new DelayMweAudioProcessorEditor (*this);
}

//==============================================================================
void DelayMweAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayMweAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayMweAudioProcessor();
}
