/*
  ==============================================================================

    Area.h
    Created: 8 Apr 2025 3:22:01pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Note.h"
#include "MidiManager.h"

#define PESO_THRESH 0.17 //Percentuale rispetto al totale dei boidi

class Area {
public:
    Area(String ch, int oct, float w, MidiManager& midi, int& n_boids, float& thr, bool& autoThr) 
        : nota( ch, oct ), peso(w), midiManager(midi), numberOfBoids(n_boids), externalThreshold(thr), autoThreshold(autoThr){};
    ~Area() {};

    Note getNota() const { return nota; }
    bool getUsed() const { return used; }
    float getPeso() const { return peso; }
    float getTresh() const { return threshold; }
    Rectangle<int> getAreaBounds() const { return areaBounds; }
    bool getInSelectedTonality() const { return inSelectedTonality; }
    bool getActivated() const { return activated; }

    void setUsed(bool u) { used = u; }
    void setNota(String ch, int oct) { nota.setChroma(ch); nota.setOctave(oct); }
    void setInSelectedTonality(bool in) { inSelectedTonality = in; }
    void setPeso(float w, float now) 
    { 
        //Threshold automatiche o meno...
        if (autoThreshold)
            threshold = std::max(0.5f, std::min(numberOfBoids * 0.07f, 125.f)); //5% dei boidi totali o ...
        else
            threshold = externalThreshold;

        if (w <= threshold * 2) 
            peso = w;
        else
            peso = threshold;

        if (peso <= 0)
        {
            lastSleep = now;

            if (activated == true) 
            {
                midiManager.addNote(nota.getMidiNote(), false, 0.f);
                activated = false;
            }
        }
    }
    void setAreaBounds(int x, int y, int width, int height) { areaBounds.setBounds(x, y, width, height); }

    void checkPeso(float now) 
    {
        auto rested = now - lastActivated;
        if (peso >=  threshold && !activated && rested >= 0.1f)
        {
            auto chargedIn = now - lastSleep;
            auto velocity = jmax(1.1f - jmap<float>(chargedIn, 0.f, 1.f, 0.1f, 1.f),0.1f);
            midiManager.addNote(nota.getMidiNote(), true, velocity); 
            activated = true;
            lastActivated = now;
        }
    }

private:
    Rectangle<int> areaBounds{ 0, 0, 0, 0 };
    Note nota;
    bool inSelectedTonality = false;
    float peso;
    float& externalThreshold;
    float threshold;
    bool& autoThreshold;
    int& numberOfBoids;

    float lastSleep = 0;
    float lastActivated = 0;

    bool activated = false;
    bool used = false;

    MidiManager& midiManager;
};