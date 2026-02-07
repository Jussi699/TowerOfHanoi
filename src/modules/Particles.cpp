#include "Particles.h"
#include <algorithm>

void Particles::CreateVictoryParticles(Vector2 position, int count) {
    for (int i = 0; i < count; i++) {
        Particle p;
        p.position = position;
        p.velocity = { (float)(GetRandomValue(-50, 50)) / 10.0f, (float)(GetRandomValue(-50, 50)) / 10.0f };
        p.life = 1.0f;
        p.color = GOLD;
        particles.push_back(p);
    }
}

void Particles::UpdateParticles(float deltaTime) {
    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].position.x += particles[i].velocity.x;
        particles[i].position.y += particles[i].velocity.y;
        particles[i].life -= deltaTime;
    }
    particles.erase(remove_if(particles.begin(), particles.end(), [](const Particle& p) { return p.life <= 0.0f; }), particles.end());
}

void Particles::DrawParticles() const {
    for (const auto& p : particles) {
        DrawCircleV(p.position, 3, Fade(p.color, p.life));
    }
}

void Particles::DrawStar(Vector2 position, float radius, Color color) {
    Vector2 points[10];
    for (int i = 0; i < 10; i++) {
        float angle = i * PI / 5.0f;
        float length = (i % 2 == 0) ? radius : radius / 2.5f;
        points[i] = { position.x + cosf(angle) * length, position.y + sinf(angle) * length };
    }
    for (int i = 0; i < 10; i++) {
        DrawTriangle(points[i], points[(i + 2) % 10], points[(i + 1) % 10], color);
    }
}

