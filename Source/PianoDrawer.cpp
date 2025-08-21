/*
  ==============================================================================

    PianoDrawer.cpp
    Created: 8 Apr 2025 5:39:44pm
    Author:  lgiac

  ==============================================================================
*/

#include "PianoDrawer.h"

PianoDrawer::PianoDrawer(Piano* pia, ToggleButton& pb) : piano(pia), bottonePiano(pb)
{
    areaMatrix = &piano->getAreaCollection();
    setAreasDrawers();
    startTimerHz(5);
}

void PianoDrawer::setAreasDrawers()
{
    areasComponent.clear(true);

    for (auto& row : *areaMatrix)
    {
        for (auto& a : row) {
            auto* areaComp = new AreaDrawer(&a);
            addAndMakeVisible(areaComp);
            areasComponent.add(areaComp);
        }
    }
}

void PianoDrawer::timerCallback() //Non c'eisogno di un aggiornamento continuo credo
{
    if (secondCounter == 3) bottonePiano.setEnabled(true);

    if (!piano->getUpdatedState()) {
        bottonePiano.setEnabled(false);
        secondCounter = 0; //Starto il timer
        setAreasDrawers();
        getParentComponent()->resized();
        resized();

        piano->setUpdated(true);
    }

    ++secondCounter;
}


void PianoDrawer::resized()
{
    int counter = 0;
    auto startOffset = 0.f;

    for (auto& r : *areaMatrix)
    {
        for (auto& a : r)
        {
            areasComponent[counter]->setBounds(a.getAreaBounds());
            ++counter;
        }
    }
}
