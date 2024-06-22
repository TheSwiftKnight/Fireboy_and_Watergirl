#ifndef DIAMOND_HPP
#define DIAMOND_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
enum Diamond_type {blue, red};
class Diamond: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool eaten = false;
    Diamond(std::string imgDiamond, int x, int y, float radius, std::string diamond_type);
    void Draw();
    int x, y;
    Diamond_type diamond_type;
    void Update(float deltatime);
};
#endif // TURRET_HPP
