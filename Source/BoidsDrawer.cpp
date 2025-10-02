/*
  ==============================================================================

    boidsDrawer.cpp
    Created: 9 Apr 2025 12:51:35am
    Author:  lgiac

  ==============================================================================
*/

#include "BoidsDrawer.h"

BoidsDrawer::BoidsDrawer(std::vector<std::unique_ptr<Boids>>& b, int& _activeBoids, Colour& boidsColor)
    : activeBoids(_activeBoids), bc(boidsColor)
{
    //boidImage = juce::ImageCache::getFromMemory(BinaryData::boid_png, BinaryData::boid_pngSize);
    //boidImage = boidImage.rescaled(b[0]->getSize(), b[0]->getSize(), Graphics::mediumResamplingQuality);

    for (auto& e: b ) 
        boids.push_back(e.get()); 
}

void BoidsDrawer::paint(Graphics& g)
{
    for (int i = 0; i < activeBoids; ++i) 
    {
        g.setColour(bc);
        //g.drawImageAt(boidImage,boids[i]->getPosition().x(), boids[i]->getPosition().y(),false);
        g.fillEllipse(boids[i]->getPosition().x(), boids[i]->getPosition().y(), 7.f, 7.f);
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
