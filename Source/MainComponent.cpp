#include "MainComponent.h"

#include "juce_graphics/juce_graphics.h"

//==============================================================================
MainComponent::MainComponent() {
        // Set the size of the main component
        setSize(600, 400);

        // Check if the application needs audio recording permissions
        if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) &&
            !RuntimePermissions::isGranted(RuntimePermissions::recordAudio)) {
                // Request permission to record audio if it's required
                RuntimePermissions::request(RuntimePermissions::recordAudio, [&](bool granted) {
                        if (granted) setAudioChannels(2, 2);  // If granted, set audio input and output channels to 2
                });
        } else {
                // If permission is not required or already granted, set audio input to none and output to 2 channels
                setAudioChannels(0, 2);
        }

        // Make sure the child components are visible
        addAndMakeVisible(assemblePane1);
        addAndMakeVisible(assemblePane2);
        addAndMakeVisible(playlistComponent);

        // Register the basic audio formats for playback (e.g., MP3, WAV)
        formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() { 
        // Shutdown audio resources when the component is destroyed
        shutdownAudio(); 
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {
        // Fill the background with a custom color
        g.fillAll(juce::Colour{12, 15, 88});

        // Set the color for text or drawing
        g.setColour(juce::Colours::navajowhite);
}

void MainComponent::resized() {
        // Calculate the height for each row
        int rowH = getHeight() / 4;

        // Set bounds for assemblePane1 (takes up 3/4 of the height and half the width)
        assemblePane1.setBounds(0, 0, getWidth() / 2, rowH * 3);

        // Set bounds for assemblePane2 (to the right of assemblePane1, sharing the same height)
        assemblePane2.setBounds(assemblePane1.getBounds().getRight(), 0, getWidth() / 2, rowH * 3);

        // Set bounds for the playlistComponent (located below assemblePane2)
        playlistComponent.setBounds(5, assemblePane2.getBounds().getBottom(), getWidth() - 10, rowH);
}

void MainComponent::releaseResources() {
        // Remove all input sources and release resources for the mixer and players
        mixerSource.removeAllInputs();
        mixerSource.releaseResources();
        player1.releaseResources();
        player2.releaseResources();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        // Prepare the two audio players for playback
        player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
        player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

        // Prepare the mixer for playback and add both players as input sources
        mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        mixerSource.addInputSource(&player1, false);
        mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
        // Check if the buffer is valid
        if (bufferToFill.buffer == nullptr) {
                return;
        }

        // Pass the audio block to the mixer for processing
        mixerSource.getNextAudioBlock(bufferToFill);
}
