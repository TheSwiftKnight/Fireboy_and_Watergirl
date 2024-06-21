#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;

class Button: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();
    // Reference: Design Patterns - Factory Method.

public:
    bool Opened = false;
    Button(std::string imgButton, int x, int y);
    void Draw() const override;
    void CreateButton();
    int x, y;
    
};
#endif // TURRET_HPP
