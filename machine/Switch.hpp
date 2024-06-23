#ifndef SWITCH_HPP
#define SWITCH_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
class Switch: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool opened;
    Switch(std::string imgSwitch, int x, int y, int init_y, int final_y, int num, int opening);
    void Draw();
    int x, y;
    int init_x, final_x;
    int Switch_num;
    void Update(float deltatime) override;
};
#endif // TURRET_HPP
