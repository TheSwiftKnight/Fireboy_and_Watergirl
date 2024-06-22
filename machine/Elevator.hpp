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
    Elevator(std::string imgElevator, int x, int y, int init_x, int final_x, int num);
    void Draw();
    int x, y;
    int init_x, final_x;
    int elevator_num;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
