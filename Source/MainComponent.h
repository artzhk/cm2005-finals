#pragma once

#include <JuceHeader.h>

#include "AudioPlayer.h"
#include "PlaylistComponent.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

/**
 * The MainComponent serves as the primary UI and audio control hub of the application.
 * It manages multiple audio players, a playlist, and the overall audio processing pipeline.
 */
class MainComponent : public juce::AudioAppComponent {
       public:
        /**
         * Constructor: Initializes all components and prepares the audio system.
         */
        MainComponent();

        /** Destructor: Cleans up resources before shutdown. */
        ~MainComponent() override;

        /**
         * Paints the component's background and custom graphics.
         * @param g The graphics context used for rendering.
         */
        void paint(juce::Graphics& g) override;
        
        /**
         * Handles resizing of internal components when the window size changes.
         */
        void resized() override;

        /**
         * Fills the output buffer with the next block of audio data.
         * @param bufferToFill The audio buffer to be filled with data.
         */
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        
        /**
         * Prepares the audio system for playback, including sample rate and buffer size settings.
         * @param samplesPerBlockExpected Expected number of samples per processing block.
         * @param sampleRate The sample rate in Hz.
         */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        
        /**
         * Releases any allocated audio resources when playback stops.
         */
        void releaseResources() override;

       private:
        /** Manages audio file format handling (e.g., WAV, MP3). */
        juce::AudioFormatManager formatManager;
        
        /** Caches audio thumbnails for waveform visualization. */
        juce::AudioThumbnailCache thumbnailCache{100};

        /** File chooser dialog for selecting audio files. */
        juce::FileChooser chooser{"Select a file to process..."};

        /** Two independent audio players for mixing or parallel playback. */
        AudioPlayer player1{formatManager};
        AudioPlayer player2{formatManager};

        /**
         * UI panels associated with each audio player, handling playback controls,
         * waveform display, and audio processing.
         */
        AssemblePane assemblePane1{&player1, formatManager, thumbnailCache};
        AssemblePane assemblePane2{&player2, formatManager, thumbnailCache};

        /** An additional AudioPlayer instance used for previewing metadata. */
        AudioPlayer playMetadata{formatManager};

        /** Playlist component managing track selection and playback assignments. */
        PlaylistComponent playlistComponent{&assemblePane1, &assemblePane2, &playMetadata};

        /** Mixer for combining multiple audio sources into a single output. */
        juce::MixerAudioSource mixerSource;

        /** Random number generator (potentially for visual effects or procedural processing). */
        juce::Random rand;

        /** Phase values used for waveform synthesis or effects. */
        double phase;
        double dphase;

        /**
         * JUCE macro to prevent accidental copying of the MainComponent instance,
         * which could lead to resource duplication issues.
         */
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
