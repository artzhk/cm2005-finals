#include "AudioPlayer.h"

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
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
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
        // unpack audio URL and converted to input stream, and creates a reader
        auto* reader = formatManager.createReaderFor(audioUrl.createInputStream(false));

        if (reader == nullptr) {
                return;
        }

        // create audio format reader source, when file is read
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        // control playback of audio
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        // transfer ownership to class variable
        readerSource.reset(newSource.release());
}

void AudioPlayer::setGain(double gain) {
        if (gain > 0 && gain < 1.0) {
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
                return (transportSource.getCurrentPosition() / transportSource.getLengthInSeconds()) * 100;
        } else {
                return 0;
        };
}

double AudioPlayer::getLengthInSeconds() {
        return transportSource.getLengthInSeconds();
}

void AudioPlayer::setPosition(double posInSecs) { transportSource.setPosition(posInSecs); }

void AudioPlayer::start() { transportSource.start(); }

void AudioPlayer::stop() { transportSource.stop(); }
