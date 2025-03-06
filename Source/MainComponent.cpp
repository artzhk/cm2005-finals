#include "MainComponent.h"

#include <ostream>

#include "juce_graphics/juce_graphics.h"

//==============================================================================
MainComponent::MainComponent() {
        setSize(600, 400);

        // if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) &&
        //     !RuntimePermissions::isGranted(RuntimePermissions::recordAudio)) {
        //         RuntimePermissions::request(RuntimePermissions::recordAudio, [&](bool granted) {
        //                 if (granted) setAudioChannels(2, 2);
        //         });
        // } else {
        //         setAudioChannels(0, 2);
        // }

        setAudioChannels(0, 2);

        // addAndMakeVisible(playButton);
        // addAndMakeVisible(stopButton);
        // addAndMakeVisible(volumeSlider);
        addAndMakeVisible(assemblePane1);
        addAndMakeVisible(assemblePane2);


        addAndMakeVisible(playlistComponent);

        formatManager.registerBasicFormats();
        // playButton.addListener(this);
        // stopButton.addListener(this);
        // volumeSlider.addListener(this);
}

MainComponent::~MainComponent() { shutdownAudio(); }

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {
        //
        // (Our component is opaque, so we must completely fill the background
        // with a solid colour)
        // g.fillAll(juce::olours::white);

        // g.setFont(juce::FontOptions(16.0f));
        // g.setColour(juce::Colours::black);
        // g.drawText("Hello World!", getLocalBounds(), juce::Justification::bottomLeft, true);

        g.fillAll(juce::Colour{62, 95, 138});

        // You can add your drawing code here!
        g.setColour(juce::Colours::navajowhite);
        g.setFont(30.0f);
        g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized() {
        // This is called when the MainComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        int rowH = getHeight() / 5;
        int closeBthH = getHeight() / 7;

        std::cout << "This is resized method" << std::endl;

        int panesRowHeight = getHeight() / 3;

        assemblePane1.setBounds(0, 0, getWidth() / 2, panesRowHeight * 2);
        assemblePane2.setBounds(getWidth() / 2, 0, getWidth() / 2, panesRowHeight * 2);

        playlistComponent.setBounds(5, panesRowHeight * 2, getWidth() - 10, panesRowHeight - 10);
}

void MainComponent::releaseResources() {
        mixerSource.removeAllInputs();
        mixerSource.releaseResources();
        player1.releaseResources();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        std::cout << "This is prepare to play method" << std::endl;
        // phase = 0.2;
        // dphase = 0.001;
        player1.prepareToPlay(samplesPerBlockExpected, sampleRate);

        mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        mixerSource.addInputSource(&player1, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
        // bufferToFill.clearActiveBufferRegion();
        mixerSource.getNextAudioBlock(bufferToFill);

        // auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        // auto* rightChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

        // for (auto i = 0; i < bufferToFill.numSamples;
        //      ++i)        // increments and assigns; while i++ copies, increements copy and then assigns
        // {
        //         double sample = rand.nextDouble() * 0.5;
        //         // double sample = fmod(phase, 0.3);

        //         leftChannel[i] = sample;
        //         rightChannel[i] = sample;

        //         phase += dphase;
        // }
}
