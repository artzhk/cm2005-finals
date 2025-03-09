#pragma once

#include <JuceHeader.h>

#include <tuple>

#include "AudioPlayer.h"
#include "MixerVisualiser.h"
#include "WaveDisplay.h"
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

        // void setBuffer();
        // void setBuffer(const juce::AudioBuffer<float>& buffer);

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


        juce::Slider bassSlider;
        juce::Label bassLabel;

        juce::Slider midSlider;
        juce::Label midLabel;

        juce::Slider trembleSlider;
        juce::Label trembleLabel;

        juce::Slider freqSlider;
        juce::Label freqLabel;

        WaveDisplay waveDisplay;
        std::shared_ptr<LiveAudioVisualiser> liveAudioVisualiser;

        juce::FileChooser fChooser{"Select a file...", File::getSpecialLocation(File::userHomeDirectory),
                                   "*.wav;*.mp3;*.aiff"};

        AudioPlayer* player;

        struct SliderParams {
                std::tuple<double, double> range;
                double defaultValue;
                int numDecimalPlaces;
                juce::Slider::SliderStyle style;
                std::string* textValueSuffix = nullptr;
        };

        void setupButton(juce::Button* component);
        void setupSlider(juce::Slider* component, LookAndFeel* lookAndFeel, SliderParams params);
        void setupLabel(juce::Component* target, juce::Label* label, std::string text);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssemblePane)
};
