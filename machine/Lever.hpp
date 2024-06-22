#ifndef LEVER_HPP
#define LEVER_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
enum Lever_state {right, left, middle};
class Lever: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool opening;
    Lever(std::string imgLever,int x, int y, std::string s);
    void Draw();
    int x, y;
    int init_x, final_x;
    int Lever_num;
    Lever_state state;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
