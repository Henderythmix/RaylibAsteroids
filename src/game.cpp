#include <vector>
#include <cmath>

#include "DEngine/DEngine.hpp"
using namespace DEngine;
using namespace DENGINE::Input;

#include "scenes.hpp"
#include "game.hpp"

// Player Class
void Player::Update() {
    Acc[0] = 2*IsJoystick(LHorizontal);
    Acc[1] = -2*IsJoystick(LVertical);

    if (UsingGamepad == true) {
        if (IsJoystick(RHorizontal) != 0) rotation = (IsJoystick(RHorizontal) > 0) ? atanf(-IsJoystick(RVertical)/IsJoystick(RHorizontal)) : (PI + atanf(-IsJoystick(RVertical)/IsJoystick(RHorizontal)));
    } else {
        rotation = (GetMouseX()-CenterX-X > 0) ? atanf((GetMousePosition().y-CenterY-Y)/(GetMousePosition().x-CenterX-X)) : (PI + atanf((GetMousePosition().y-CenterY-Y)/(GetMousePosition().x-CenterX-X)) );
    }

    Vel[0] += Acc[0];
    Vel[1] += Acc[1];
    
    X += Vel[0] * GetFrameTime();
    Y += Vel[1] * GetFrameTime();
    
    if (X > CenterX) X = -CenterX;
    if (X < -CenterX) X = CenterX;
    if (Y > CenterY) Y = -CenterY;
    if (Y < -CenterY) Y = CenterY;
};

void Player::Draw() {
    DrawPoly((Vector2){CenterX+X, CenterY+Y}, 3, 15, (rotation*180/PI)-90, WHITE);
}

// Bullet Class

void Bullet::Update() {
    X+=400*cos(rotation)*GetFrameTime();
    Y+=400*sin(rotation)*GetFrameTime();
}

void Bullet::Draw() {
    DrawRectangle(CenterX+X-2, CenterY+Y-2, 4, 4, WHITE);
}

// Asteroid Class

void Asteroid::Draw() {
    DrawPoly((Vector2){CenterX+X, CenterY+Y}, size%3+5, size, 0, WHITE);
}

void Asteroid::Update() {
    X += -(InitX/5)*GetFrameTime();
    Y += -(InitY/5)*GetFrameTime();
}

// Game Namespace

void Game::Init() {
    Score = 0;
    Game::player = Player();
    bullets.erase(bullets.begin(), bullets.begin()+(bullets.size()));
    asteroids.erase(asteroids.begin(), asteroids.begin()+(asteroids.size()));
}

void Game::Update() {
    if (!Paused) {
        if (IsInput<Pressed>(Start)) Paused = true;
        
        // Player Updating
        player.Update();
        if (IsInput<Pressed>(L) && bullets.size() < 10) {
            bullets.push_back(Bullet(player.X, player.Y, player.rotation));
            player.Vel[0] -= 50*cos(player.rotation);
            player.Vel[1] -= 50*sin(player.rotation);
        }
        for (int i=0; i<bullets.size(); i++) {
            bullets[i].Update();
            if ((bullets[i].X>CenterX || bullets[i].X< -CenterX) || (bullets[i].Y>CenterY || bullets[i].Y< -CenterY)) {
                bullets.erase(bullets.begin()+i, bullets.begin()+i+1);
                bullets.shrink_to_fit();
            }
        }
        // Asteroids
        if (timer > 1) {
            asteroids.push_back(Asteroid());
            timer -= 1;
        }
        for (int i=0; i<asteroids.size(); i++) {
            asteroids[i].Update();
            if ((asteroids[i].X>CenterX || asteroids[i].X< -CenterX) || (asteroids[i].Y>CenterY || asteroids[i].Y< -CenterY)) {
                asteroids.erase(asteroids.begin()+i, asteroids.begin()+i+1);
                asteroids.shrink_to_fit();
            }
            // Asteroid Collision
            for (int b=0; b<bullets.size(); b++) {
                if (Math::InRange<float>(bullets[b].X, asteroids[i].X-asteroids[i].size+3, asteroids[i].X+asteroids[i].size-3) && Math::InRange<float>(bullets[b].Y, asteroids[i].Y-asteroids[i].size+3, asteroids[i].Y+asteroids[i].size-3)) {
                    bullets.erase(bullets.begin()+b, bullets.begin()+b+1);
                    asteroids.erase(asteroids.begin()+i, asteroids.begin()+i+1);
                    bullets.shrink_to_fit();
                    asteroids.shrink_to_fit();
                    Score++;
                    break;
                }
            }
            if (Math::InRange<float>(player.X, asteroids[i].X-asteroids[i].size+3, asteroids[i].X+asteroids[i].size-3) && Math::InRange<float>(player.Y, asteroids[i].Y-asteroids[i].size+3, asteroids[i].Y+asteroids[i].size-3)) gameWindow << Game::Game;
        }
        timer += GetFrameTime();
    } else {
        if (IsInput<Pressed>(Start)) CloseWindow();
        if (IsInput<Pressed>(A)) Paused = false;
    }
    if (IsInput<Pressed>(Select)) gameWindow << Game::Game;
}

void Game::Draw() {
    player.Draw();
    for (Bullet b : Game::bullets) {
        b.Draw();
    }

    for (Asteroid A: asteroids) {
        A.Draw();
    }

    DrawText(TextFormat("Score: %i", Score), 10, 10, 12, WHITE);

    if (Paused) DrawText("Paused", 10, 30, 12, WHITE);
}