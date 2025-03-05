/*
  ==============================================================================
    PlaylistComponent.h
    Created: 05/03/2025 07:39:27
    Author:  artzhk
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AssesmblePane.h"
#include "Track.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::TextEditor::Listener        // inherit TableListBoxModel, to allow
                                                                   // PlayListComponent to behave like a table
{
       public:
        PlaylistComponent(AssemblePane* _deckGUI1, AssemblePane* _deckGUI2, AudioPlayer* _playerForParsingMetaData);
        ~PlaylistComponent() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        int getNumRows() override;

        void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

        void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height,
                       bool rowIsSelected) override;

        Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                           Component* existingComponentToUpdate) override;

        void buttonClicked(juce::Button* button) override;

       private:
        juce::TableListBox tableComponent;
        std::vector<Track> tracks;

        juce::FileChooser fChooser{"Select a file..."};

        juce::TextButton importButton{"IMPORT AUDIO LIBRARY"};
        juce::TextEditor searchField;
        juce::TableListBox library;
        juce::TextButton addToPlayer1Button{"ADD TO LEFT DECK"};
        juce::TextButton addToPlayer2Button{"ADD TO RIGHT DECK"};

        AssemblePane* deckGUI1;
        AssemblePane* deckGUI2;
        AudioPlayer* playerForParsingMetaData;

        juce::String getLength(juce::URL audioURL);
        juce::String secondsToMinutes(double seconds);

        void importToLibrary();
        void searchLibrary(juce::String searchText);
        void saveLibrary();
        void loadLibrary();
        void deleteFromTracks(int id);
        bool isInTracks(juce::String fileNameWithoutExtension);
        int whereInTracks(juce::String searchText);
        void loadInPlayer(AssemblePane* AssemblePane);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
