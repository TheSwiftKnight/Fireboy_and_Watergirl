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
#include "machine/Button.hpp"
PlayScene* Elevator::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Elevator::Elevator(std::string imgElevator,int x, int y, int init_x, int final_x):
    Sprite(imgElevator, x, y), x(x), y(y), init_x(init_x), final_x(final_x){
}
void Elevator::Draw() {
	Sprite::Draw();
}
void Elevator::Update(float deltaTime) {
    if(open && x != final_x){
        getPlayScene()->ButtonGroup->GetObjects()
    }
    getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
    getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",0,0,0,0));
    else if(!open && x!=init_x){

    }
}