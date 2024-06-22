#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy/Enemy.hpp"
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
	Sprite::Update(deltaTime);
    if(diamond_type == red){
        if(Engine::Collider::IsPointInRect(Engine::Point(Diamond::getPlayScene()->girl->x,Diamond::getPlayScene()->girl->y),Engine::Point(x,y),Engine::Point(64,64))){
            eaten = 1;
            getPlayScene()->DiamondGroup->RemoveObject(GetObjectIterator());
        }
    }
    else if(diamond_type == blue) {
        if(Engine::Collider::IsPointInRect(Engine::Point(Diamond::getPlayScene()->boy->x,getPlayScene()->boy->y),Engine::Point(x,y),Engine::Point(64,64))){
            eaten = 1;
            getPlayScene()->DiamondGroup->RemoveObject(GetObjectIterator());
        }
    }
}