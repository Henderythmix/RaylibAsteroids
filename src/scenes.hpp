#pragma once 

namespace TitleScreen {
    inline DEngine::Scene titleScreen;
}

namespace Game {
    inline DEngine::Scene Game;
}

// Create Game Window
static const char gameTitle[] = "Asteroids";
extern DEngine::Window gameWindow;