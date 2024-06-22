#include <allegro5/base.h>
#include <cmath>
#include <string>
#include <vector>
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
Elevator::Elevator(std::string imgElevator,int x, int y, int init_y, int final_y, int num, int opening):
    Sprite(imgElevator, x, y), x(x), y(y), init_y(init_y), final_y(final_y), elevator_num(num), opening(opening){
        
    getPlayScene()->mapState[(y-32)/64][x/64] = getPlayScene()->TILE_ELEVATOR;
            
}
void Elevator::Draw() {
	Sprite::Draw();
}
void Elevator::Update(float deltaTime) {
    std::list<IObject*> objects = getPlayScene()->ButtonGroup->GetObjects();
    bool start = 0;
    for(auto obj:objects){
        Button* btn = dynamic_cast<Button*>(obj);
        if(btn->started == 1){
            start = 1;
            break;
        }
    }
    if(start && y < final_y){
        getPlayScene()->mapState[init_y/64][x/64] = getPlayScene()->TILE_FLOOR;
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x,y+2,init_y,final_y,elevator_num,1));
    }
    else if(!start && y > init_y){
        getPlayScene()->mapState[init_y/64][x/64] = getPlayScene()->TILE_ELEVATOR;
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x,y-2,init_y,final_y,elevator_num,0));
        
    }
}