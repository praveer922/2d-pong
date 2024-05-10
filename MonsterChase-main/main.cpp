#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#endif // _DEBUG
#pragma once
#include <Windows.h>
#include "GLib.h"
#include "GameObject/GameObject.h"
#include "Components/Components.h"
#include "Systems/Rendering.h"
#include "Systems/Timing.h"
#include "Systems/Physics.h"
#include "Console/ConsolePrint.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/PlayerObjectController.h"


using namespace Engine::Math;

void runGame(HINSTANCE i_hInstance, int i_nCmdShow) {
	Rendering::init(i_hInstance, i_nCmdShow, "MonsterChase", 800, 600);
	

	// create game objects
	std::vector<std::shared_ptr<GameObject>> AllGameObjects;

	std::shared_ptr<GameObject> player = GameObjectFactory::createGameObject("data\\Player.json");
	std::shared_ptr<GameObject> badGuy = GameObjectFactory::createGameObject("data\\BadGuy.json");

	AllGameObjects.push_back(player);
	AllGameObjects.push_back(badGuy);

	bool bQuit = false;

	// set player controller
	IGameObjectController* playerController = new PlayerObjectController();
	player->setController(playerController);

	while (!bQuit) {
		// IMPORTANT: We need to let GLib do it's thing. 
		GLib::Service(bQuit);
		// first, check if anything collided last frame
		if (Timing::nextCollisionTime != -1 && Timing::nextCollisionTime < Timing::getCurrentTime()) {
			Engine::ConsolePrint("COLLISSION!!!\n");
			Physics::handleCollision(player->getComponent<MovementComponent>(), player->getComponent<ForceComponent>());
		}

		for (auto& gameObject : AllGameObjects) {
			gameObject->update();
		}

		// update collision time
		float collision_time = Physics::sweptAABB(player->getComponent<MovementComponent>(), 
			badGuy->getComponent<MovementComponent>());
		if (collision_time == -1) {
			Timing::nextCollisionTime = -1;
		}
		else {
			Timing::nextCollisionTime = collision_time + Timing::getCurrentTime();
		}

		Rendering::beginRenderLoop();
		for (auto& gameObject : AllGameObjects) {
			Rendering::drawSprite(gameObject->getComponent<SpriteComponent>(),
				gameObject->getComponent<MovementComponent>());
		}
		Rendering::endRenderLoop();
	}

	// IMPORTANT:  Tell GLib to shutdown, releasing resources.
	Rendering::shutdown();
	delete playerController;
	// game object shared ptrs are deallocated automatically
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	runGame(i_hInstance, i_nCmdShow);

#ifdef _DEBUG	
	_CrtDumpMemoryLeaks();
#endif // _DEBUG	
}


