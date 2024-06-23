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
#include "Lever.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
PlayScene* Lever::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Lever::Lever(std::string imgLever,int x, int y,int mid_start, int final_start, int num):
    Sprite(imgLever, x, y), x(x), y(y), middle_started(mid_start),started(final_start),Lever_num(num){
}
void Lever::Draw() {
	Sprite::Draw();
}
void Lever::Update(float deltaTime) {
    bool mid_collision = Engine::Collider::IsPointInRect(getPlayScene()->boy->Position, Position-Engine::Point(32,32), Size) || \
       Engine::Collider::IsPointInRect(getPlayScene()->girl->Position, Position-Engine::Point(32,32), Size) ;
    bool final_collision = Engine::Collider::IsPointInRect(getPlayScene()->boy->Position, Position-Engine::Point(16,32), Size) || \
       Engine::Collider::IsPointInRect(getPlayScene()->girl->Position, Position-Engine::Point(16,32), Size) ;
    if(!middle_started && mid_collision){
        middle_started = 1;
        getPlayScene()->LeverGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->LeverGroup->AddNewObject(new Lever("play/lever_middle.png", Position.x, Position.y, 1, 0, Lever_num));
    }
    else if(middle_started && final_collision){
        started = 1;
        getPlayScene()->LeverGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->LeverGroup->AddNewObject(new Lever("play/lever_right.png", Position.x, Position.y, 1, 1, Lever_num));
    }
}
bool Lever::getStartedValue(){
    return started;
}