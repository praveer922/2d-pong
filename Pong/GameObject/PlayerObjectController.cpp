#include "PlayerObjectController.h"
#include "GLib.h"
#include "Systems/Timing.h"
#include "Systems/Physics.h"
#include "GameObject.h"
#include "Console/ConsolePrint.h"

PlayerObjectController::PlayerObjectController() : forceApplied({0.0f,0.0f}) {
    GLib::SetKeyStateChangeCallback(std::bind(&PlayerObjectController::KeypressHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void PlayerObjectController::update(GameObject& gameObject) {
    // Implement movement functionality for the player GameObject
	ForceComponent * forceComponent = gameObject.getComponent<ForceComponent>();
	forceComponent->force = forceComponent->force + forceApplied;
	float dt = Timing::getDeltaTimeSinceLastCall();
	Physics::Update(gameObject.getComponent<MovementComponent>(), gameObject.getComponent<ForceComponent>(), dt);
	forceApplied = { 0.0f, 0.0f };
}

void PlayerObjectController::KeypressHandler(unsigned int i_VKeyID, bool i_bDown)
	{
		if (i_VKeyID == 87) { // W key
			forceApplied = { 0,360000.0f };
		}
		else if (i_VKeyID == 65) { // A key
			forceApplied = { -360000.0f , 0 };
		}
		else if (i_VKeyID == 83) { // S key
			forceApplied = { 0, -360000.0f };
		}
		else if (i_VKeyID == 68) { // D 
			forceApplied = { 360000.0f , 0 };
		}
	}
