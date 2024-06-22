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
Elevator::Elevator(std::string imgElevator,int x, int y, int init_x, int final_x, int num, int opening):
    Sprite(imgElevator, x, y), x(x), y(y), init_x(init_x), final_x(final_x), elevator_num(num), opening(opening){
        if(opening){
            std::cout << "elevator:" << x << " " << y << "\n";
            for(int i=init_x-128;i<=x-128;i+=64){
                std::cout << i << std::endl;
                getPlayScene()->mapState[(y-32)/64][i/64] = getPlayScene()->TILE_FLOOR;
            }
        }
        else{
            for(int i=x-128;i<final_x-128;i+=64){
                std::cout << i << std::endl;
                getPlayScene()->mapState[(y-32)/64][i/64] = getPlayScene()->TILE_ELEVATOR;
            }
        }
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
    if(start && x < final_x){
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x+1,y,init_x,final_x,elevator_num,1));
    }
    else if(!start && x > init_x){
        getPlayScene()->ElevatorGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ElevatorGroup->AddNewObject(new Elevator("play/Elevator.png",x-1,y,init_x,final_x,elevator_num,0));
        
    }
}