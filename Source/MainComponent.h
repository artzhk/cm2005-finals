#pragma once

#include <JuceHeader.h>

#include "AssesmblePane.h"
#include "AudioPlayer.h"
#include "PlaylistComponent.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent {
       public:
        //==============================================================================
        MainComponent();
        ~MainComponent() override;

        //==============================================================================
        void paint(juce::Graphics &) override;
        void resized() override;

        void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void releaseResources() override;

       private:
        juce::AudioFormatManager formatManager;
        juce::AudioThumbnailCache thumbnailCache{100};

        juce::FileChooser chooser{"Select a file to proccess..." };

        AudioPlayer player1{formatManager};
        AudioPlayer player2{formatManager};

        AssemblePane assemblePane1{&player1, formatManager, thumbnailCache};
        AssemblePane assemblePane2{&player2, formatManager, thumbnailCache};

        AudioPlayer playMetadata{formatManager};
        PlaylistComponent playlistComponent{&assemblePane1, &assemblePane2, &playMetadata};

        juce::MixerAudioSource mixerSource;

        juce::Random rand;
        double phase;
        double dphase;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
