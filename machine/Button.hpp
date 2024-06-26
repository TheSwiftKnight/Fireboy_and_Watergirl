#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp" 

class PlayScene;
class Button: public Engine::Sprite {
protected:
    PlayScene* getPlayScene();

public:
    bool started;
    Button(std::string imgbutton, int x, int y, float radius, int num);
    void Draw();
    int x, y;
    void Update(float deltatime) override;
    bool getStartedValue();
    int button_num;
};
#endif // TURRET_HPP
