#pragma once

#include "GLib.h"
#include "Math/Vector2.h"

using namespace Engine::Math;

// Base Component class
class Component {
public:
    virtual ~Component() {} // Virtual destructor for polymorphic behavior
};


class MovementComponent : public Component {
public:
    MovementComponent(const Vector2& position, const Vector2& velocity) :
        position(position), velocity(velocity) {}

    Vector2 position;
    Vector2 velocity;
};


class ForceComponent : public Component {
public:
    ForceComponent(const Vector2& force) : force(force) {}

    Vector2 force;
};

class SpriteComponent : public Component {
public:
    SpriteComponent::SpriteComponent(GLib::Sprite* sprite) : sprite(sprite) {
    }

    // Destructor
    SpriteComponent::~SpriteComponent() {
        // Release the sprite resource
        if (sprite) {
            GLib::Release(sprite);
            sprite = nullptr;
        }
    }

    GLib::Sprite* sprite;
};