/*
  ==============================================================================

    AreaDrawer.cpp
    Created: 8 Apr 2025 5:39:35pm
    Author:  lgiac

  ==============================================================================
*/

#include "AreaDrawer.h"

void AreaDrawer::paint(Graphics& g)
{   
    /*
        Paint viene richiamato alla velocità con cui aggiorno il piano dei boidi,
        sicuramente eccessiva per aggiornarci il piano.
    */

    if (area->getNota().getChroma() != "/") { //Stampo solo le aree che utilizzo
        if (iteration_counter % 25 == 0)
        {
            inSelectedTonality = area->getInSelectedTonality();
            inTonalityIndicatorPos = {9.f + getLocalBounds().getX() + getLocalBounds().getWidth() / 2, 3.f + getLocalBounds().getY() + getLocalBounds().getHeight() / 2, 8.f, 8.f };
            sfondoNote = { (float)getLocalBounds().getX(), (float)getLocalBounds().getY(), (float)getLocalBounds().getWidth(), (float)getLocalBounds().getHeight() };
        }

        //Sfondo aree note
        g.setColour(Colours::transparentWhite.withAlpha(0.f));
        g.fillEllipse(sfondoNote);

        //Bordo area nota
        //g.setColour(Colour(74, 88, 89));
        //g.drawEllipse(getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getWidth(), getLocalBounds().getHeight(), 0.9f);

        //Active notes
        g.setColour(inSelectedTonality ? Colour(0xffffc759).withAlpha(0.3f) : Colour(0xffffc759).withAlpha(0.f));
        g.fillEllipse(inTonalityIndicatorPos);

        //Nota label
        g.setColour(area->getActivated() ? Colour(0xFFB48291) : Colour(0xff32373b).darker(0.1f).withAlpha(0.7f));
        g.setFont(juce::FontOptions(12.0f + (area->getActivated() * 2), Font::bold));
        g.drawFittedText(areaInfoNota, getLocalBounds(), juce::Justification::centred, 1);
        //g.setFont(juce::FontOptions(7.0f + area->getActivated()));
        //g.drawFittedText(areaInfoPeso, Rectangle<int>(0, 10, getLocalBounds().getWidth(), getLocalBounds().getHeight()), juce::Justification::centred, 1);

        ++iteration_counter;
    }
}
