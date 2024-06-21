#ifndef TWINS_HPP
#define TWINS_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"

class Enemy;
class PlayScene;

enum Direction{NO,LEFT,RIGHT};

class Twins: public Engine::Sprite {
protected:
    std::list<Twins*>::iterator lockedTwinsIterator;
    PlayScene* getPlayScene();
public:
    bool Tool = false;

    bool Enabled = true;
    bool Preview = false;
    
    bool active = false;
    int sourceX;
    int moveCD;

    const float gravity = 1;
    int speed;
    bool jump;
    bool jumpTimestamp;
    int jumpCD;
    float jumpspeed = 5;
    int x,y;
    int velx,vely;
    int dir = NO;

    Twins(std::string imgTwins, int x, int y, float radius);
    void XUpdate();
    void YUpdate();
    void MeDraw();
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    void updateTime(int deltaTime);
};
#endif // TURRET_HPP
