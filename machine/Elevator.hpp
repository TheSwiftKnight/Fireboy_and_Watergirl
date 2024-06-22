#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
class Elevator: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool opening;
    Elevator(std::string imgElevator, int x, int y, int init_x, int final_x);
    void Draw();
    int x, y;
    int init_x, final_x;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
