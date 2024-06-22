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
    getPlayScene()->mapState[(y-32)/64][x/64] = getPlayScene()->TILE_FLOOR;
}
void Stone::Draw() {
	Sprite::Draw();
}
void Stone::Update(float deltaTime) {
    
    if(Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->girl->Position, getPlayScene()->boy->Position + Size / 2) ){
        eaten = 1;
        getPlayScene()->StoneGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->StoneGroup->AddNewObject(new Stone("play/stone.png", x-3, y, 0));
    }
    
    
}