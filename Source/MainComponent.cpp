#include "MainComponent.h"

#include <ostream>

#include "juce_graphics/juce_graphics.h"

//==============================================================================
MainComponent::MainComponent() {
        setSize(600, 400);

        if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio) &&
            !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio)) {
                juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                                                  [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
        } else {
                // Specify the number of input and output channels that we want to open
                setAudioChannels(2, 2);
        }

        // addAndMakeVisible(playButton);
        // addAndMakeVisible(stopButton);
        // addAndMakeVisible(volumeSlider);
        addAndMakeVisible(assemblePane1);

        formatManager.registerBasicFormats();
        // playButton.addListener(this);
        // stopButton.addListener(this);
        // volumeSlider.addListener(this);
}

MainComponent::~MainComponent() { shutdownAudio(); }

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
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

        // playButton.setBounds(0, rowH, getWidth(), rowH);
        // stopButton.setBounds(2 * (getWidth() / 3), 3 * rowH, getWidth() / 3, closeBthH);
        // volumeSlider.setBounds(0, rowH + rowH, getWidth(), rowH);

        assemblePane1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);        // getWidth() / 2 to set two decks
}

void MainComponent::releaseResources() {
        mixerSource.removeAllInputs();
        mixerSource.releaseResources();
        player1.releaseResources();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        std::cout << "This is prepare to play method" << std::endl;
        phase = 0;
        dphase = 0.001;

        mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        mixerSource.addInputSource(&player1, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
        mixerSource.getNextAudioBlock(bufferToFill);
        return;
}
