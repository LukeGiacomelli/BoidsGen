/*
  ==============================================================================

    Piano.h
    Created: 8 Apr 2025 3:22:30pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "Area.h"
#include "MidiManager.h"

#define piano_width 800
#define screen_width 1300
#define screen_height 940

class Piano {
public:
    Piano(int low_o, int high_o, String scale_key, MidiManager& midi, int& n_boids);
    ~Piano() {};

    std::vector<std::vector<Area>>& getAreaCollection() { return areas_collection; }
    Rectangle<int> getPianoBounds() { return Rectangle<int> { lato,lato }; }
    Rectangle<int>& getPianoBoundsInTheScreen() { return pianoBoundsInTheScreen; }
    Point<int> getScreen() { return {screen_width,screen_height}; }

    int getLowOctave() const { return low_oct; }
    int getHighOctave() const { return high_oct; }
    bool getUpdatedState() const { return updated; }
    String getScale() const { return scale; }
    std::array<String, 12> getNoteInScale() { return notes; }
    std::array<String, 7>& getNoteInTonality() { return tonalities[currentTonality]; }
    void updatePianoTonality(String newTonality);
    float getDeltaOffset() const { return deltaOffset; }
    float getThreshold() const { return areasThreshold; }
    bool getAutoThreshold() const { return autoThreshold; }

    void setUpdated(bool s) { updated = s;}

    void setScale(String scale_key) 
    { 
        scale = scale_key; 
        updatePiano();
    }
    void setLowOct(int low) 
    { 
        if ((high_oct - low) >= 1 && (high_oct - low) <= 4) 
        {
            low_oct = low;
            updatePiano();
        }
    }
    void setHighOct(int high) 
    {
        if ((high - low_oct) >= 1 && (high - low_oct) <= 4)
        {
            high_oct = high;
            updatePiano();
        }
    }
    void setThreshold(float newThr) { areasThreshold = newThr; }
    void useAutoThreshold(bool autoThr) { autoThreshold = autoThr; }
private:
    const int lato = piano_width;
    Rectangle<int> pianoBoundsInTheScreen{};
    void updatePianoBoundsInTheScreen(int cols, float cellWidth, float deltaOffset);
    float deltaOffset = 0.f;

    void updatePiano();
    void resetPiano();

    String currentTonality = "Cmaj";
    String scale;
    int low_oct, high_oct;
    int numberOfNotes;
    float areasThreshold = 1.f;
    bool autoThreshold = true;
    std::vector<std::vector<Area>> areas_collection;

    bool updated = true;

    std::array<String, 12> notes = { "C", "C#", "D", "D#","E", "F", "F#", "G", "G#", "A", "A#", "B" };
    std::map<String, std::array<String, 7>> tonalities = {
        // Scale maggiori
        {"Cmaj",  {"C", "D", "E", "F", "G", "A", "B"}},
        {"C#maj", {"C#", "D#", "E#", "F#", "G#", "A#", "B#"}},
        {"Dmaj",  {"D", "E", "F#", "G", "A", "B", "C#"}},
        {"Ebmaj", {"D#", "F", "G", "G#", "A#", "C", "D"}},
        {"Emaj",  {"E", "F#", "G#", "A", "B", "C#", "D#"}},
        {"Fmaj",  {"F", "G", "A", "A#", "C", "D", "E"}},
        {"F#maj", {"F#", "G#", "A#", "B", "C#", "D#", "E#"}},
        {"Gmaj",  {"G", "A", "B", "C", "D", "E", "F#"}},
        {"Abmaj", {"G#", "A#", "C", "C#", "D#", "F", "G"}},
        {"Amaj",  {"A", "B", "C#", "D", "E", "F#", "G#"}},
        {"Bbmaj", {"A#", "C", "D", "D#", "F", "G", "A"}},
        {"Bmaj",  {"B", "C#", "D#", "E", "F#", "G#", "A#"}},

        // Scale minori naturali
        {"Cmin",  {"C", "D", "D#", "F", "G", "G#", "A#"}},
        {"C#min", {"C#", "D#", "E", "F#", "G#", "A", "B"}},
        {"Dmin",  {"D", "E", "F", "G", "A", "A#", "C"}},
        {"Ebmin", {"D#", "F", "F#", "G#", "A#", "B", "C#"}},
        {"Emin",  {"E", "F#", "G", "A", "B", "C", "D"}},
        {"Fmin",  {"F", "G", "G#", "A#", "C", "C#", "D#"}},
        {"F#min", {"F#", "G#", "A", "B", "C#", "D", "E"}},
        {"Gmin",  {"G", "A", "A#", "C", "D", "D#", "F"}},
        {"Abmin", {"G#", "A#", "B", "C#", "D#", "E", "F#"}},
        {"Amin",  {"A", "B", "C", "D", "E", "F", "G"}},
        {"Bbmin", {"A#", "C", "C#", "D#", "F", "F#", "G#"}},
        {"Bmin",  {"B", "C#", "D", "E", "F#", "G", "A"}},

        //Live tonality
        {"Live", {}}
    };
};