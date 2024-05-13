#pragma once

#include <vector>
#include "Components/Components.h"
#include "IGameObjectController.h"

struct GameObject {
    std::vector<Component*> components; 
    IGameObjectController * controller;


    // Constructor
    GameObject() : controller(nullptr) {}

    // Destructor
    ~GameObject();

    // Member functions to add and get components
    void addComponent(Component* component);

    void setController(IGameObjectController * newController);
    void update();
    
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
