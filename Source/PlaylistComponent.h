#pragma once

#include <JuceHeader.h>

#include "AssemblePane.h"
#include "Track.h"
#include "juce_gui_basics/juce_gui_basics.h"

/**
 * PlaylistComponent manages the audio library, allowing users to import,
 * search, and load tracks into the player decks.
 */
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::TextEditor::Listener {
       public:
        /**
         * Constructor: Initializes the playlist component and links it to the audio players.
         * @param _assemblePane1 Pointer to the first AssemblePane (left deck).
         * @param _assemblePane2 Pointer to the second AssemblePane (right deck).
         * @param _playerForParsingMetaData Pointer to an AudioPlayer for metadata extraction.
         */
        PlaylistComponent(AssemblePane* _assemblePane1, AssemblePane* _assemblePane2, AudioPlayer* _playerForParsingMetaData);
        
        /** Destructor: Cleans up resources. */
        ~PlaylistComponent() override;

        /** Paints the component. */
        void paint(juce::Graphics&) override;
        
        /** Resizes internal components when the main window is resized. */
        void resized() override;

        /** Returns the number of rows in the playlist table. */
        int getNumRows() override;

        /** Paints the background of a row in the playlist table. */
        void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

        /** Paints the content of a cell in the playlist table. */
        void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

        /**
         * Refreshes a table cell component (e.g., buttons inside cells).
         * @param rowNumber The row index.
         * @param columnId The column index.
         * @param isRowSelected Whether the row is currently selected.
         * @param existingComponentToUpdate A pointer to an existing component that can be reused.
         * @return A new or updated component for the cell.
         */
        Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

        /** Handles button click events within the playlist. */
        void buttonClicked(juce::Button* button) override;

       private:
        /** Table component for displaying the playlist. */
        juce::TableListBox tableComponent;
        
        /** A collection of tracks stored in the playlist. */
        std::vector<Track> tracks;

        /** File chooser for importing audio files. */
        juce::FileChooser fChooser{"Select a file..."};

        /** UI buttons for managing the playlist. */
        juce::TextButton importButton{"IMPORT AUDIO LIBRARY"};
        juce::TextEditor searchField;
        juce::TableListBox library;
        juce::TextButton addToPlayer1Button{"ADD TO LEFT DECK"};
        juce::TextButton addToPlayer2Button{"ADD TO RIGHT DECK"};

        /** Pointers to the two assemble panes (left and right decks). */
        AssemblePane* assemblePane1;
        AssemblePane* assemblePane2;
        
        /** Pointer to an AudioPlayer for extracting metadata from tracks. */
        AudioPlayer* playerForParsingMetaData;

        /** Retrieves the length of an audio file. */
        juce::String getLength(juce::URL audioURL);
        
        /** Converts time in seconds to a minutes:seconds format. */
        juce::String secondsToMinutes(double seconds);

        /** Imports audio files into the playlist. */
        void importToLibrary();
        
        /** Filters tracks in the playlist based on a search query. */
        void searchLibrary(juce::String searchText);
        
        /** Saves the current playlist to a file. */
        void saveLibrary();
        
        /** Loads a saved playlist from a file. */
        void loadLibrary();
        
        /** Deletes a track from the playlist by ID. */
        void deleteFromTracks(int id);
        
        /** Checks if a track is already in the playlist. */
        bool isInTracks(juce::String fileNameWithoutExtension);
        
        /** Finds the index of a track in the playlist. */
        int whereInTracks(juce::String searchText);
        
        /** Loads a track into the specified AssemblePane. */
        void loadInPlayer(AssemblePane* assemblePane);

        /** Prevents copying of the PlaylistComponent instance. */
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
