#pragma once
#include "GLib.h"
#include "Components/Components.h"


using namespace GLib;


namespace Physics {
    // Function to update the position and velocity of a game object
    void Update(MovementComponent* movementComponent, ForceComponent* forceComponent, float dt);
    float sweptAABB(MovementComponent* obj1, MovementComponent* obj2);
    void handleCollision(MovementComponent* playerMovement, ForceComponent* playerForce);
}