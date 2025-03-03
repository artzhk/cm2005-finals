/*
  ==============================================================================

    Track.cpp
    Created: 2025-03-03 19:39:15
    Author:  artzhk

  ==============================================================================
*/

#include "Track.h"

Track::Track(juce::File _file) : file(_file), title(_file.getFileNameWithoutExtension()), URL(juce::URL{_file}) {
        DBG("Track created: " << title);
}

bool Track::operator==(const juce::String& other) const { return title == other; }
