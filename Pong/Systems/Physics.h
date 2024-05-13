#pragma once
#include "GLib.h"
#include "Components/Components.h"
#include <vector>
#include <memory>
#include "GameObject/GameObject.h"


using namespace GLib;


namespace Physics {
    // Function to update the position and velocity of a game object
    void Update(MovementComponent* movementComponent, ForceComponent* forceComponent, float dt);
    bool isCollision(MovementComponent* obj1, MovementComponent* obj2);
    void handleCollisions(std::vector<std::shared_ptr<GameObject>>& allGameObjects);
    void handleCollision(MovementComponent* playerMovement, ForceComponent* playerForce);
}