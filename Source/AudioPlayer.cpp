#include "AudioPlayer.h"
#include <memory>

#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

using namespace juce;

AudioPlayer::AudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager) {
    // Initialize reverb parameters with default values (no effect)
    reverbParameters.roomSize = 0.0f;
    reverbParameters.damping = 0.0f;
    reverbParameters.wetLevel = 0.0f;
    reverbParameters.dryLevel = 1.0f;

    // Set the reverb effect with the initialized parameters
    reverbSource.setParameters(reverbParameters);

    // Initialize filter gain to default values (neutral, no boost/cut)
    bassGain = 1.0f;
    midGain = 1.0f;
    trebleGain = 1.0f;

    // Set up filter coefficients for bass, mid, and treble with default values
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(44100, 200.0f, 0.707f, bassGain);
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(44100, 1000.0f, 1.0f, midGain);
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(44100, 5000.0f, 0.707f, trebleGain);
}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Prepare all audio sources for playback
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    currentSampleRate = sampleRate;
    const uint32 numChannels = 2; // For stereo processing
    juce::dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlockExpected), numChannels};

    // Prepare the filter duplicators (for stereo and multi-channel support)
    bassFilterDuplicator.prepare(spec);
    midFilterDuplicator.prepare(spec);
    trebleFilterDuplicator.prepare(spec);

    // Update filter coefficients based on the new sample rate
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 200.0f, 0.707f, bassGain);
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 1000.0f, 1.0f, midGain);
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 5000.0f, 0.707f, trebleGain);
}

void AudioPlayer::releaseResources() {
    // Release resources from all audio sources to free up memory and processing power
    reverbSource.releaseResources();
    resampleSource.releaseResources();
    transportSource.releaseResources();
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    // Fetch the next audio block of samples for processing
    resampleSource.getNextAudioBlock(bufferToFill);

    // Ensure that the live visualizer is initialized before updating it
    if (liveVisualiser == nullptr) {
        DBG("AudioPlayer::getNextAudioBlock: liveVisualiser is not null");
        return;
    }

    // Create a copy of the audio buffer to send to the visualizer
    const juce::AudioBuffer<float> bufferCopy = *bufferToFill.buffer;
    float magnitude = bufferCopy.getMagnitude(bufferToFill.startSample, bufferToFill.numSamples);

    // Multi-channel processing: Apply filters based on the number of channels
    const int numChannels = bufferToFill.buffer->getNumChannels();
    if (numChannels == 1) {
        // Mono processing: Apply filters to a single channel
        dsp::AudioBlock<float> block(*bufferToFill.buffer);
        dsp::ProcessContextReplacing<float> context(block);

        // Apply bass, mid, and treble filters for mono signal
        bassFilterDuplicator.process(context);
        midFilterDuplicator.process(context);
        trebleFilterDuplicator.process(context);
    } else {
        // Stereo or multi-channel processing: Apply filters to each channel
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

    // Update the live visualizer asynchronously with the buffer and its magnitude
    juce::MessageManager::callAsync([this, bufferCopy, magnitude]() mutable {
        liveVisualiser->setBuffer(bufferCopy, magnitude);
    });
}

void AudioPlayer::loadUrl(URL audioUrl) {
    // Convert the provided URL to a local file path
    File audioFile = audioUrl.getLocalFile();
    formatManager.registerBasicFormats();

    // Enable MP3 support if necessary
#if JUCE_USE_MP3AUDIOFORMAT
    DBG("MP3 support enabled");
    formatManager.registerFormat(new MP3AudioFormat(), true);
#endif

    // Check if the audio file exists
    if (!audioFile.existsAsFile()) {
        DBG("Error: File does not exist -> " + audioFile.getFullPathName());
        return;
    }

    // Create an input stream from the audio file and create a reader for it
    std::unique_ptr<InputStream> inputStream(audioFile.createInputStream());
    AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

    // If the reader could not be created, exit early
    if (reader == nullptr) {
        DBG("Error: could not create reader for file");
        return;
    }

    DBG("Audio file loaded: " << audioUrl.toString(true));

    // Create an audio format reader source, which manages the audio file's playback
    std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));

    // Set up the transport source to control playback of the audio
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

    // Transfer ownership of the new source to the class variable
    readerSource.reset(newSource.release());
}

void AudioPlayer::setGain(double gain) {
    // Set the gain for the transport source if it is within a valid range
    if (gain > 0 && gain < 10.0) {
        transportSource.setGain(gain);
    }
}

void AudioPlayer::setSpeed(double ratio) {
    // Set the resampling ratio for playback speed adjustment if it is within a valid range
    if (ratio > 0 && ratio < 100.0) {
        resampleSource.setResamplingRatio(ratio);
    }
}

void AudioPlayer::setPositionRelative(double pos) {
    // Set the playhead position as a relative value between 0.0 and 1.0
    if (pos > 0 && pos < 1.0) {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void AudioPlayer::setPlayerVisualiser(std::shared_ptr<LiveAudioVisualiser>& _liveVisualiser) {
    // Assign a shared pointer to the live audio visualizer
    liveVisualiser = _liveVisualiser;
}

double AudioPlayer::getPositionRelative() {
    // Calculate and return the relative position of the playhead as a percentage
    if (transportSource.getLengthInSeconds() > 0) {
        return (transportSource.getCurrentPosition() * 100 / transportSource.getLengthInSeconds());
    }
    return 0;
}

double AudioPlayer::getLengthInSeconds() {
    // Return the total length of the audio file in seconds
    return transportSource.getLengthInSeconds();
}

void AudioPlayer::setPosition(double posInSecs) {
    // Set the absolute position of the playhead in seconds
    transportSource.setPosition(posInSecs);
}

void AudioPlayer::start() {
    // Start playback of the audio
    transportSource.start();
}

void AudioPlayer::stop() {
    // Stop playback of the audio
    transportSource.stop();
}

void AudioPlayer::setDamping(float damping) {
    // Set the damping value for the reverb effect
    reverbParameters.damping = damping;
    reverbSource.setParameters(reverbParameters);

    DBG("Damping set to: " << damping);
}

void AudioPlayer::setBassGain(float gain) {
    // Set the gain for the bass filter and update its coefficients
    bassGain = gain;
    bassFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, 200.0f, 0.707f, bassGain);
}

void AudioPlayer::setMidGain(float gain) {
    // Set the gain for the mid filter and update its coefficients
    midGain = gain;
    midFilterDuplicator.state = dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 1000.0f, 1.0f, midGain);
}

void AudioPlayer::setTrebleGain(float gain) {
    // Set the gain for the treble filter and update its coefficients
    trebleGain = gain;
    trebleFilterDuplicator.state = dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 5000.0f, 0.707f, trebleGain);
}
