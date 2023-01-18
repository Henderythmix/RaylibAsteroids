#include "DEngine.hpp"

DEngine::Window::Window() {
    Width = DefaultWidth;
    Height = DefaultHeight;
    ::InitWindow(Width, Height, "Game Title");
    ::SetTargetFPS(60);
    //::ToggleFullscreen();
    CurrentScene = &DEngine::Default::Scene;
    CurrentScene->Init();
}

void DEngine::Window::Loop() {
    while (DEngine::Running) {
        // Pre-Processing Phase
        if (WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) DEngine::Running = false;
        if (IsKeyPressed(KEY_F11)) ::ToggleFullscreen();
        CurrentScene->Update();

        // Drawing Phase
        ::BeginDrawing();
        ::ClearBackground(BLACK);
        CurrentScene->Draw();
        ::EndDrawing();
    }
}

void DEngine::Window::operator<<(DEngine::Scene& NewScene) {
    CurrentScene = &NewScene;
    CurrentScene->Init();
}