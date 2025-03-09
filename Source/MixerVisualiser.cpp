#include "MixerVisualiser.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_graphics/juce_graphics.h"

//==============================================================================
LiveAudioVisualiser::LiveAudioVisualiser() : juce::AudioVisualiserComponent(2) {
        // Set the size of the buffer to be used for audio visualization
        setBufferSize(1024);

        // Define the number of samples per block to visualize
        setSamplesPerBlock(16);

        // Set the colours for the visualiser: black background and pink waveform
        setColours(juce::Colours::black, juce::Colours::pink);
}

LiveAudioVisualiser::~LiveAudioVisualiser() { 
        // Destructor (no specific clean-up required for this component)
}

//==============================================================================
void LiveAudioVisualiser::paint(juce::Graphics& g) {
        // Call the base class paint method to render the audio waveform
        AudioVisualiserComponent::paint(g);

        // Optionally, add extra visuals such as volume level indicators
        if (currentVolume <= 0.7) {
                // Green for low volume
                g.setColour(juce::Colours::limegreen);
        } else if (currentVolume > 0.7 && currentVolume <= 0.85) {
                // Yellow for mid-level volume
                g.setColour(juce::Colours::yellow);
        } else {
                // Red for high volume
                g.setColour(juce::Colours::red);
        }

        // The return here is redundant, as the function has no further operations after this point.
        return;
}

void LiveAudioVisualiser::setBuffer(const juce::AudioBuffer<float>& buffer, float magnitude) {
        // Push the provided audio buffer to be visualized
        pushBuffer(buffer);

        // Update the current volume level (magnitude) for the visualizer
        currentVolume = magnitude;

        // Trigger a repaint to update the visualizer with the new data
        repaint();

        // The return here is redundant as well, but keeps the function's signature consistent
        return;
}
