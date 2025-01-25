#pragma once

#include <JuceHeader.h>

#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_basics/sources/juce_AudioSource.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener {
 public:
  //==============================================================================
  MainComponent();
  ~MainComponent() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;
  void buttonClicked(juce::Button *button) override;
  void sliderValueChanged(juce::Slider *) override;
  void getNextAudioBlock(const juce::AudioSourceChannelInfo& buffer);

 private:
  //==============================================================================
  // Your private member variables go here...
  juce::TextButton playButton{"Hello honey"};
  juce::TextButton stopButton{"Bye"};
  juce::Slider volumeSlider{juce::Slider::SliderStyle::LinearHorizontal,
                      juce::Slider::TextEntryBoxPosition::TextBoxLeft};

  juce::Random rand;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
