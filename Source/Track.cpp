#pragma once

#include "Track.h"

//==============================================================================
Track::Track(juce::File _file) 
    : file(_file), title(_file.getFileNameWithoutExtension()), URL(juce::URL{_file}) {
    DBG("Track created: " << title);  // Debug message indicating track creation
}

//==============================================================================
// Overloaded equality operator to compare Track objects by title
bool Track::operator==(const juce::String& other) const {
    return title == other;  // Check if the title of the track matches the given string
}
