/*
 * DEngine - A Raylib Wrapper Designed for Modern Games
 * By: Henderythmix
 *
 * About: This is just a personal C++ wrapper I made for packing games in a cross platform ready structure
 * (and showing off some c++ skills lol)
 * 
 * Some features include:
 * - A Dynamic Window TEMPLATE Class (Insert Exploding Head Emoji) (done)
 * - Simplified Controls for Cross-Platform Systems (50%-ish)
 * - A Convenient Scene Manager (DEngine::Window << DEngine::Scene) (done)
 * 
 * You may have a better experience with the RobLoach wrapper, but feel free to use this too :D
 */

#pragma once

extern "C" {
    #include "raylib.h"
}

#ifndef DefaultWidth
#define DefaultWidth 1280
#endif

#ifndef DefaultHeight
#define DefaultHeight 720
#endif

#define CenterX (DefaultWidth/2)
#define CenterY (DefaultHeight/2)

// --------------------- //
// Namespace Declaration //
// --------------------- //

namespace DEngine {
    namespace Default {
        inline void SceneInit() {};
        inline void SceneDraw() {
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        };
        inline void SceneUpdate() {};
    }

    class Scene {
        public:
            void (*Init)();
            void (*Draw)();
            void (*Update)();

            Scene() {
                Init = &DEngine::Default::SceneInit;
                Draw = &DEngine::Default::SceneDraw;
                Update = &DEngine::Default::SceneUpdate;
            };
    };

    template<char const *T>
    class Window {
        public:
            int Width;
            int Height;
            DEngine::Scene* CurrentScene;

            Window();
    
            void Loop();

            void operator<<(DEngine::Scene& NewScene);
    };

    inline bool Running = true;

    namespace Input {
        enum Button{Start, Select, A, B, X, Y, L, R, ZL, ZR};
        enum Joystick{LVertical, LHorizontal};

        static int KeyButtonBinding[]{KEY_ESCAPE, KEY_M, KEY_SPACE, KEY_LEFT_SHIFT };
        static int KeyJoystickBinding[]{KEY_W, KEY_S, KEY_D, KEY_A};

        static bool UsingGamepad = false;

        enum InputType{Down, Pressed, Up, Released};
        template<int inpType>
        inline bool IsInput(int B);
        inline float IsJoystick(int D);
    }

    namespace Math {
        template<class T>
        inline bool InRange(T x, T low, T high) {
            return (low <= x && x <= high);
        }
    }
}

namespace DEngine {
    namespace Default {
        inline DEngine::Scene Scene;
    }
}

// ------------ //
// Window Class //
// ------------ //

template <char const *T>
DEngine::Window<T>::Window() {
    Width = DefaultWidth;
    Height = DefaultHeight;
    ::InitWindow(Width, Height, T);
    ::SetTargetFPS(60);
    //::ToggleFullscreen();
    CurrentScene = &DEngine::Default::Scene;
    CurrentScene->Init();
}

template <char const *T>
void DEngine::Window<T>::Loop() {
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

    CloseWindow();
}

template<char const *T>
void DEngine::Window<T>::operator<<(DEngine::Scene& NewScene) {
    CurrentScene = &NewScene;
    CurrentScene->Init();
}

// ------------ //
// Input Module //
// ------------ //

template<int inpType>
bool DEngine::Input::IsInput(int B) {
    static int GamepadButtonBindings[]{GAMEPAD_BUTTON_MIDDLE_RIGHT, GAMEPAD_BUTTON_MIDDLE_LEFT, GAMEPAD_BUTTON_RIGHT_FACE_DOWN};
    static bool (*KeyInput[])(int) {&IsKeyDown, &IsKeyPressed, &IsKeyUp, &IsKeyReleased};
    static bool (*GPInput[])(int, int) {&IsGamepadButtonDown, &IsGamepadButtonPressed, &IsGamepadButtonUp, &IsGamepadButtonReleased};

    if ((KeyInput[inpType])(DEngine::Input::KeyButtonBinding[B])) {
        DEngine::Input::UsingGamepad = false;
        return true;
    } else if ((GPInput[inpType])(0, GamepadButtonBindings[B])) {
        DEngine::Input::UsingGamepad = true;
        return true;
    } else {
        return false;
    }
}

float DEngine::Input::IsJoystick(int D) {
    int Axis[] {GAMEPAD_AXIS_LEFT_Y, GAMEPAD_AXIS_LEFT_X};
    if (GetGamepadAxisMovement(0, Axis[D]) != 0) {
        DEngine::Input::UsingGamepad = true;
        return GetGamepadAxisMovement(0, Axis[D]) * (D % 2 ? 1 : -1);
    } else {
        if (IsKeyDown(Input::KeyJoystickBinding[D*2])) {
            UsingGamepad = false;
            return 1;
        } else if (IsKeyDown(Input::KeyJoystickBinding[D*2+1])) {
            UsingGamepad = false;
            return -1;
        } else {
            return 0;
        }
    }
}