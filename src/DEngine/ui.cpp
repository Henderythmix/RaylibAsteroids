#include "DEngine.hpp"
using namespace DEngine::UI;
using namespace DEngine::Input;

#include <iostream>

// UI Component
DEngine::UI::UIComponent::UIComponent() {
    std::cerr << "Do not use the default constructor. Thx :D" << std::endl;
}

DEngine::UI::UIComponent::UIComponent(int x, int y, int w, int h) {
    Position[0] = x;
    Position[1] = y;
    Dimensions[0] = w;
    Dimensions[1] = h;
}

void DEngine::UI::UIComponent::Draw() {
    DrawRectangle(Position[0], Position[1], Dimensions[0], Dimensions[1], BGColor);
}

void DEngine::UI::UIComponent::Update() {
    if (DEngine::Input::UsingGamepad) {

    } else {

    }
}

// Button

void DEngine::UI::Button::Draw() {
    DrawRectangle(Position[0], Position[1], Dimensions[0], Dimensions[1], BGColor);
    DrawText(text, Position[0], Position[1], 12, TextColor);
}

bool DEngine::UI::Button::IsSelected() {
    if (DEngine::Input::UsingGamepad) {
        if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            return true;
        }
    } else {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }
    return false;
}