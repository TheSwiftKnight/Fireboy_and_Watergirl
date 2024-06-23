#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>
#include <iostream>
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Stone.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
PlayScene* Stone::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Stone::Stone(std::string imgStone,int x, int y, float radius):
    Sprite(imgStone, x, y), x(x), y(y){
    CollisionRadius = radius;
}
void Stone::Draw() {
	Sprite::Draw();
}
void Stone::Update(float deltaTime) {
    //x=14,y=7
    //falling
    int y2 = (y+32)/64  , x2 = (x-32) / 64;
    if(getPlayScene()->mapState[y2][x2] == (getPlayScene()->TILE_DIRT || getPlayScene()->TILE_DIAMOND)){
        getPlayScene()->mapState[(y-32)/64][x2] = getPlayScene()->TILE_DIRT;
        getPlayScene()->StoneGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->StoneGroup->AddNewObject(new Stone("play/stone.png", x, y+8, 0));
    }
    std::cout <<  x << " " <<((x-32)%64==0) <<" "<< (x-32)/64 - ((x-32)%64==0) << " " << y2-1 << std::endl;
    //push
    bool left_collision = Engine::Collider::IsPointInRect(getPlayScene()->boy->Position, Position-Engine::Point(64,32), Size) || \
       Engine::Collider::IsPointInRect(getPlayScene()->girl->Position, Position-Engine::Point(64,32), Size) ;
    bool right_collision = Engine::Collider::IsPointInRect(getPlayScene()->boy->Position, Position+Engine::Point(0,-32), Size) || \
       Engine::Collider::IsPointInRect(getPlayScene()->girl->Position, Position+Engine::Point(0,-32), Size) ;
    if(left_collision && !right_collision && getPlayScene()->mapState[y2-1][(x-32)/64 + ((x-32)%64==0)] != getPlayScene()->TILE_DIRT){
        getPlayScene()->mapState[y2-1][(x-32)/64 + ((x-32)%64==0)] = getPlayScene()->TILE_FLOOR;
        std::cout << "left\n";
        getPlayScene()->StoneGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->StoneGroup->AddNewObject(new Stone("play/stone.png", x+4, y, 0));
    }
    else if(!left_collision && right_collision && getPlayScene()->mapState[y2-1][(x-32)/64 - ((x-32)%64==0)] != getPlayScene()->TILE_DIRT)
    {

        getPlayScene()->mapState[y2-1][(x-32)/64]  = getPlayScene()->TILE_FLOOR;
        std::cout << "right" << " " << (x-32)/64 << "\n" ;
        getPlayScene()->StoneGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->StoneGroup->AddNewObject(new Stone("play/stone.png", x-4, y, 0));
    }
    if()
}