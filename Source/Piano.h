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
    std::array<String, 8>& getNoteInTonality() { return tonality; }
    void setPianoScale(String newScale);
    void setPianoTonic(int newTonic);
    void updatePianoTonality();
    float getDeltaOffset() const { return deltaOffset; }
    float getThreshold() const { return areasThreshold; }
    bool getAutoThreshold() const { return autoThreshold; }

    void setUpdated(bool s) { updated = s;}

    void setScale(String scale_key) //Layout del piano
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
    void buildTonality(int note, String scale);
private:
    const int lato = piano_width;
    Rectangle<int> pianoBoundsInTheScreen{};
    void updatePianoBoundsInTheScreen(int cols, float cellWidth, float deltaOffset);
    float deltaOffset = 0.f;

    void updatePiano();
    void resetPiano();

    String currentScale = "Major";
    int currentTonic = 0; //C
    String scale;
    int low_oct, high_oct;
    int numberOfNotes;
    float areasThreshold = 1.f;
    bool autoThreshold = true;
    std::vector<std::vector<Area>> areas_collection;

    bool updated = true;

    std::array<String, 8> tonality;
    std::array<String, 12> notes = { "C", "C#", "D", "D#","E", "F", "F#", "G", "G#", "A", "A#", "B" };
    std::map<String, std::vector<int>> SCALE_PATTERNS = {
        {"Major", {2,2,1,2,2,2,1}},
        {"Major 7th", {4,3,4,1}},
        {"Major Dominant", {4,3,3,2}},
        {"Major 6th", {4,3,2,3}},
        {"Major Augmented", {2,2,1,3,1,2,1}},
        {"Minor", {2,1,2,2,1,2,2}},
        {"Minor 7th", {3,4,3,2}},
        {"Minor Major 7th", {3,4,4,1}},
        {"Minor 6th", {3,4,2,3}},
        {"Blues", {3,2,1,1,3,2}},
        {"Dorian", {2,1,2,2,2,1,2}},
        {"Mixolydian", {2,2,1,2,2,1,2}},
        {"Phrygian", {1,2,2,2,1,2,2}},
        {"Lydian", {2,2,2,1,2,2,1}},
        {"Locrian", {1,2,2,1,2,2,2}},
        {"Harmonic Minor", {2,1,2,2,1,3,1}},
        {"Melodic Minor", {2,1,2,2,2,2,1}},
        {"Pentatonic Neutral", {2,3,2,3,2}},
        {"Pentatonic Minor", {3,2,2,3,2}},
        {"Pentatonic Major", {2,2,3,2,3}},
        {"Dim Half", {1,2,1,2,1,2,1,2}},
        {"Dim Whole", {2,1,2,1,2,1,2,1}},
        {"Augmented", {3,1,3,1,3,1}},
        {"Roumanian Minor", {2,1,3,1,2,1,2}},
        {"Spanish - Gypsy", {1,3,1,2,1,2,2}},
        {"Double Harmonic", {1,3,1,2,1,3,1}},
        {"Eight-Tone Spanish", {1,2,1,1,1,2,2,2}},
        {"Enigmatic", {1,3,2,2,2,1,1}},
        {"Algerian", {2,1,3,1,1,2,2}},
        {"Balinese", {1,2,4,1,4}},
        {"Hirajoshi", {2,1,4,1,4}},
        {"Hungarian Gypsy", {2,1,3,1,1,3,1}},
        {"Japanese", {1,4,2,1,4}},
        {"Persian", {1,3,1,1,2,3,1}},
        {"Prometheus", {2,2,2,3,1,2}},
        {"Six-Tone Symetrical", {2,1,2,1,2,4}},
        {"Super Locrian", {1,2,1,2,2,2,2}},
        {"Wholetone", {2,2,2,2,2,2}},
        {"Half Diminished", {2,1,2,1,2,2,2}},
        {"Live", {}}
    };
};