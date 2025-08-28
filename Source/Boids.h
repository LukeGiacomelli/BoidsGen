/*
  ==============================================================================

    Boids.h
    Created: 8 Apr 2025 4:43:31pm
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Area.h"
#include "MidiManager.h"
#include "Eigen/Dense"

using namespace Eigen;

class Boids 
{
public:
    Boids(Rectangle<int>& s, float& default_speed_source, float& max_speed_source, float& fov_source);
    ~Boids() {};

    //Rules
    Vector2f alignment(const std::vector<Boids*>& b);
    Vector2f separation(const std::vector<Boids*>& b);
    Vector2f cohesion(const std::vector<Boids*>& b);
    Vector2f avoidNonTonality(const Vector2f pianoPosition,const std::array<String,7>& notes, const std::vector<std::vector<Area>>& aree);
    Vector2f followTonality(const Vector2f pianoPosition,const std::array<String,7>& notes, const std::vector<std::vector<Area>>& aree, bool live);
    void alignementBias(const float probability);
    void checkForNotes(const Vector2f pianoPosition, std::vector<std::vector<Area>>& aree, float now);
    

    //Physics
    void update();
    void applyForce(const Vector2f& force);
    bool borders();
    float angle(const Vector2f& v);
    bool inTheFov(Vector2f current_direction, float rangeOfView);
    Vector2f limitVector(Vector2f vector);

    //Set
    void setPosition(Vector2f pos) { position = pos; }
    void setVelocity(Vector2f vel) { velocity = vel; }
    void setAcceleration(Vector2f acc) { acceleration = acc; }

    //Get
    Vector2f getPosition() const { return position; }
    Vector2f getVelocity() const { return velocity; }
    Vector2f getAcceleration() const { return acceleration; }
    Colour getColour() const { return colour; }
    float getRange() const { return range; }
    float getWeight() const { return weight; }
    float getSize() const { return size; }

private:
    juce::Random rnd;
    
    Rectangle<int>& screenBounds = Rectangle<int>(); //sus

    float distance(Vector2f v1, Vector2f v2);
    float& default_speed;
    Vector2f position;
    Vector2f velocity;
    Vector2f acceleration;

    float& maxSpeed;
    float maxForce;

    float weight;
    float& range;
    Colour colour;
    float size;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Boids)
};