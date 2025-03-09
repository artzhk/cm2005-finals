#pragma once

#include <memory>

#include "MixerVisualiser.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"

using namespace juce;

/**
 * AudioPlayer handles audio playback, including loading, controlling playback state,
 * applying basic audio effects (EQ, reverb), and integrating with a visualizer.
 */
class AudioPlayer : public AudioSource {
       public:
        /**
         * Constructor: Initializes the AudioPlayer with an AudioFormatManager.
         * @param _formatManager Reference to a JUCE AudioFormatManager for handling audio formats.
         */
        AudioPlayer(AudioFormatManager& _formatManager);

        /** Destructor */
        ~AudioPlayer();

        /**
         * Prepares the audio source to play by allocating resources.
         * @param samplesPerBlockExpected Number of samples per processing block.
         * @param sampleRate The sample rate in Hz.
         */
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        
        /** Releases resources when playback stops. */
        virtual void releaseResources() override;
        
        /**
         * Called repeatedly to process audio data and send it to the output buffer.
         * @param bufferToFill Contains buffer information to be filled with audio data.
         */
        virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

        /**
         * Loads an audio file from a given URL.
         * @param audioUrl The URL of the audio file to load.
         */
        void loadUrl(URL audioUrl);

        /** Adjusts the overall audio gain (volume). */
        void setGain(double gain);
        
        /** Adjusts playback speed by setting the resampling ratio. */
        void setSpeed(double ratio);
        
        /** Sets the playback position in seconds. */
        void setPosition(double posInSecs);
        
        /** Sets the playback position relative to the total length (0.0 - 1.0). */
        void setPositionRelative(double pos);
        
        /** Links an external visualizer to display live audio waveforms. */
        void setPlayerVisualiser(std::shared_ptr<LiveAudioVisualiser>& _liveVisualiser);

        /** Returns the current playback position relative to the total length. */
        double getPositionRelative();
        
        /** Returns the total length of the loaded audio file in seconds. */
        double getLengthInSeconds();

        /** Adjusts the bass frequency gain. */
        void setBassGain(float gain);
        
        /** Adjusts the mid frequency gain. */
        void setMidGain(float gain);
        
        /** Adjusts the treble frequency gain. */
        void setTrebleGain(float gain);
        
        /** Applies damping to the reverb effect. */
        void setDamping(float damping);

        /** Starts audio playback. */
        void start();
        
        /** Stops audio playback. */
        void stop();

       private:
        /** Current sample rate for playback. Default is 44100 Hz. */
        double currentSampleRate = 44100.0;

        /** Reference to the AudioFormatManager for handling different file formats. */
        AudioFormatManager& formatManager;

        /** Optional visualizer for real-time audio visualization. */
        std::shared_ptr<LiveAudioVisualiser> liveVisualiser;

        /** Gain values for the equalizer (bass, mid, treble). */
        float bassGain{1.0f}, midGain{1.0f}, trebleGain{1.0f};

        /**
         * Digital signal processing filters for equalization.
         * ProcessorDuplicator ensures efficient filtering by applying coefficient-based processing.
         */
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            bassFilterDuplicator;
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            midFilterDuplicator;
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            trebleFilterDuplicator;

        /** Handles playback control and audio transport operations. */
        AudioTransportSource transportSource;

        /**
         * Smart pointer to the audio file reader source.
         * Required for handling file playback and ensuring safe memory management.
         */
        std::unique_ptr<AudioFormatReaderSource> readerSource;

        /**
         * Handles audio resampling for speed adjustments.
         * Set to process two channels (stereo) and positioned before reverb processing.
         */
        ResamplingAudioSource resampleSource{&transportSource, false, 2};

        /**
         * Applies reverb to the processed audio signal.
         * This wraps the resampling source to allow real-time effect control.
         */
        ReverbAudioSource reverbSource{&resampleSource, false};

        /** Stores the parameters for the reverb effect. */
        Reverb::Parameters reverbParameters;
};
