#ifndef TWINS_HPP
#define TWINS_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;

enum Direction{NO,UP,DOWN,LEFT,RIGHT};

class Twins: public Engine::Sprite {
protected:
    // int price;
    // float coolDown;
    // float reload = 0;
    // float rotateRadian = 2 * ALLEGRO_PI;
    // Sprite imgBase;
    std::list<Twins*>::iterator lockedTwinsIterator;
    PlayScene* getPlayScene();
    // Reference: Design Patterns - Factory Method.

    // virtual void CreateBullet() = 0;

public:
    bool Tool = false;
    bool Enabled = true;
    bool Preview = false;
    Twins(std::string imgTwins, int x, int y, float radius);
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    int speed;
    bool jump;
    int x;
    int y;
    int dir = NO;
	// int GetPrice() const;
};
#endif // TURRET_HPP
