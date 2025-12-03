/*
  ==============================================================================

    Boids.cpp
    Created: 8 Apr 2025 4:43:31pm
    Author:  lgiac

  ==============================================================================
*/

#include "Boids.h"

#define PI 3.141592635
#define max_force 0.8f
#define default_turning_factor 0.07
#define alpha_turn 5
#define boids_jitter 0.2
#define separation_range 17

Boids::Boids(Rectangle<int>& s, float& default_speed_source, float& max_speed_source, float& range_source)
    : screenBounds(s), default_speed(default_speed_source), maxSpeed(max_speed_source), range(range_source)
{
    //Inizializzo le posizioni dei boidi
    float rnd_x = screenBounds.getX() + rnd.nextFloat() * screenBounds.getWidth();
    float rnd_y = screenBounds.getY() + rnd.nextFloat() * screenBounds.getHeight();
    position = Eigen::Vector2f(rnd_x, rnd_y);

    weight = 1;
    acceleration = Eigen::Vector2f(0, 0);
    velocity = Eigen::Vector2f(rnd.nextFloat() * 2 - 1, rnd.nextFloat() * 2 - 1) * default_speed;
    maxForce = max_force;
    size = 7.5f;
}

Vector2f Boids::alignment(const std::vector<Boids*>& b)
{
    Vector2f avgDirection = velocity;
    float avgVelocity = velocity.norm();

    int number_of_friends = 1;
    
    float dist;
    for (auto& a : b) {
        dist = distance(a->getPosition(), position); 
        if (dist > 0) {
            avgDirection += a->getVelocity();
            avgVelocity += a->getVelocity().norm();
            ++number_of_friends;
        }
    }

    if (number_of_friends > 1) {
        avgDirection /= number_of_friends;
        avgVelocity /= number_of_friends;
        avgDirection.normalize();
        avgDirection *= maxSpeed;
        avgDirection -= velocity;
        avgDirection = limitVector(avgDirection);
        return avgDirection;
    }
    else
    {
        return Vector2f(0, 0);
    }
}

Vector2f Boids::separation(const std::vector<Boids*>& b)
{
    Vector2f steer (0,0);
    int number_of_collision = 0;

    for (auto& a : b) {
        auto dist = distance(a->getPosition(), position);
        if (dist > 0 && dist < separation_range) {
            auto dist = distance(a->getPosition(), position);
            Vector2f diff = position - a->getPosition();
            //diff.normalize();
            diff /= dist; //Divido in modo da ottenere per distanze minori diff maggiori!
            steer += diff;
            ++number_of_collision;
        }
    }

    if (number_of_collision > 0) steer /= number_of_collision; //Media
    if (auto norm = steer.norm() > 0) {
        steer.normalize();
        steer *= maxSpeed;
        steer /= norm;
        steer -= velocity;
        steer = limitVector(steer);
    }

    return steer;
}

Vector2f Boids::cohesion(const std::vector<Boids*>& b)
{
    Vector2f steer(0, 0);
    int number_of_friends = 1; //Includo già me stesso nel gruppo
    Vector2f center_of_mass = position;

    for (auto& a : b) {
        auto dist = distance(a->getPosition(), position);
        if (dist > 0) {
            center_of_mass += a->getPosition();
            ++number_of_friends;
        }
    }

    if(number_of_friends>1) 
    {
        center_of_mass /= number_of_friends;
        center_of_mass = position - center_of_mass;
        steer -= center_of_mass;
        steer.normalize();
        steer *= maxSpeed;
        steer -= velocity;
        steer = limitVector(steer);
    }

    return steer;
}

Vector2f Boids::avoidNonTonality(Vector2f pianoPosition, const std::array<String,8>& notes, const std::vector<std::vector<Area>>& aree)
{
    Vector2f steer(0, 0);
    int number_of_collision = 0;

    for (auto& c : aree) {
        for (auto& a : c) {
            if (!a.getUsed()) continue;

            Vector2f aPos(pianoPosition.x() + a.getAreaBounds().getX() + (a.getAreaBounds().getWidth()/2), pianoPosition.y() + a.getAreaBounds().getY() + (a.getAreaBounds().getHeight()/2));
            auto dist = distance(aPos, position);

            if ((dist > 0 && dist < (a.getAreaBounds().getWidth() * 0.5))) {
                if (std::find(notes.begin(),notes.end(), a.getNota().getChroma()) != notes.end()) continue;
                Vector2f diff = position - aPos;
                diff.normalize();
                diff /= dist;
                steer += diff;
                ++number_of_collision;
            }
        }
    }

    if (number_of_collision > 0) steer /= number_of_collision; //Media

    auto norm = steer.norm();
    if (norm > 0) {
        steer.normalize();
        steer *= maxSpeed;
        steer /= norm;
        steer -= velocity;
        steer = limitVector(steer);
    }

    return steer;
}

