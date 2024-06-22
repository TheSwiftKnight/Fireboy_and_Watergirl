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
    bool started;
    Elevator(std::string imgElevator, int x, int y, float radius);
    void Draw();
    int x, y;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
