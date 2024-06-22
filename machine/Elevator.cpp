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
Elevator::Elevator(std::string imgElevator,int x, int y, int init_x, int final_x, int num):
    Sprite(imgElevator, x, y), x(x), y(y), init_x(init_x), final_x(final_x), elevator_num(num){
}
void Elevator::Draw() {
	Sprite::Draw();
}
void Elevator::Update(float deltaTime) {
    std::list<IObject*> objects = getPlayScene()->ButtonGroup->GetObjects();
    for(auto obj:objects){
        Button* btn = dynamic_cast<Button*>(obj);
        if(btn->button_num == elevator_num){
            if(btn->started && x < final_x){
                getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
                getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x+1,y,init_x,final_x,elevator_num));
            }
            else if(!btn->started && x > init_x){
                getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
                getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x-1,y,init_x,final_x,elevator_num));
            }
        }
    }
    
}