/*
  ==============================================================================

    AreaDrawer.h
    Created: 8 Apr 2025 5:38:58pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Area.h"

class AreaDrawer : public Component{
public:
    AreaDrawer(Area* a) : area(a)
    {
        areaInfoNota = area->getNota().getChroma() + String(area->getNota().getOctave());
        areaInfoPeso = String(area->getTresh());
    }
    ~AreaDrawer() {};
    void paint(Graphics& g) override;
    Area* getArea() { return area; }
private:
    Area* area;

    String areaInfoNota;
    String areaInfoPeso;
    bool inSelectedTonality = false;
    Rectangle<float> sfondoNote;
    Rectangle<float> inTonalityIndicatorPos;

    int iteration_counter = 0;
};