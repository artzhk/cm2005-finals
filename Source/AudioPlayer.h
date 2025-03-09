#pragma once

#include <memory>

#include "MixerVisualiser.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"

using namespace juce;

class AudioPlayer : public AudioSource {
       public:
        AudioPlayer(AudioFormatManager& _formatManager);

        ~AudioPlayer();
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        virtual void releaseResources() override;
        virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

        void loadUrl(URL audioUrl);

        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);
        void setPlayerVisualiser(std::shared_ptr<LiveAudioVisualiser>& _liveVisualiser);

        double getPositionRelative();
        double getLengthInSeconds();

        void setBassGain(float gain);
        void setMidGain(float gain);
        void setTrebleGain(float gain);
        void setDamping(float damping);

        void start();
        void stop();

       private:
        double currentSampleRate = 44100.0;
        // Handle audio file formats
        AudioFormatManager& formatManager;
        // Optional visuliser
        std::shared_ptr<LiveAudioVisualiser> liveVisualiser;

        float bassGain{1.0f}, midGain{1.0f}, trebleGain{1.0f};
        // juce::dsp::IIR::Filter<float> bassFilter, midFilter, trebleFilter;

        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            bassFilterDuplicator;
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            midFilterDuplicator;
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
            trebleFilterDuplicator;

        // Audio playback control and audio volume
        AudioTransportSource transportSource;

        // To create on the fly, to read a file once the file is identified, smart pointer requiered by the JUCE
        std::unique_ptr<AudioFormatReaderSource> readerSource;

        // Audio speed control
        ResamplingAudioSource resampleSource{&transportSource, false, 2};
        ReverbAudioSource reverbSource{&resampleSource, false};

        // Reverb stuf
        Reverb::Parameters reverbParameters;
};
