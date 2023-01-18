#include "DEngine/DEngine.hpp"
#include "title.hpp"
#include "scenes.hpp"

void TitleScreen::Draw() {
    DrawFPS(0, 0);
    TitleScreen::PlayButton.Draw();
}

void TitleScreen::Update() {
    //TitleScreen::PlayButton.Update();

    if (TitleScreen::PlayButton.IsSelected()) {
        gameWindow << Game::Game;
    }
}

void TitleScreen::Setup() {
    TitleScreen::titleScreen.Draw = &TitleScreen::Draw;
    TitleScreen::titleScreen.Update = &TitleScreen::Update;
    //TitleScreen::titleScreen.Init = &TitleScreen::Init;
}