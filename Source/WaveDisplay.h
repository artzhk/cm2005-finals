#pragma once

#include <JuceHeader.h>

/**
 * WaveDisplay is a component that visually represents the waveform of an audio file.
 * It utilizes JUCE's AudioThumbnail for efficient waveform rendering and implements
 * ChangeListener to react to updates in the loaded audio data.
 */
class WaveDisplay : public juce::Component,
                    public juce::ChangeListener {
       public:
        /**
         * Constructor: Initializes the WaveDisplay with a format manager and a thumbnail cache.
         * @param formatManagerToUse Reference to an AudioFormatManager for handling audio formats.
         * @param cacheToUse Reference to an AudioThumbnailCache for managing waveform thumbnails.
         */
        WaveDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
        
        /** Destructor */
        ~WaveDisplay() override;

        /**
         * Renders the waveform onto the component.
         * @param g Graphics context for drawing.
         */
        void paint(juce::Graphics&) override;

        /** Handles component resizing. */
        void resized() override;

        /**
         * Called when the audio thumbnail changes (e.g., when a new file is loaded).
         * @param source The ChangeBroadcaster source triggering the update.
         */
        void changeListenerCallback(juce::ChangeBroadcaster* source) override;

        /**
         * Loads an audio file from a given URL and generates its waveform display.
         * @param audioURL The URL of the audio file to load.
         */
        void loadURL(juce::URL audioURL);

        /**
         * Sets the playhead position relative to the audio duration.
         * @param pos Normalized position (0.0 to 1.0) within the audio file.
         */
        void setPositionRelative(double pos);

       private:
        /** The audio thumbnail used for waveform rendering. */
        juce::AudioThumbnail audioThumb;

        /** Flag indicating whether an audio file is loaded. */
        bool fileLoaded;

        /** Current playhead position (normalized). */
        double position;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveDisplay)
};
