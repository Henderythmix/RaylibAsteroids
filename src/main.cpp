#include <iostream>
using namespace std;

#include "DEngine/DEngine.hpp"
using namespace DEngine;
using namespace DEngine::Input;

#include "scenes.hpp"
#include "title.hpp"
#include "game.hpp"

DEngine::Window gameWindow;

int main() {
    TitleScreen::Setup();

    Game::Game.Init = &Game::Init;
    Game::Game.Update = &Game::Update;
    Game::Game.Draw = &Game::Draw;

    gameWindow << TitleScreen::titleScreen;

    gameWindow.Loop();

    return 0;
}

// To-Do
//
// Astroids can crash into each other
// Astroids bounce off walls (?)
// Astroids split into two after being broken
// Decent Graphics?
// - Fancy Shaders??? 🤔