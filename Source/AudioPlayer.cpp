#include "AudioPlayer.h"
#include <memory>

#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

using namespace juce;

AudioPlayer::AudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager) {
    // Initialize reverb parameters
    reverbParameters.roomSize = 0.0f;
    reverbParameters.damping = 0.0f;
    reverbParameters.wetLevel = 0.0f;
    reverbParameters.dryLevel = 1.0f;

    reverbSource.setParameters(reverbParameters);

    // Initialize filter gain
    bassGain = 1.0f;
    midGain = 1.0f;
    trebleGain = 1.0f;

    // Filter coefficients setup (initial values)
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(44100, 200.0f, 0.707f, bassGain);
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(44100, 1000.0f, 1.0f, midGain);
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(44100, 5000.0f, 0.707f, trebleGain);
}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Prepare audio sources
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    currentSampleRate = sampleRate;
    const uint32 numChannels = 2; // For stereo processing
    juce::dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlockExpected), numChannels};

    // Prepare the filter duplicators (multi-channel processing)
    bassFilterDuplicator.prepare(spec);
    midFilterDuplicator.prepare(spec);
    trebleFilterDuplicator.prepare(spec);

    // Update filter coefficients with new sample rate
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 200.0f, 0.707f, bassGain);
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 1000.0f, 1.0f, midGain);
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 5000.0f, 0.707f, trebleGain);
}

void AudioPlayer::releaseResources() {
    // Release resources from all audio sources
    reverbSource.releaseResources();
    resampleSource.releaseResources();
    transportSource.releaseResources();
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    // Fetch the audio block
    resampleSource.getNextAudioBlock(bufferToFill);

    if (liveVisualiser == nullptr) {
        DBG("AudioPlayer::getNextAudioBlock: liveVisualiser is not null");
        return;
    }

    const juce::AudioBuffer<float> bufferCopy = *bufferToFill.buffer;
    float magnitude = bufferCopy.getMagnitude(bufferToFill.startSample, bufferToFill.numSamples);

    // Multi-channel processing: Apply the filters
    const int numChannels = bufferToFill.buffer->getNumChannels();
    if (numChannels == 1) {
        // Mono processing
        dsp::AudioBlock<float> block(*bufferToFill.buffer);
        dsp::ProcessContextReplacing<float> context(block);

        // Apply bass, mid, and treble filters for mono signal
        bassFilterDuplicator.process(context);
        midFilterDuplicator.process(context);
        trebleFilterDuplicator.process(context);
    } else {
        // Stereo or multi-channel processing
        for (int channel = 0; channel < numChannels; ++channel) {
            float* channelData = bufferToFill.buffer->getWritePointer(channel);
            float* channelPtr[1] = {channelData};

            dsp::AudioBlock<float> block(channelPtr, 1, bufferToFill.buffer->getNumSamples());
            dsp::ProcessContextReplacing<float> context(block);

            // Apply bass, mid, and treble filters for each channel
            bassFilterDuplicator.process(context);
            midFilterDuplicator.process(context);
            trebleFilterDuplicator.process(context);
        }
    }

    // Optional: Update live visualizer (only if it's initialized)
    juce::MessageManager::callAsync([this, bufferCopy, magnitude]() mutable {
        liveVisualiser->setBuffer(bufferCopy, magnitude);
    });
}

void AudioPlayer::loadUrl(URL audioUrl) {
    File audioFile = audioUrl.getLocalFile();
    formatManager.registerBasicFormats();

    // Enable MP3 support
#if JUCE_USE_MP3AUDIOFORMAT
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
}

void AudioPlayer::setSpeed(double ratio) {
    if (ratio > 0 && ratio < 100.0) {
        resampleSource.setResamplingRatio(ratio);
    }
}

void AudioPlayer::setPositionRelative(double pos) {
    if (pos > 0 && pos < 1.0) {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void AudioPlayer::setPlayerVisualiser(std::shared_ptr<LiveAudioVisualiser>& _liveVisualiser) {
    liveVisualiser = _liveVisualiser;
}

double AudioPlayer::getPositionRelative() {
    if (transportSource.getLengthInSeconds() > 0) {
        return (transportSource.getCurrentPosition() * 100 / transportSource.getLengthInSeconds());
    }
    return 0;
}

double AudioPlayer::getLengthInSeconds() {
    return transportSource.getLengthInSeconds();
}

void AudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void AudioPlayer::start() {
    transportSource.start();
}

void AudioPlayer::stop() {
    transportSource.stop();
}

void AudioPlayer::setDamping(float damping) {
    reverbParameters.damping = damping;
    reverbSource.setParameters(reverbParameters);

    DBG("Damping set to: " << damping);
}

void AudioPlayer::setBassGain(float gain) {
    bassGain = gain;
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, 200.0f, 0.707f, bassGain);
}

void AudioPlayer::setMidGain(float gain) {
    midGain = gain;
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 1000.0f, 1.0f, midGain);
}

void AudioPlayer::setTrebleGain(float gain) {
    trebleGain = gain;
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 5000.0f, 0.707f, trebleGain);
}
