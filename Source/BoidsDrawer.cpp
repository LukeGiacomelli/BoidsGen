/*
  ==============================================================================

    boidsDrawer.cpp
    Created: 9 Apr 2025 12:51:35am
    Author:  lgiac

  ==============================================================================
*/

#include "BoidsDrawer.h"

BoidsDrawer::BoidsDrawer(std::vector<std::unique_ptr<Boids>>& b, int& _activeBoids, Colour& boidsColor, float& bs)
    : activeBoids(_activeBoids), bc(boidsColor), boids_size(bs)
{
    for (auto& e: b ) 
        boids.push_back(e.get()); 
}

void BoidsDrawer::paint(Graphics& g)
{
    for (int i = 0; i < activeBoids; ++i) 
    {
        g.setColour(bc);
        g.fillEllipse(boids[i]->getPosition().x(), boids[i]->getPosition().y(), boids_size, boids_size);
    }
}

void BoidsDrawer::timerCallback()
{
    repaint();
}

void BoidsDrawer::resized()
{
    startTimerHz(30);
}
