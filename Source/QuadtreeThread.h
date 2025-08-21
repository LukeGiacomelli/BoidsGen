#pragma once

#include <JuceHeader.h>
#include "Boids.h"
#include "Quadtree.h"

class QuadtreeThread : public juce::Thread
{
public:
    QuadtreeThread(std::vector<std::unique_ptr<Boids>>& boidsRef, std::atomic<Quadtree*>& activeRef,Quadtree& qtA, Quadtree& qtB, int& _activeBoids) 
        : juce::Thread("QuadtreeThread"), boids(boidsRef), activeQt(activeRef), qt1(qtA), qt2(qtB), activeBoids(_activeBoids) {}

    void run() override
    {
        while (!threadShouldExit())
        {
            Quadtree* inactive = (activeQt.load() == &qt1) ? &qt2 : &qt1;

            inactive->clear();
            for (int i = 0; i < activeBoids; ++i)
                inactive->insert(boids[i].get());

            activeQt.store(inactive);

            wait(33); //millisecondi
        }
    }

private:
    int& activeBoids;
    std::vector<std::unique_ptr<Boids>>& boids;
    std::atomic<Quadtree*>& activeQt;
    Quadtree& qt1;
    Quadtree& qt2;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QuadtreeThread)
};