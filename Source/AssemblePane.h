#pragma once

#include <JuceHeader.h>

#include <tuple>

#include "AudioPlayer.h"
#include "MixerVisualiser.h"
#include "WaveDisplay.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/**
 * AssemblePane is a JUCE component that provides a UI for controlling audio playback.
 * It allows users to load an audio file, start/stop playback, and adjust playback parameters
 * such as volume, speed, damping, and equalization settings.
 */
class AssemblePane : public juce::Component,
                     public juce::Button::Listener,
                     public juce::Slider::Listener,
                     public juce::FileDragAndDropTarget,
                     public juce::Timer
{
       public:
        /**
         * Constructor: Initializes the UI components and links the audio player.
         * @param player Pointer to an AudioPlayer instance for handling audio playback.
         * @param formatManagerToUse Reference to a JUCE AudioFormatManager for file format handling.
         * @param cacheToUse Reference to a JUCE AudioThumbnailCache for efficient waveform visualization.
         */
        AssemblePane(AudioPlayer* player, juce::AudioFormatManager& formatManagerToUse,
                     juce::AudioThumbnailCache& cacheToUse);

        /** Destructor */
        ~AssemblePane() override;

        /** Paint method to define the visual appearance of the component. */
        void paint(juce::Graphics&) override;
        
        /** Handles component layout and resizing. */
        void resized() override;

        /** Handles button click events (e.g., play, stop, load buttons). */
        void buttonClicked(juce::Button* button) override;

        /** Handles slider value changes, updating audio parameters dynamically. */
        void sliderValueChanged(juce::Slider* slider) override;

        /** Determines whether the component accepts dragged file types. */
        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        
        /** Handles the event when files are dropped onto the component. */
        void filesDropped(const juce::StringArray& files, int, int y) override;

        /** Called at regular intervals to update the component (e.g., UI updates). */
        void timerCallback() override;

        /** Loads an audio file from a given URL and prepares it for playback. */
        void loadFile(juce::URL audioURL);

       private:
        /** Look and Feel settings for different UI elements. */
        juce::LookAndFeel_V4 otherLookAndFeel1;
        juce::LookAndFeel_V4 otherLookAndFeel2;
        juce::LookAndFeel_V4 otherLookAndFeel3;

        /** UI buttons for playback control. */
        juce::TextButton playButton{"Start"}; ///< Starts audio playback.
        juce::TextButton stopButton{"Stop"};  ///< Stops audio playback.
        juce::TextButton loadButton{"Load track"}; ///< Opens file chooser to load a track.

        /** UI sliders and labels for controlling playback parameters. */
        juce::Slider volSlider; ///< Controls volume level.
        juce::Label volLabel;

        juce::Slider speedSlider; ///< Controls playback speed.
        juce::Label speedLabel;

        juce::Slider dampingSlider; ///< Controls damping effect.
        juce::Label dampingLabel;

        juce::Slider positionSlider; ///< Displays and allows seeking through the track.
        juce::Label positionLabel;

        juce::Slider bassSlider; ///< Controls bass frequencies.
        juce::Label bassLabel;

        juce::Slider midSlider; ///< Controls mid frequencies.
        juce::Label midLabel;

        juce::Slider trembleSlider; ///< Controls high frequencies.
        juce::Label trembleLabel;

        juce::Slider freqSlider; ///< Controls filter cutoff frequency.
        juce::Label freqLabel;

        /** Waveform display component for visualizing the loaded audio track. */
        WaveDisplay waveDisplay;
        
        /** Pointer to an instance of LiveAudioVisualiser for real-time audio visualization. */
        std::shared_ptr<LiveAudioVisualiser> liveAudioVisualiser;

        /** File chooser dialog for loading audio files. */
        juce::FileChooser fChooser{"Select a file...", File::getSpecialLocation(File::userHomeDirectory), "*.wav;*.mp3;*.aiff"};

        /** Pointer to the audio player handling playback. */
        AudioPlayer* player;

        /**
         * Structure holding slider configuration parameters.
         */
        struct SliderParams {
                std::tuple<double, double> range; ///< Min and max values for the slider.
                double defaultValue; ///< Default value when initialized.
                int numDecimalPlaces; ///< Number of decimal places for displayed value.
                juce::Slider::SliderStyle style; ///< Visual style of the slider.
                std::string* textValueSuffix = nullptr; ///< Optional suffix for displayed value (e.g., " Hz").
        };

        /**
         * Helper function to set up a button with common properties.
         * @param component Pointer to the button being configured.
         */
        void setupButton(juce::Button* component);

        /**
         * Helper function to configure a slider's properties.
         * @param component Pointer to the slider being set up.
         * @param lookAndFeel Pointer to the LookAndFeel instance for styling.
         * @param params Slider parameters defining range, default value, etc.
         */
        void setupSlider(juce::Slider* component, LookAndFeel* lookAndFeel, SliderParams params);

        /**
         * Helper function to set up labels linked to UI components.
         * @param target Pointer to the UI component the label describes.
         * @param label Pointer to the label being set up.
         * @param text Text to display in the label.
         */
        void setupLabel(juce::Component* target, juce::Label* label, std::string text);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssemblePane)
};
