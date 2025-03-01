#pragma once

#include <JuceHeader.h>

#include <tuple>

#include "AudioPlayer.h"        // to know about this class
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
class AssemblePane : public juce::Component,
                     public juce::Button::Listener,
                     public juce::Slider::Listener,
                     public juce::FileDragAndDropTarget,
                     public juce::Timer

{
       public:
        AssemblePane(AudioPlayer* player, juce::AudioFormatManager& formatManagerToUse,
                     juce::AudioThumbnailCache& cacheToUse);
        // AssemblePane gets DJ audio player via the constructor, and use assignment list
        ~AssemblePane() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        // intended to handle button click listener events
        void buttonClicked(juce::Button* button) override;

        // intended to handle to implement Slider::Listener
        void sliderValueChanged(juce::Slider* slider) override;

        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        void filesDropped(const juce::StringArray& files, int, int y) override;

        void timerCallback() override;
        void loadFile(juce::URL audioURL);

       private:
        // Add instances of the components beeing processed
        juce::LookAndFeel_V4 otherLookAndFeel1;
        juce::LookAndFeel_V4 otherLookAndFeel2;
        juce::LookAndFeel_V4 otherLookAndFeel3;

        juce::TextButton playButton{"Start"};
        juce::TextButton stopButton{"Stop"};
        juce::TextButton loadButton{"Load track"};

        juce::Slider volSlider;
        juce::Label volLabel;

        juce::Slider speedSlider;
        juce::Label speedLabel;

        juce::Slider dampingSlider;
        juce::Label dampingLabel;

        juce::Slider positionSlider;
        juce::Label positionLabel;

        juce::FileChooser fChooser{"Select a file..."};

        AudioPlayer* player;        // AssemblePane is unaware of audio player;
                                    // when receive events from buttons, how to send events on to DJAudio player;
                                    // so need reference with pointer

        struct SliderParams {
                std::tuple<double, double> range;
                double defaultValue;
                int numDecimalPlaces;
                juce::Slider::SliderStyle style;
        };

        void setupButton(juce::Button* component);
        void setupSlider(juce::Slider* component, LookAndFeel* lookAndFeel, SliderParams params);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssemblePane)
};
