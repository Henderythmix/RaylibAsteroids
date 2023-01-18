#ifndef GAME
#define GAME

class Player {
    public:
        float X = 0, Y = 0;
        float Vel[2] = {0, 0}, Acc[2] = {0, 0};
        float rotation = 0;
        float Dec = -2;

        void Update();
        void Draw();
};

class Bullet {
    public:
        float X;
        float Y;
        float rotation = 0;

        Bullet(float x, float y, float rot) {
            X=x;Y=y;rotation=rot;
        };

        void Update();
        void Draw();
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

        void Draw();

        void Update();
};

namespace Game {
    inline bool Paused = false;
    inline int Score;
    inline float timer;

    inline Player player;
    inline std::vector<Bullet> bullets;
    inline std::vector<Asteroid> asteroids;

    void Init();

    void Update();

    void Draw();
}

#endif