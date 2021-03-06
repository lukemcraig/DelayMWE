/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayMweAudioProcessorEditor::DelayMweAudioProcessorEditor (DelayMweAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DelayMweAudioProcessorEditor::~DelayMweAudioProcessorEditor()
{
}

//==============================================================================
void DelayMweAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("This is a minimum working example of a delay, implemented with a circular buffer.\n It is simplified from an example given in Audio Effects: Theory, Implementation and Application \n The delay time is one second. It is 100% wet.", getLocalBounds(), Justification::centred, 1);
}

void DelayMweAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
