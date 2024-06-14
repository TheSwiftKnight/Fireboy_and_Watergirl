#ifndef TWINS_HPP
#define TWINS_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;

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
    Twins(std::string imgTwins, float x, float y, float radius);
    void Update(float deltaTime) override;
    void Draw() const override;
	// int GetPrice() const;
};
#endif // TURRET_HPP
