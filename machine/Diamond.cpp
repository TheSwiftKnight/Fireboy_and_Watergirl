#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>
#include <iostream>
// #include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Diamond.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
PlayScene* Diamond::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Diamond::Diamond(std::string imgDiamond,int x, int y, float radius, std::string type):
    Sprite(imgDiamond, x, y), x(x), y(y){
    CollisionRadius = radius;
    diamond_type = (type == "red" ? red : blue);
}
void Diamond::Draw() {
	Sprite::Draw();
}
void Diamond::Update(float deltaTime) {
    if(diamond_type == blue){
        if(Engine::Collider::IsRectOverlap(Position, Position+Size/2 , getPlayScene()->girl->Position, getPlayScene()->girl->Position + Size / 2) ){
            eaten = 1;
            getPlayScene()->DiamondGroup->RemoveObject(GetObjectIterator());
            getPlayScene()->score += 1000;
        }
    }
    else if(diamond_type == red) {
        // std::cout << "boy: " <<  Diamond::getPlayScene()->boy->Position.x << " " << getPlayScene()->boy->Position.y << " " << Diamond::getPlayScene()->boy->Position.x+32 << " " << getPlayScene()->boy->Position.y+32<< std::endl;
        // std::cout <<"R_diamond: "<< Position.x << " " << Position.y << " " << Position.x+32 << " " << Position.y+32 << std::endl;
        if(Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->boy->Position, getPlayScene()->boy->Position + Size / 2) ){
            eaten = 1;
            getPlayScene()->DiamondGroup->RemoveObject(GetObjectIterator());
            getPlayScene()->score += 1000;
        }
    }
}