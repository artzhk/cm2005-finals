#include "MainComponent.h"

#include "juce_graphics/juce_graphics.h"

//==============================================================================
MainComponent::MainComponent() {
        setSize(600, 400);

        setAudioChannels(0, 2);

        addAndMakeVisible(assemblePane1);
        addAndMakeVisible(assemblePane2);

        addAndMakeVisible(playlistComponent);

        formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() { shutdownAudio(); }

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {
        g.fillAll(juce::Colour{62, 95, 138});
        g.setColour(juce::Colours::navajowhite);
}

void MainComponent::resized() {
        int rowH = getHeight() / 4;

        assemblePane1.setBounds(0, 0, getWidth() / 2, rowH * 2);
        assemblePane2.setBounds(assemblePane1.getBounds().getRight(), 0, getWidth() / 2, rowH * 2);

        playlistComponent.setBounds(5, assemblePane2.getBounds().getBottom(), getWidth() - 10, rowH * 2);
}

void MainComponent::releaseResources() {
        mixerSource.removeAllInputs();
        mixerSource.releaseResources();
        player1.releaseResources();
        player2.releaseResources();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
        player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

        mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

        mixerSource.addInputSource(&player1, false);
        mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
        if (bufferToFill.buffer == nullptr) {
                return;
        }

        mixerSource.getNextAudioBlock(bufferToFill);

        // bufferToFill.clearActiveBufferRegion();
        // juce::AudioBuffer<float> bufferCopy = *bufferToFill.buffer;
        // Pass a copy to the UI safely
        // juce::MessageManager::callAsync([this, bufferCopy]() mutable {
        //         // assemblePane1.setBuffer(bufferCopy);
        //         // assemblePane1.setBuffer(bufferCopy);
        //         assemblePane2.setBuffer();
        //         assemblePane2.setBuffer();
        // });
}
