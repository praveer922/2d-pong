#pragma once

#include "IGameObjectController.h"
#include "Math/Vector2.h"

using namespace Engine::Math;

class PlayerObjectController : public IGameObjectController {
public:
    PlayerObjectController(); // Constructor

    // Override the update function
    virtual void update(GameObject& gameObject) override;

private:
    void KeypressHandler(unsigned int i_VKeyID, bool i_bDown);
    Vector2 forceApplied;
};