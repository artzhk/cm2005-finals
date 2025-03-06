#include "AudioPlayer.h"

#include <memory>

#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

using namespace juce;

AudioPlayer::AudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager) {
        // default values

        // reverb settings
        // 0 - 1 - reverb level
        reverbParameters.roomSize = 0.0f;
        // 0 - 1 - damping level
        reverbParameters.damping = 0.0f;
        // 0 - 1
        reverbParameters.wetLevel = 0.0f;
        // 0 - 1
        reverbParameters.dryLevel = 1.0f;

        reverbSource.setParameters(reverbParameters);
}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::releaseResources() {
        reverbSource.releaseResources();
        // transportSource.releaseResources();
        resampleSource.releaseResources();
        // readerSource->releaseResources();
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
        resampleSource.getNextAudioBlock(bufferToFill);
}

void AudioPlayer::loadUrl(URL audioUrl) {
        File audioFile = audioUrl.getLocalFile();
        formatManager.registerBasicFormats();
#if JUCE_USE_MP3AUDIOFORMAT
        // Enable MP3 support
        DBG("MP3 support enabled");
        formatManager.registerFormat(new MP3AudioFormat(), true);
#endif

        if (!audioFile.existsAsFile()) {
                DBG("Error: File does not exist -> " + audioFile.getFullPathName());
                return;
        }

        std::unique_ptr<InputStream> inputStream(audioFile.createInputStream());

        AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

        if (reader == nullptr) {
                DBG("Error: could not create reader for file");
                return;
        }

        DBG("Audio file loaded: " << audioUrl.toString(true));
        // create audio format reader source, when file is read
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        // control playback of audio
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        // transfer ownership to class variable
        readerSource.reset(newSource.release());
}

void AudioPlayer::setGain(double gain) {
        if (gain > 0 && gain < 10.0) {
                transportSource.setGain(gain);
        }

        return;
}

void AudioPlayer::setSpeed(double ratio) {
        if (ratio > 0 && ratio < 100.0) {
                resampleSource.setResamplingRatio(ratio);
        }

        return;
}

void AudioPlayer::setPositionRelative(double pos) {
        if (pos > 0 && pos < 1.0) {
                double posInSecs = transportSource.getLengthInSeconds() * pos;
                setPosition(posInSecs);
        }

        return;
}

double AudioPlayer::getPositionRelative() {
        if (transportSource.getLengthInSeconds() > 0) {
                return (transportSource.getCurrentPosition() * 100 / transportSource.getLengthInSeconds());
        } else {
                return 0;
        };
}

double AudioPlayer::getLengthInSeconds() { return transportSource.getLengthInSeconds(); }

void AudioPlayer::setPosition(double posInSecs) { transportSource.setPosition(posInSecs); }

void AudioPlayer::start() { transportSource.start(); }

void AudioPlayer::stop() { transportSource.stop(); }
