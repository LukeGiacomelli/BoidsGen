/*
  ==============================================================================

    Note.h
    Created: 8 Apr 2025 3:21:45pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Note {
public:
    Note(String ch, int oct = 0) : chroma(ch), octave(oct) {};
    ~Note() {};

    String getChroma() { return chroma; }
    int getOctave() { return octave; }
    double getMidiNote() { return midiNote;  }

    void setChroma(String c) 
    {
        chroma = c; 
        updateMidiNote();
    }
    void setOctave(int o) 
    { 
        octave = o; 
        updateMidiNote();
    }
private:
    void updateMidiNote()
    {
        StringArray notes 
        {
            "C",
            "C#",
            "D",
            "D#",
            "E",
            "F",
            "F#",
            "G", 
            "G#",
            "A",
            "A#",
            "B"
        };

        auto index = notes.indexOf(chroma, true);
        midiNote = (12 * (octave + 2)) + index;
   }

    String chroma;
    int octave;

    double midiNote = 0.0;
};