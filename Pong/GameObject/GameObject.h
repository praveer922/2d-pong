#pragma once

#include <vector>
#include "Components/Components.h"
#include "IGameObjectController.h"

enum class GameObjectType {
    PLAYER1,
    PLAYER2,
    BALL
};

struct GameObject {
    std::vector<Component*> components; 
    IGameObjectController * controller;
    GameObjectType type;


    // Constructor
    GameObject(GameObjectType objType) : controller(nullptr), type(objType) {}

    // Destructor
    ~GameObject();

    // Member functions to add and get components
    void addComponent(Component* component);

    void setController(IGameObjectController * newController);
    void update(float dt);
    
    // Member function to get a component by type
    template<typename T>
    T* getComponent() const {
        for (Component* component : components) {
            if (T* target = dynamic_cast<T*>(component)) {
                return target;
            }
        }
        return nullptr;
    }

};
