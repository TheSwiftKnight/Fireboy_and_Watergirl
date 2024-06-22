#ifndef STONE_HPP
#define sTONE_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
class Stone: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool eaten = false;
    Stone(std::string imgStone, int x, int y, float radius);
    void Draw();
    int x, y;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
