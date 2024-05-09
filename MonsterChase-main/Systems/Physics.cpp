#include "Physics.h"
#include <Components/Components.h>

namespace Physics {

    // This updates the entity's position and velocity if there are any forces present on it.
    void Update(MovementComponent * movementComponent, ForceComponent * forceComponent, float dt) {
            Vector2* position = &movementComponent->position;
            Vector2* velocity = &movementComponent->velocity;
            Vector2* force = &forceComponent->force;

            // Using Euler integration
            // Update velocity
            velocity->x(velocity->x() + (force->x() / 1.0f) * dt);
            velocity->y(velocity->y() + (force->y() / 1.0f) * dt);

            position->x(position->x() + velocity->x() * dt);
            position->y(position->y() + velocity->y() * dt);

            // now that velocity has been updated, set drag as the only force remaining on the object
            float drag_x = -0.03 * (velocity->x() * velocity->x());
            if (velocity->x() < 0) {
                drag_x = -drag_x;
            }
            float drag_y = -0.03 * (velocity->y() * velocity->y());
            if (velocity->y() < 0) {
                drag_y = -drag_y;
            }

            force->x(drag_x);
            force->y(drag_y);
    }

    // collision detection: returns time till objects collide, or -1 if they don't
    float sweptAABB(MovementComponent * obj1, MovementComponent * obj2) {
        float dxEntry, dxExit;
        float dyEntry, dyExit;

        if (obj1->velocity.x() > 0.0f) {
            dxEntry = obj2->position.x() - (obj1->position.x() + obj1->width);
            dxExit = (obj2->position.x() + obj2->width) - obj1->position.x();
        }
        else {
                dxEntry = (obj2->position.x() + obj2->width) - obj1->position.x();
                dxExit = obj2->position.x() - (obj1->position.x() + obj1->width);
        }

        if (obj1->velocity.y() > 0.0f) {
            dyEntry = obj2->position.y() - (obj1->position.y() + obj1->height);
            dyExit = (obj2->position.y() + obj2->height) - obj1->position.y();
        }
        else {
            dyEntry = (obj2->position.y() + obj2->height) - obj1->position.y();
            dyExit = obj2->position.y() - (obj1->position.y() + obj1->height);
        }

        float txEntry, txExit;
        float tyEntry, tyExit;

        if (obj1->velocity.x() == 0.0f) {
            txEntry = -std::numeric_limits<float>::infinity();
            txExit = std::numeric_limits<float>::infinity();
        }
        else {
            txEntry = dxEntry / obj1->velocity.x();
            txExit = dxExit / obj1->velocity.x();
        }

        if (obj1->velocity.y() == 0.0f) {
            tyEntry = -std::numeric_limits<float>::infinity();
            tyExit = std::numeric_limits<float>::infinity();
        }
        else {
            tyEntry = dyEntry / obj1->velocity.y();
            tyExit = dyExit / obj1->velocity.y();
        }

        float entryTime = max(txEntry, tyEntry);
        float exitTime = min(txExit, tyExit);

        if (entryTime < 0.0f || exitTime < 0.0f || entryTime > exitTime) {
            return -1;
        }
        else {
            return entryTime;
        }
    }
}