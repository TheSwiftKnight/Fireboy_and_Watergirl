#ifndef DIAMOND_HPP
#define DIAMOND_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;

class Diamond: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool Opened = false;
    Diamond(std::string imgDiamond, int x, int y);
    void Draw() const override;
    void CreateDiamond();
    int x, y;
    
};
#endif // TURRET_HPP
