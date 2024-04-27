#include "Rendering.h"
#include <stdexcept>
#include <DirectXColors.h>


namespace Rendering {

    void init(HINSTANCE i_hInstance, int i_nCmdShow, const char* windowName, unsigned int windowWidth, unsigned int windowHeight) {
        bool success =  GLib::Initialize(i_hInstance, i_nCmdShow, windowName, -1, windowWidth, windowHeight, true);
        if (!success) {
            throw std::runtime_error("Failed to initialize Glib.");
        }
    }

    void beginRenderLoop() {
        // IMPORTANT: Tell GLib that we want to start rendering
        GLib::BeginRendering(DirectX::Colors::Blue);
        // Tell GLib that we want to render some sprites
        GLib::Sprites::BeginRendering();
    }

    void drawSprite(SpriteComponent * spriteComponent, MovementComponent * movementComponent) {
        // Tell GLib to render this sprite at our calculated location
        GLib::Render(*(spriteComponent->sprite),
            { movementComponent->position.x(),movementComponent->position.y() },
            0.0f, 0.0f);
    }

    void endRenderLoop() {
        // Tell GLib we're done rendering sprites
        GLib::Sprites::EndRendering();
        // IMPORTANT: Tell GLib we're done rendering
        GLib::EndRendering();
    }

    void shutdown() {
        GLib::Shutdown();
    }
}