#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Elevator.hpp"
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
#include "Elevator.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
PlayScene* Elevator::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Elevator::Elevator(std::string imgElevator,int x, int y, float started):
    Sprite(imgElevator, x, y), x(x), y(y), started(started){
}
void Elevator::Draw() {
	Sprite::Draw();
}
void Elevator::Update(float deltaTime) {
    bool collision = Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->boy->Position, getPlayScene()->boy->Position + Size / 2) || \
       Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->girl->Position, getPlayScene()->girl->Position + Size / 2);
    
    if(!started && collision){
        std::cout << started << " " << collision << "\n";
        started = 1;
        std::cout << "start:" << started << "\n";
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator1.png", Position.x, Position.y, 1));
    }
    if(started && !collision){
        started = 0;
        std::cout << started << " " << collision << "\n";
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator0.png", Position.x, Position.y, 0));
    }
}