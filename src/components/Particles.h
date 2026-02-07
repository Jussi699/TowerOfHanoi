#pragma once

#ifndef PARTICLES_H
#define PARTICLES_H

#include "Music.h"
#include "BaseClass.h"
#include "DiskStruct.h"
#include <cmath>
#include <vector>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
};

class Particles : public BaseClass
{
public:
    void CreateVictoryParticles(Vector2 position, int count);
    void DrawStar(Vector2 position, float radius, Color color);
    void UpdateParticles(float deltaTime);
    void DrawParticles() const;
   
private:
     std::vector<Particle> particles;
};

#endif