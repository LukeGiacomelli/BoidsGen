/*
  ==============================================================================

    PianoDrawer.h
    Created: 8 Apr 2025 5:39:44pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Piano.h"
#include "AreaDrawer.h"

class PianoDrawer : public Component, public Timer {
public:
    PianoDrawer(Piano* pia, ToggleButton& pb);
    void timerCallback() override;
    void resized() override;
private:
    Piano* piano;
    OwnedArray<AreaDrawer> areasComponent;
    std::vector<std::vector<Area>>* areaMatrix;
    void setAreasDrawers();
    ToggleButton& bottonePiano;

    int secondCounter = 2;
};