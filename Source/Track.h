#pragma once
#include <JuceHeader.h>

/**
 * Represents an audio track, storing file metadata and providing comparison functionality.
 */
class Track {
       public:
        /**
         * Constructor: Initializes a Track with a given file.
         * @param _file The file associated with this track.
         */
        Track(juce::File _file);

        /** The file containing the audio track. */
        juce::File file;
        
        /** The URL reference to the track's location. */
        juce::URL URL;
        
        /** The title of the track. */
        juce::String title;
        
        /** The duration of the track in a formatted string. */
        juce::String length;

        /**
         * Compares this track with another track based on the title.
         * @param other The title to compare against.
         * @return True if the titles match, false otherwise.
         */
        bool operator==(const juce::String& other) const;
};
