#pragma once
#include "GLib.h"
#include "Components/Components.h"


using namespace GLib;


namespace Physics {
    // Function to update the position and velocity of a game object
    void Update(MovementComponent* movementComponent, ForceComponent* forceComponent, float dt);
}