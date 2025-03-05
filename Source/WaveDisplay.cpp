/*
  ==============================================================================

    WaveDisplay.cpp
    Created: 03/03/2025 19:40:12
    Author:  artzhk

  ==============================================================================
*/

#include "WaveDisplay.h"

#include <JuceHeader.h>

//==============================================================================
// constructor
WaveDisplay::WaveDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0) {
        // In your constructor, you should add any child components, and initialise any special settings that your
        // component needs.

        audioThumb.addChangeListener(this);
}

WaveDisplay::~WaveDisplay() {}

void WaveDisplay::paint(juce::Graphics& g) {
        /* This demo code just fills the component's background and draws some placeholder text to get you started. Only
           gets called when needed; not continuously

           You should replace everything in this method with your own drawing code..
        */

        g.fillAll(juce::Colour{220, 220, 220});
        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);        // draw an outline around the component

        g.setColour(juce::Colours::orangered);
        if (fileLoaded) {
                // draw inside waveform component
                audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.5f);

                // from 0 to end of audio file
                g.setColour(juce::Colours::lightgreen);
                g.drawRect(position * getWidth(), 0, 5, getHeight());
                g.fillRect(position * getWidth(), 0, 5, getHeight());
        } else {
                // g.setFont (30.0f);
                g.setFont(juce::Font(20.0f, juce::Font::italic));
                g.drawText("audio waveform to come....", getLocalBounds(), juce::Justification::centred,
                           true);        // draw some placeholder text
        }
}

void WaveDisplay::resized() {
        // This method is where you should set the bounds of any child
        // components that your component contains..
}

// DeckGUI needs to tell WaveDisplay when file is ready, from loadButton
void WaveDisplay::loadURL(juce::URL audioURL) {
        audioThumb.clear();
        // getting URL; unpack URL, turn into new input source calling URLInputSource, then setSource onto audioThumb
        // AudioThumbnail::setSource is a boolean; specifies the file or stream that contains the audio file; returns
        fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

        // True is valid audio source need to call paint to draw the waveform after the file is loaded
        if (fileLoaded) {
                DBG("Waveform loaded!");
        } else {
                DBG("WFD: not loaded....");
        }
}

void WaveDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
        // DBG("changeBroadcaster!");
        repaint();
}

void WaveDisplay::setPositionRelative(double pos) {
        if (pos != position && pos > 0) {
                position = pos;
                repaint();
        }
}
