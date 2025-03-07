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
        audioThumb.addChangeListener(this);
}

WaveDisplay::~WaveDisplay() {}

void WaveDisplay::paint(juce::Graphics& g) {
        //background paint
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.setColour(juce::Colours::grey);

        // main stroke colour
        g.drawRect(getLocalBounds(), 1);
        g.setColour(juce::Colours::cyan);

        if (fileLoaded) {
                audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);

                g.setOpacity(1);
                g.setColour(juce::Colours::red);
                g.fillRect(position, 0, 2, getHeight());

                g.setColour(juce::Colours::darkmagenta);
                g.setOpacity(0.5f);
                g.fillRect(0, 0, position, getHeight());

                g.setOpacity(1);
        } else {
                g.setFont(16.0f);
                g.setColour(juce::Colours::yellow);
                g.drawText("Select a song from the playlist...", getLocalBounds(), juce::Justification::centred, true);
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
                repaint();
        } else {
                DBG("WFD: not loaded....");
        }
}

void WaveDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) { repaint(); }

void WaveDisplay::setPositionRelative(double pos) {
        if (pos != position && pos > 0) {
                position = pos;
                repaint();
        }
}
