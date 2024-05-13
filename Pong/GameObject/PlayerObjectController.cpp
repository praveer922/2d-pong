#include "PlayerObjectController.h"
#include "GLib.h"
#include "Systems/Timing.h"
#include "Systems/Physics.h"
#include "GameObject.h"
#include "Console/ConsolePrint.h"

PlayerObjectController::PlayerObjectController() : forceApplied_p1({0.0f,0.0f}), forceApplied_p2({ 0.0f,0.0f }) {
	GLib::SetKeyStateChangeCallback(std::bind(&PlayerObjectController::KeypressHandler, this, std::placeholders::_1, std::placeholders::_2));   
}

void PlayerObjectController::update(GameObject& gameObject, float dt) {
    // Implement movement functionality for the player GameObject
	ForceComponent * forceComponent = gameObject.getComponent<ForceComponent>();
	if (gameObject.type == GameObjectType::PLAYER1) {
		forceComponent->force = forceComponent->force + forceApplied_p1;
		forceApplied_p1 = { 0.0f, 0.0f };
	} else if (gameObject.type == GameObjectType::PLAYER2) {
		forceComponent->force = forceComponent->force + forceApplied_p2;
		forceApplied_p2 = { 0.0f, 0.0f };
	}

	Physics::Update(gameObject.getComponent<MovementComponent>(), gameObject.getComponent<ForceComponent>(), dt);
}

void PlayerObjectController::KeypressHandler(unsigned int i_VKeyID, bool i_bDown) {
		if (i_VKeyID == 87) { // W key
			forceApplied_p1 = { 0,360000.0f };
		}
		else if (i_VKeyID == 65) { // A key
			forceApplied_p1 = { -360000.0f , 0 };
		}
		else if (i_VKeyID == 83) { // S key
			forceApplied_p1 = { 0, -360000.0f };
		}
		else if (i_VKeyID == 68) { // D 
			forceApplied_p1 = { 360000.0f , 0 };
		}

		if (i_VKeyID == 73) { // I key
			forceApplied_p2 = { 0,360000.0f };
		}
		else if (i_VKeyID == 74) { // J key
			forceApplied_p2 = { -360000.0f , 0 };
		}
		else if (i_VKeyID == 75) { // K key
			forceApplied_p2 = { 0, -360000.0f };
		}
		else if (i_VKeyID == 76) { // L 
			forceApplied_p2 = { 360000.0f , 0 };
		}
}
