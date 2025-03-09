/*
  ==============================================================================

    WaveDisplay.h
    Created: 03/03/2025 07:13:35
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
                    // add ChangeBroadcaster listener to inheritance definition
                    public juce::ChangeListener {
       public:
        WaveDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache&
                        // constructor with AudioFormatManager and the thumbnail cache
                        cacheToUse);
        ~WaveDisplay() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void changeListenerCallback(juce::ChangeBroadcaster* source) override;

        void loadURL(juce::URL audioURL);

        // set the relative position of the playhead
        void setPositionRelative(double pos);

       private:
        juce::AudioThumbnail audioThumb;
        bool fileLoaded;
        double position;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveDisplay)
};
