#pragma once

#include <memory>

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
        double getPositionRelative();
        double getLengthInSeconds();       

        void start();
        void stop();

       private:
        // handle audio file formats
        AudioFormatManager& formatManager;

        // audio playback control and audio volume
        AudioTransportSource transportSource;
        // to create on the fly, to read a file once the file is identified, smart pointer requiered by the JUCE
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        // audio speed control
        ResamplingAudioSource resampleSource{&transportSource, false, 2};
        ReverbAudioSource reverbSource{&resampleSource, false};

        // reverb stuf
        Reverb::Parameters reverbParameters;
};
