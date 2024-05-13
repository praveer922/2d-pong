#pragma once

#include "GLib.h"
#include "Math/Vector2.h"
#include "Components/Components.h"


using namespace GLib;
using namespace Engine::Math;


namespace Rendering {
    void init(HINSTANCE i_hInstance, int i_nCmdShow, const char* windowName, unsigned int windowWidth, unsigned int windowHeight);
    void beginRenderLoop();
    void drawSprite(SpriteComponent* spriteComponent, MovementComponent* movementComponent);
    void endRenderLoop();
    void shutdown();
}
