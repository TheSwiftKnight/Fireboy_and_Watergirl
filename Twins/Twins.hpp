#ifndef TWINS_HPP
#define TWINS_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"

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
    
    bool active = false;
    int sourceX;
    int moveCD;

    // std::vector<std::vector<int>> mapState;
    Twins(std::string imgTwins, int x, int y, float radius);
    // void getMapState(std::vector<std::vector<PlayScene::TileType>> ms,int h,int w);
    void MeUpdate();
    void MeDraw();
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    void updateTime(int deltaTime);
    int speed;
    bool jump;
    int x;
    int y;
    int dir = NO;
	// int GetPrice() const;
};
#endif // TURRET_HPP
