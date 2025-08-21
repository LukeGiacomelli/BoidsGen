/*
  ==============================================================================

    boidsDrawer.cpp
    Created: 9 Apr 2025 12:51:35am
    Author:  lgiac

  ==============================================================================
*/

#include "BoidsDrawer.h"

#define boid_size 7.5f

BoidsDrawer::BoidsDrawer(std::vector<std::unique_ptr<Boids>>& b, int& _activeBoids)
    : activeBoids(_activeBoids)
{
    boidImage = juce::ImageCache::getFromMemory(BinaryData::boid_png, BinaryData::boid_pngSize);
    for (auto& e: b ) 
        boids.push_back(e.get()); 
}

void BoidsDrawer::paint(Graphics& g)
{
    for (int i = 0; i < activeBoids; ++i) 
    {
        //g.setColour(b->getColour());
        //g.fillEllipse(b->getPosition().x(), b->getPosition().y(), boid_size, boid_size);
        g.drawImage(boidImage,Rectangle<float>{ boids[i]->getPosition().x(), boids[i]->getPosition().y(), boids[i]->getSize(), boids[i]->getSize()});
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
