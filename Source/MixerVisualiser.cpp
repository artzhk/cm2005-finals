#include "MixerVisualiser.h"
#include "juce_audio_basics/juce_audio_basics.h"

LiveAudioVisualiser::LiveAudioVisualiser() : juce::AudioVisualiserComponent(2) {
        setBufferSize(1024);
        setSamplesPerBlock(16);
        setColours(juce::Colours::black, juce::Colours::red);
}

LiveAudioVisualiser::~LiveAudioVisualiser() {};

void LiveAudioVisualiser::paint(juce::Graphics& g) {
        AudioVisualiserComponent::paint(g);

        // Optionally, you can add some extra visuals, like volume indicators
        if (currentVolume <= 0.7) {
                g.setColour(juce::Colours::limegreen);
        } else if (currentVolume > 0.7 && currentVolume <= 0.85) {
                g.setColour(juce::Colours::yellow);
        } else {
                g.setColour(juce::Colours::red);
        }

        return;
}

void LiveAudioVisualiser::setBuffer(const juce::AudioBuffer<float>& buffer, float magnitude) {
        // Use pushBuffer to automatically calculate and visualize the waveform
        pushBuffer(buffer);
        // currentVolume = buffer.buffer->getMagnitude(buffer.startSample, buffer.numSamples);
        repaint();
        return;
}