Vector2f Boids::followTonality(const Vector2f pianoPosition, const std::array<String, 8>& notes, const std::vector<std::vector<Area>>& aree, String scaleName)
{
    Vector2f nearest_area{0,0};
    auto nearest_area_dist = 10000.f;
    String nearest_area_chroma = "";

    const int tonic = 0;
    const int dominant = scaleName == "Major" || scaleName == "Minor" ? 4 : tonic; // La dominante ha senso solo con maj e min

    for (const auto& c : aree) {
        for (const auto& a : c) {
            if (!a.getUsed()) continue;
            Vector2f aPos(pianoPosition.x() + a.getAreaBounds().getX() + (a.getAreaBounds().getWidth() / 2), pianoPosition.y() + a.getAreaBounds().getY() + (a.getAreaBounds().getHeight() / 2));
            
            auto dist = distance(aPos, position);
            if (dist > 0 && dist < (a.getAreaBounds().getWidth() / 3) + (scaleName == "Live" ? range * 5 : range)) { //Se live, range maggiore
                if (std::find(notes.begin(), notes.end(), a.getNota().getChroma()) == notes.end()) continue;

                Vector2f diff = aPos - position;
                dist /= a.getNota().getChroma() == notes[tonic] ? 2.5 : a.getNota().getChroma() == notes[dominant] ? 1.5 : 1; //Se è la tonica (o dominante) aumento il peso
                if (dist < nearest_area_dist)  
                {
                    nearest_area_dist = dist;
                    nearest_area = diff;
                    nearest_area_chroma = a.getNota().getChroma();
                }
            }
        }
    }

    if (nearest_area != Vector2f{0, 0}) {
        nearest_area.normalize();
        nearest_area *= maxSpeed;
        nearest_area -= velocity;
        nearest_area *= nearest_area_dist;
        nearest_area *= nearest_area_chroma == notes[tonic] ? 5 : nearest_area_chroma == notes[dominant] ? 3 : 1; //Fattore moltiplicativo per tonica e dominante
        nearest_area = limitVector(nearest_area);
    }

    return nearest_area;
}

void Boids::alignementBias(float probability) 
{
    if (rnd.nextFloat() < probability) 
    {
        //Variazioni casuali tra -jitterAmount e +jitterAmount
        float varAmount = boids_jitter;

        float varX = (rnd.nextFloat() * 2.0f - 1.0f) * varAmount;
        float varY = (rnd.nextFloat() * 2.0f - 1.0f) * varAmount;

        velocity += limitVector(Vector2f(varX, varY) * maxSpeed);
    }
}

void Boids::checkForNotes(const Vector2f pianoPosition, std::vector<std::vector<Area>>& aree, float now)
{
    for (auto& c : aree) {
        for (auto& a : c) {
            if (!a.getUsed()) continue;

            Point<float> aPos(pianoPosition.x() + a.getAreaBounds().getX() + (a.getAreaBounds().getWidth() / 2), pianoPosition.y() + a.getAreaBounds().getY() + (a.getAreaBounds().getHeight() / 2));
            auto dist = distance(Vector2f{ aPos.x, aPos.y }, position);

            if (dist > 0 && dist < (a.getAreaBounds().getWidth() * 0.25))
            { 
                a.setPeso(a.getPeso() + weight, now);
                a.checkPeso(now);
            }
        }
    }
}

void Boids::update()
{
    acceleration *= .6;
    auto magnitude = velocity.norm(); //pitagora
    velocity += acceleration;

    if (magnitude > maxSpeed) velocity = { velocity.x() / magnitude, velocity.y() / magnitude }; //Limite massimo
    if (magnitude < default_speed) velocity = velocity.normalized() * default_speed; //Limite minimo

    position += velocity;
    acceleration *= 0;
}

void Boids::applyForce(const Vector2f& force)
{
    acceleration += force;
}

bool Boids::borders()
{
    bool avoiding = false;

    if (screenBounds != Rectangle<int>())
    {
        auto turning_factor = default_turning_factor;
        auto left = screenBounds.getX();
        auto top = screenBounds.getY();
        auto right = screenBounds.getX() + screenBounds.getWidth();
        auto bottom = screenBounds.getY() + screenBounds.getHeight() - 10;

        if (position.x() < left) 
        {
            avoiding = true;
            turning_factor *= 1 + ( alpha_turn * (left - position.x()) / left); //Più si allontana dal bordo più il turning factor aumenta
            velocity = { velocity.x() + turning_factor, velocity.y() };
        }
        if (position.y() < top)
        {
            avoiding = true;
            turning_factor *= 1 + ( alpha_turn * (top - position.y()) / top);
            velocity = { velocity.x(), velocity.y() + turning_factor };
        }
        if (position.x() > right)
        {
            avoiding = true;
            turning_factor *= 1 + ( alpha_turn * (position.x() - right) / right);
            velocity = { velocity.x() - turning_factor , velocity.y() };
        }
        if (position.y() > bottom) 
        {
            avoiding = true;
            turning_factor *= 1 + ( alpha_turn * (position.y() - (bottom)) / bottom);
            velocity = { velocity.x(), velocity.y() - turning_factor };
        }

        //if (!avoiding) colour = Colour((uint8)121, (uint8)183, (uint8)145, jmap<uint8>(weight, 0, 5, 200, 255));
        //else colour = Colours::red;
    }

    return avoiding;
}

float Boids::angle(const Vector2f& v)
{
    return std::atan2(-v.y(),v.x()); 
}

//bool Boids::inTheFov(Vector2f current_position, float rangeOfView) //Da sistemare
//{
//    auto my_front = angle(velocity);
//    auto angle_of_collision = angle({ current_position.x() - position.x(), current_position.y() - position.y() });
//
//    //Differenza fra gli angoli e normalizzo per gestire il punto di wrap-around
//    float diff = std::fabs(my_front - angle_of_collision);
//    diff = std::fmod(diff, 2 * PI);
//    diff = diff > PI ? 2 * PI - diff : diff;
//
//    return diff < rangeOfView;
//}

Vector2f Boids::limitVector(Vector2f vector)
{
    if (vector.norm() > maxForce)
     return Vector2f{ vector.x() / vector.norm(), vector.y() / vector.norm() };

    return vector;
}

float Boids::distance(Vector2f v1, Vector2f v2) 
{
    auto dx = v1.x() - v2.x();
    auto dy = v1.y() - v2.y();

    return std::sqrt(dx * dx + dy * dy);
}

