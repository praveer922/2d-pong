#include "Timing.h"
#include <windows.h>

namespace Timing {
    static LARGE_INTEGER frequency;
    
    float getDeltaTimeSinceLastCall() {
        static LARGE_INTEGER lastCallTime;
        LARGE_INTEGER currentTime;

        // Initialize the frequency (ticks per second) if not done already
        if (frequency.QuadPart == 0) {
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&lastCallTime);
            return 0.0; // Delta time for the first call will be zero
        }

        QueryPerformanceCounter(&currentTime);

        // Calculate delta time in seconds
        float deltaTime = static_cast<float>(currentTime.QuadPart - lastCallTime.QuadPart) / frequency.QuadPart;

        lastCallTime = currentTime;

        return deltaTime;
    }

    float getCurrentTime() {
        LARGE_INTEGER currentTime;

        QueryPerformanceCounter(&currentTime);

        return static_cast<float>(currentTime.QuadPart) / frequency.QuadPart;
    }
}