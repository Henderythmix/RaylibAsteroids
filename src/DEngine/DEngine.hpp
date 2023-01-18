/*
 * DEngine - A Personal Raylib Wrapper
 * By: Henderythmix
 *
 * About: This is just a personal C++ wrapper I made for creating games in a modern structure
 * (and showing off some c++ skills lol)
 * 
 * Some features include:
 * - A Dynamic Window TEMPLATE Class (Insert Exploding Head Emoji) (done)
 * - Simplified Controls for Cross-Platform Systems (50%-ish)
 * - A Convenient Scene Manager (DEngine::Window << DEngine::Scene) (done)
 *
 * If you want a specific module, you will need to compile it separately 
 * These modules include:
 * - UI
 * 
 * You may have a better experience with the RobLoach wrapper, but feel free to use this too :D
 */

#ifndef DENGINE
#define DENGINE

#include <vector>
#include <string>

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
        enum Joystick{LVertical, LHorizontal, RVertical, RHorizontal};

        static int KeyButtonBinding[]{KEY_ESCAPE, KEY_M, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_SPACE, KEY_LEFT_SHIFT };
        static int KeyJoystickBinding[]{KEY_W, KEY_S, KEY_D, KEY_A};

        static bool UsingGamepad = false;

        enum InputType{Down, Pressed, Up, Released};
        template<int inpType>
        inline bool IsInput(int B);
        inline double IsJoystick(int D);
    }

    namespace Math {
        template<class T>
        inline bool InRange(T x, T low, T high) {
            return (low <= x && x <= high);
        }
    }

    namespace UI {
        enum Alignment{TopLeft};
        class UIComponent {
            public:
                Color BGColor = (Color){200, 200, 200, 255};
                Color BGHighlighted = (Color){128, 128, 128, 255};
                Color BGSelected = (Color){255, 255, 255, 255};

                Color TextColor = (Color){0, 0, 0, 255};

                bool hovered = false;
                bool selected = false;

                void (*Callback)();

                int Position[2] = {0, 0};
                int Dimensions[2] = {0, 0};
                int Alignment{TopLeft};

                void Draw();
                void Update();
                UIComponent();
                UIComponent(int x, int y, int w, int h);
        };

        class Button : public UIComponent {
            public:
                char* text;
                bool IsSelected();
                void Draw();
                //void Update();
                //Button(int x, int y, int w, int h);
                Button(char* t, int x, int y, int w, int h): UIComponent() {
                    text = t;
                    Position[0] = x;
                    Position[1] = y;
                    Dimensions[0] = w;
                    Dimensions[1] = h;
                };
        };
    }
}

namespace DEngine {
    namespace Default {
        inline DEngine::Scene Scene;
    }
}

// ------------ //
// Input Module //
// ------------ //

template<int inpType>
bool DEngine::Input::IsInput(int B) {
    static int GamepadButtonBindings[]{GAMEPAD_BUTTON_MIDDLE_RIGHT, GAMEPAD_BUTTON_MIDDLE_LEFT, GAMEPAD_BUTTON_RIGHT_FACE_DOWN, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT, GAMEPAD_BUTTON_LEFT_FACE_LEFT, GAMEPAD_BUTTON_LEFT_FACE_UP, GAMEPAD_BUTTON_LEFT_TRIGGER_1};
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

double DEngine::Input::IsJoystick(int D) {
    int Axis[] {GAMEPAD_AXIS_LEFT_Y, GAMEPAD_AXIS_LEFT_X, GAMEPAD_AXIS_RIGHT_Y, GAMEPAD_AXIS_RIGHT_X};
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

#endif