/*
  ==============================================================================

    WaveDisplay.cpp
    Created: 03/03/2025 19:40:12
    Author:  artzhk

  ==============================================================================
*/

#include "WaveDisplay.h"

#include <JuceHeader.h>
#include "juce_graphics/juce_graphics.h"

//==============================================================================
// constructor
WaveDisplay::WaveDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0) {
        audioThumb.addChangeListener(this);
}

WaveDisplay::~WaveDisplay() {}

void WaveDisplay::paint(juce::Graphics& g) {
        int relativePosition = (getWidth() * position) / 100;
        // background paint
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.setColour(juce::Colours::grey);

        // main stroke colour
        g.drawRect(getLocalBounds(), 1);
        g.setColour(juce::Colours::ghostwhite);

        if (fileLoaded) {
                audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);

                g.setOpacity(1);
                g.setColour(juce::Colours::red);
                g.fillRect(relativePosition, 0, 2, getHeight());

                g.setColour(juce::Colours::darkmagenta);
                g.setOpacity(0.5f);
                g.fillRect(0, 0, relativePosition, getHeight());

                g.setOpacity(1);
        } else {
                g.setFont(16.0f);
                g.setColour(juce::Colours::yellow);
                g.drawText("Select a song from the playlist...", getLocalBounds(), juce::Justification::centred, true);
        }
}

void WaveDisplay::resized() {}

void WaveDisplay::loadURL(juce::URL audioURL) {
        audioThumb.clear();
        fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

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
