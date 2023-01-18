#ifndef TITLESCREEN
#define TITLESCREEN

namespace TitleScreen {
    void Draw();
    void Update();
    void Setup();
    inline DEngine::UI::Button PlayButton("Play", 10, 10, 200, 25);
}

#endif