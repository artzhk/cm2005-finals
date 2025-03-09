#include "WaveDisplay.h"

#include <JuceHeader.h>
#include "juce_graphics/juce_graphics.h"

//==============================================================================
WaveDisplay::WaveDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0) {
        audioThumb.addChangeListener(this);  // Add this object as a listener to the audio thumbnail
}

WaveDisplay::~WaveDisplay() {}

//==============================================================================
void WaveDisplay::paint(juce::Graphics& g) {
        int relativePosition = (getWidth() * position) / 100;  // Calculate the position relative to the display width

        // Set background color
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        // Draw a border around the display area
        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);

        // Set the color for the waveform
        g.setColour(juce::Colours::ghostwhite);

        if (fileLoaded) {
                // If the file is loaded, draw the waveform
                audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);

                // Draw the position indicator (red line)
                g.setOpacity(1);
                g.setColour(juce::Colours::red);
                g.fillRect(relativePosition, 0, 2, getHeight());

                // Highlight the part of the waveform that has been played (dark magenta area)
                g.setColour(juce::Colours::darkmagenta);
                g.setOpacity(0.5f);
                g.fillRect(0, 0, relativePosition, getHeight());

                g.setOpacity(1);
        } else {
                // If no file is loaded, display a message
                g.setFont(16.0f);
                g.setColour(juce::Colours::yellow);
                g.drawText("Select a song from the playlist...", getLocalBounds(), juce::Justification::centred, true);
        }
}

void WaveDisplay::resized() {}

//==============================================================================
void WaveDisplay::loadURL(juce::URL audioURL) {
        audioThumb.clear();  // Clear the current thumbnail
        fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));  // Load the new audio file

        if (fileLoaded) {
                DBG("Waveform loaded!");  // Debug message when the waveform is successfully loaded
                repaint();
        } else {
                DBG("WFD: not loaded....");  // Debug message if the file failed to load
        }
}

void WaveDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
        repaint();  // Redraw the waveform when there is a change in the thumbnail
}

void WaveDisplay::setPositionRelative(double pos) {
        if (pos != position && pos > 0) {  // Only update if the position has changed and it's a positive value
                position = pos;
                repaint();  // Repaint to reflect the new position
        }
}
