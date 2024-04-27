#pragma once

#include <string>
#include "Components/Components.h"
#include "GameObject.h"
#include "json.hpp"
#include <memory>


namespace GameObjectFactory {
    SpriteComponent* createSpriteComponent(const std::string& spritePath);
	std::shared_ptr<GameObject> createGameObject(const char* i_pJSONConfig);
}