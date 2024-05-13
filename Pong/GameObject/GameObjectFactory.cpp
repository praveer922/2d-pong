#pragma once
#include <cassert>
#include "GameObjectFactory.h"
#include "GLib.h"
#include "Math/Vector2.h"



GLib::Sprite* CreateSprite(const char* i_pFilename);
std::vector<uint8_t> LoadFileToBuffer(const char* i_pFilename);

using json = nlohmann::json;

namespace GameObjectFactory {
	SpriteComponent* createSpriteComponent(const std::string& spritePath) {
		return new SpriteComponent(CreateSprite(spritePath.c_str()));
	}

	std::shared_ptr<GameObject> CreateGameObject(nlohmann::json& ConfigData)
	{
		using namespace Engine::Math;

		std::shared_ptr<GameObject> NewGameObject;
		if (ConfigData["type"] == "player1") {
			NewGameObject = std::make_shared<GameObject>(GameObjectType::PLAYER1);
		}
		else if (ConfigData["type"] == "player2") {
			NewGameObject = std::make_shared<GameObject>(GameObjectType::PLAYER2);
		}
		else {
			NewGameObject = std::make_shared<GameObject>(GameObjectType::BALL);
		}

		if (ConfigData.contains("components"))
		{
			assert(ConfigData["components"].is_object());



			for (json::iterator it = ConfigData["components"].begin(); it != ConfigData["components"].end(); ++it)
			{
				const std::string& ComponentName = it.key();

				if (ComponentName == "sprite") {
					NewGameObject->addComponent(createSpriteComponent(it.value()["sprite_texture_source"]));
				}
				else if (ComponentName == "movement") {
					// Access the "position" attribute (tuple)
					const json& positionArray = it.value()["position"];

					// Access individual elements of the tuple
					float x = positionArray[0]; // Accessing the first element (x-coordinate)
					float y = positionArray[1]; // Accessing the second element (y-coordinate)
					float width = it.value()["width"];
					float height = it.value()["height"];
					Vector2 velocity = { 0.0f, 0.0f };
					if (NewGameObject->type == GameObjectType::BALL) {
						velocity = { 100.0f, 100.0f };
					}
					NewGameObject->addComponent(new MovementComponent{Vector2(x,y), velocity, width, height});
				}

				NewGameObject->addComponent(new ForceComponent{ Vector2(0.0f,0.0f) });
			}
		}

		return NewGameObject;
	}

	std::shared_ptr<GameObject> createGameObject(const char* i_pJSONConfig)
	{
		std::vector<uint8_t> ConfigDataData = LoadFileToBuffer(i_pJSONConfig);

		if (!ConfigDataData.empty())
		{
			json ConfigDataJSON = json::parse(ConfigDataData);
			return CreateGameObject(ConfigDataJSON);
		}
		else
		{
			return nullptr;
		}
	}

}

void* LoadFile(const char* i_pFilename, size_t& o_sizeFile)
{
	assert(i_pFilename != NULL);

	FILE* pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile != NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError == 0);

	long FileSize = ftell(pFile);
	assert(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError == 0);

	uint8_t* pBuffer = new uint8_t[FileSize];
	assert(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}

GLib::Sprite* CreateSprite(const char* i_pFilename)
{
	assert(i_pFilename);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void* pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(*pTexture, width, height, depth);
	assert(result == true);
	assert((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

	// release our reference on the Texture
	GLib::Release(pTexture);

	return pSprite;
}

std::vector<uint8_t> LoadFileToBuffer(const char* i_pFilename)
{
	assert(i_pFilename != nullptr);

	std::vector<uint8_t> Buffer;

	FILE* pFile = nullptr;

	if (fopen_s(&pFile, i_pFilename, "rb") == 0)
	{
		assert(pFile != nullptr);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		assert(FileIOError == 0);

		Buffer.reserve(FileSize);
		Buffer.resize(FileSize);

		size_t FileRead = fread(&Buffer[0], 1, FileSize, pFile);
		assert(FileRead == FileSize);

		fclose(pFile);
	}

	return Buffer;
}
