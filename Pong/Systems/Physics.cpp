#include "Physics.h"
#include <Components/Components.h>
#include <cmath>
#include <random>

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

    void UpdateBall(MovementComponent* movementComponent, float dt) {
        Vector2* position = &movementComponent->position;
        Vector2* velocity = &movementComponent->velocity;
        position->x(position->x() + velocity->x() * dt);
        position->y(position->y() + velocity->y() * dt);
    }

    bool isCollision(MovementComponent * obj1, MovementComponent * obj2) {
        return (obj1->position.x() < obj2->position.x() + obj2->width &&
            obj1->position.x() + obj1->width > obj2->position.x() &&
            obj1->position.y() < obj2->position.y() + obj2->height &&
            obj1->position.y() + obj1->height > obj2->position.y()
            );
    }

    void handleCollision(std::shared_ptr<GameObject> gameObject) {
        if (gameObject->type == GameObjectType::BALL) {
            handleBallCollision(gameObject);
        }
        else {
            MovementComponent* playerMovement = gameObject->getComponent<MovementComponent>();
            ForceComponent* playerForce = gameObject->getComponent<ForceComponent>();
            // first get movement direction
            Vector2 collisionForce = -1 * (playerMovement->velocity);
            // scale it 
            collisionForce *= 50000;
            playerForce->force = playerForce->force + collisionForce;
        }
    }

    void handleBallCollision(std::shared_ptr<GameObject> gameObject) {
        MovementComponent* ballMovement = gameObject->getComponent<MovementComponent>();
        Vector2 position = ballMovement->position;
        if (position.y() <= -265.0 || position.y() >= 265.0) {
            ballMovement->velocity = { ballMovement->velocity.x(), -ballMovement->velocity.y() };
        }
        else {
            ballMovement->velocity = { -ballMovement->velocity.x(), ballMovement->velocity.y() };
        }
    }

    bool isWallCollision(std::shared_ptr<GameObject> gameObject) {
        Vector2 position = gameObject->getComponent<MovementComponent>()->position;
        return (position.x() <= -400.0 || position.x() >= 400.0 || 
            position.y() <= -300.0 || position.y() >= 265.0);

    }

    void handleCollisions(std::vector<std::shared_ptr<GameObject>>& allGameObjects) {
        for (int i = 0; i < allGameObjects.size(); i++) {
            // first check if ball is hitting any wall
            if (allGameObjects[i]->type == GameObjectType::BALL && 
                    isWallCollision(allGameObjects[i])) {
                    handleCollision(allGameObjects[i]);
            }
            // then check for gameobject collisions
            for (int j = i+1; j < allGameObjects.size(); j++) {
                MovementComponent* a = allGameObjects[i]->getComponent<MovementComponent>();
                MovementComponent* b = allGameObjects[j]->getComponent<MovementComponent>();
                if (isCollision(a,b)) {
                    handleCollision(allGameObjects[i]);
                    handleCollision(allGameObjects[j]);
                }
            }
        }
    }

    Vector2 getRandomVector2(float magnitude) {
        // Create a random number generator engine
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distr(0, 2 * 3.14159265); // Uniform distribution for angle

        // Generate a random angle
        float angle = distr(gen);

        // Calculate the components of the vector using polar coordinates
        float x = magnitude * cos(angle);
        float y = magnitude * sin(angle);

        return { x, y };
    }
}