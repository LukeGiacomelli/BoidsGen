#pragma once

#include <JuceHeader.h>
#include "Boids.h"

#define MAX_LVL 13
#define CAPACITY 8

class Quadtree
{
public:
	Quadtree(Rectangle<float> b, int _depth = 0)
		: bounds(b), depth(_depth)
	{ }

	~Quadtree() 
	{
		delete one;
		delete two;
		delete three;
		delete four;
	}

	bool insert(Boids* bo);
	void subdivide();
	void clear();
	void query(Rectangle<float> view, std::vector<Boids*>& b) const;


private:
	static const int capacity = CAPACITY;
	int depth = 0;

	const Rectangle<float> bounds;

	std::vector<Boids*> boids;

	bool divided = false;

	Quadtree* one = nullptr;
	Quadtree* two = nullptr;
	Quadtree* three = nullptr;
	Quadtree* four = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Quadtree)
};