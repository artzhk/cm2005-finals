/*
  ==============================================================================

    Track.h
    Created: 2025-03-03 19:38:34
    Author:  artzhk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track {
       public:
        Track(juce::File _file);
        juce::File file;
        juce::URL URL;
        juce::String title;
        juce::String length;

        bool operator==(const juce::String& other) const;        // files are compared by title
};
