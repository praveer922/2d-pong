#pragma once


struct GameObject;

class IGameObjectController {
public:
    virtual ~IGameObjectController() {}

    virtual void update(GameObject& gameObject) = 0; // Pure virtual function for updating the controller
};