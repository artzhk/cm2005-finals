/*
  ==============================================================================

    WaveDisplay.h
    Created: 2025-03-03 07:13:35
    Author:  artzhk

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 * WaveDisplay class is a component that displays the waveform of an audio file
 * using the AudioThumbnail class from JUCE. It also implements the ChangeListener
 * interface to listen for changes in the audio thumbnail.
 */
class WaveDisplay : public juce::Component,
                    public juce::ChangeListener        // add ChangeBroadcaster listener to inheritance definition
{
       public:
        WaveDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache&
                        cacheToUse);        // constructor with AudioFormatManager and the thumbnail cache
        ~WaveDisplay() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void changeListenerCallback(juce::ChangeBroadcaster* source) override;

        void loadURL(juce::URL audioURL);

        void setPositionRelative(double pos);        // set the relative position of the playhead

       private:
        // AudioThumbnail constructor (to draw waveform share of audio) has three arguments, but
        // do not have access to incoming data coming into my constructor (WaveDisplay
        // so need to add it to initialization list in the constructor file >> audioThumb(1000, formatManagerToUse,
        // cacheToUse) only want 1 instance of AudioFormatManager and AudioThumbnailCache since the cache is shared
        // between any audio thumbnails; both will be part of Main Component where data is stored
        juce::AudioThumbnail audioThumb;
        bool fileLoaded;
        double position;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveDisplay)
};
