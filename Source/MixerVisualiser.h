#pragma once

#include <JuceHeader.h>

/**
 * LiveAudioVisualiser is responsible for rendering a real-time visualization
 * of the audio waveform.
 */
class LiveAudioVisualiser : public juce::AudioVisualiserComponent {
       public:
        /**
         * Constructor: Initializes the visualizer with default settings.
         */
        LiveAudioVisualiser();

        /** Destructor: Cleans up resources if necessary. */
        ~LiveAudioVisualiser() override;

        /**
         * Paints the waveform or any additional graphical elements.
         * @param g The graphics context used for rendering.
         */
        void paint(juce::Graphics& g) override;

        /**
         * Updates the visualizer with new audio buffer data.
         * @param buffer The audio buffer containing waveform data.
         * @param magnitude The current magnitude of the waveform for scaling.
         */
        void setBuffer(const juce::AudioBuffer<float>& buffer, float magnitude);

       private:
        /** Stores the magnitude of the current waveform for visual scaling. */
        std::atomic<float> magnitude{0.0f};

        /** Tracks the current volume level for potential UI feedback. */
        double currentVolume;
};
