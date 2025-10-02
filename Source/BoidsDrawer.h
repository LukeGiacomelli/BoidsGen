/*
  ==============================================================================

    BoidsDrawer.h
    Created: 9 Apr 2025 12:51:35am
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Boids.h"

class BoidsDrawer : public Component, public Timer
{
public:
    BoidsDrawer(std::vector<std::unique_ptr<Boids>>& b, int& _activeBoids, Colour& boidsColor);
    ~BoidsDrawer() {};

    void paint(Graphics& g) override;
    void timerCallback() override;
    void resized() override;
private:
    std::vector<Boids*> boids;
    Colour& bc;
    Image boidImage;
    int& activeBoids;
};