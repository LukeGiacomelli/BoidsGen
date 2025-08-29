/*
  ==============================================================================

    Quadtree.cpp
    Created: 25 May 2025 9:15:48pm
    Author:  lgiac

  ==============================================================================
*/

#include "Quadtree.h"

bool Quadtree::insert(Boids* bo)
{
    auto bo_position = bo->getPosition();
    if (!bounds.contains(bo_position.x(), bo_position.y())) return false;

    if (boids.size() < capacity || depth >= MAX_LVL)
    {
        boids.push_back(bo);
    }
    else
    {
        if (!divided && depth < MAX_LVL)
        {
            subdivide();
        }

        if (one->insert(bo)) return true;
        else if (two->insert(bo)) return true;
        else if (three->insert(bo)) return true;
        else if (four->insert(bo)) return true;

    }

    return false;
}


void Quadtree::subdivide()
{
    Rectangle<float> new_bounds = { bounds.getX(), bounds.getY(), bounds.getWidth() / 2, bounds.getHeight() / 2 };

    one = new Quadtree(new_bounds,depth+1);
    two = new Quadtree(new_bounds.translated(new_bounds.getWidth(),0.f), depth + 1);
    three = new Quadtree(new_bounds.translated(0.f, new_bounds.getHeight()), depth+1);
    four = new Quadtree(new_bounds.translated(new_bounds.getWidth(), new_bounds.getHeight()), depth+1);

    divided = true;
}

void Quadtree::clear() {
    boids.clear();

    if (divided) {
        one->clear();
        two->clear();
        three->clear();
        four->clear();

        delete one;
        delete two;
        delete three;
        delete four;

        one = nullptr;
        two = nullptr;
        three = nullptr;
        four = nullptr;

        divided = false;
    }
}


void Quadtree::query(Rectangle<float> view, std::vector<Boids*>& n) const
{

    if (!bounds.intersectRectangle(view)) return;

    for (auto* b : boids) 
    {
        if (view.contains(b->getPosition().x(), b->getPosition().y()))
            n.push_back(b);
    }

    //Se ha suddivisioni cerco anche nei figli
    if(divided)
    {
        one->query(view, n);
        two->query(view, n);
        three->query(view, n);
        four->query(view, n);
    }
}

