#include "MainComponent.h"

#include <ostream>

#include "juce_graphics/juce_graphics.h"

//==============================================================================
MainComponent::MainComponent() {
  setSize(600, 400);

  addAndMakeVisible(playButton);
  addAndMakeVisible(stopButton);
  addAndMakeVisible(volumeSlider);
  playButton.addListener(this);
  stopButton.addListener(this);
  volumeSlider.addListener(this);
  setAudioChannels(0, 3);
}

MainComponent::~MainComponent() {}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
  //
  // (Our component is opaque, so we must completely fill the background
  // with a solid colour)
  g.fillAll(juce::Colours::white);

  g.setFont(juce::FontOptions(16.0f));
  g.setColour(juce::Colours::black);
  g.drawText("Hello World!", getLocalBounds(), juce::Justification::bottomLeft,
             true);
}

void MainComponent::resized() {
  // This is called when the MainComponent is resized.
  // If you add any child components, this is where you should
  // update their positions.
  int rowH = getHeight() / 5;
  int closeBthH = getHeight() / 7;

  playButton.setBounds(0, rowH, getWidth(), rowH);
  stopButton.setBounds(2 * (getWidth() / 3), 3 * rowH, getWidth() / 3,
                       closeBthH);
  volumeSlider.setBounds(0, rowH + rowH, getWidth(), rowH);
}

void MainComponent::releaseResources() { return; }

void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
  std::cout << "This is prepare to play method" << std::endl;
  phase = 0;
  dphase = 0.001;

  return;
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
  auto *leftChan =
      bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
  auto *rightChan =
      bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

  for (int i = 0; i < bufferToFill.numSamples; ++i) {
    double sample = fmod(phase, 0.2);
    leftChan[i] = sample;
    rightChan[i] = sample;
    phase += dphase;
  }

  return;
}

/** Called when the button is clicked. */
void MainComponent::buttonClicked(juce::Button *button) {
  if (button == &playButton) {
    std::cout << "lol the button is working" << std::endl;
    return;
  }

  if (button == &stopButton) {
    std::cout << "lol the button stop is working" << std::endl;
    return;
  }

  return;
}

void MainComponent::sliderValueChanged(juce::Slider *slider) {
  std::cout << "aint no way the slider is working" << volumeSlider.getValue()
            << std::endl;

  if (slider == &volumeSlider) {
      dphase = volumeSlider.getValue() * 0.001;
  }

  slider->updateText();
}
