#ifndef LEVER_HPP
#define LEVER_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
class Lever: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool middle_started,started;
    Lever(std::string imgLever, int x, int y, int mid_start, int final_start, int num);
    void Draw();
    int x, y;
    void Update(float deltatime) override;
    bool getStartedValue();
    int Lever_num;
};
#endif // TURRET_HPP
