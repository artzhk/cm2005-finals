#pragma once

#include <JuceHeader.h>

class LiveAudioVisualiser : public juce::AudioVisualiserComponent {
       public:
        LiveAudioVisualiser();
        ~LiveAudioVisualiser() override;

        void paint(juce::Graphics&) override;
        void setBuffer(const juce::AudioBuffer<float>& buffer, float magnitude);

       private:
        std::atomic<float> magnitude{0.0f};
        double currentVolume;
};
