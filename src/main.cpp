#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "DEngine/DEngine.hpp"
using namespace DEngine;
using namespace DEngine::Input;

// Create Game Window
static const char gameTitle[] = "Game Title";
Window<gameTitle> gameWindow;

// Game Info

class Player {
    public:
        float X = 0;
        float Y = 0;
        float Vel = 0;
        float Acc = 0;
        float rotation = 0;
        float Dec = -2;

        void Update() {
            Acc = 2*IsJoystick(LVertical);
            rotation += IsJoystick(LHorizontal)*4*GetFrameTime();

            if (Acc == 0) { 
                if (Vel < 0) {
                    Vel -= Dec;
                } else {
                    Vel -= -Dec;
                }
            } else { Vel += Acc;
            }
            X += cos(rotation) * Vel * GetFrameTime();
            Y += sin(rotation) * Vel * GetFrameTime();

            if (X > CenterX) X = -CenterX;
            if (X < -CenterX) X = CenterX;
            if (Y > CenterY) Y = -CenterY;
            if (Y < -CenterY) Y = CenterY;
        };

        void Draw() {
            DrawPoly((Vector2){CenterX+X, CenterY+Y}, 3, 15, (rotation*180/PI)-90, WHITE);
        }
};

class Bullet {
    public:
        float X;
        float Y;
        float rotation = 0;

        Bullet(float x, float y, float rot) {
            X=x;Y=y;rotation=rot;
        };

        void Update() {
            X+=400*cos(rotation)*GetFrameTime();
            Y+=400*sin(rotation)*GetFrameTime();
        }

        void Draw() {
            DrawRectangle(CenterX+X-2, CenterY+Y-2, 4, 4, WHITE);
        }
};

class Asteroid {
    public:
        float X;
        float Y;
        float InitX;
        float InitY;
        int size;

        Asteroid() {
            int edge = rand() % 2 ? 1 : -1;
            if (rand() % 2) {
                X = (CenterX - 5) * edge;
                Y = rand() % DefaultHeight - CenterY;
            } else {
                Y = (CenterY - 5) * edge;
                X = rand() % DefaultWidth - CenterX;
            }
            InitX=X; InitY=Y;
            size = rand()%20+25;
        }

        void Draw() {
            DrawPoly((Vector2){CenterX+X, CenterY+Y}, size%3+5, size, 0, WHITE);
        }

        void Update() {
            X += -(InitX/5)*GetFrameTime();
            Y += -(InitY/5)*GetFrameTime();
        }
};

namespace Game {
    bool Paused = false;
    int Score;
    float timer;

    Player player;
    vector<Bullet> bullets;
    vector<Asteroid> asteroids;

    Scene Game;

    void Init() {
        Score = 0;
        Game::player = Player();
        bullets.erase(bullets.begin(), bullets.begin()+(bullets.size()));
        asteroids.erase(asteroids.begin(), asteroids.begin()+(asteroids.size()));
    }

    void Update() {
        if (!Paused) {
            if (IsInput<Pressed>(Start)) Paused = true;
            
            // Player Updating
            player.Update();
            if (IsInput<Pressed>(A) && bullets.size() < 10) {
                bullets.push_back(Bullet(player.X, player.Y, player.rotation));
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

    void Draw() {
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
}


int main()
{
    Game::Game.Init = &Game::Init;
    Game::Game.Update = &Game::Update;
    Game::Game.Draw = &Game::Draw;

    gameWindow << Game::Game;

    gameWindow.Loop();

    return 0;
}