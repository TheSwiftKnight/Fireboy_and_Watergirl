#include <allegro5/base.h>
#include <cmath>
#include <string>
#include <vector>
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Switch.hpp"
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
#include "Switch.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
#include "machine/Lever.hpp"
PlayScene* Switch::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Switch::Switch(std::string imgSwitch,int x, int y, int init_x, int final_x, int num, int opening):
    Sprite(imgSwitch, x, y), x(x), y(y), init_x(init_x), final_x(final_x), Switch_num(num), opened(opening){
    if(x != final_x) opened = 0;
}
void Switch::Draw() {
	Sprite::Draw();
}
void Switch::Update(float deltaTime) {
    std::list<IObject*> objects = getPlayScene()->LeverGroup->GetObjects();
    bool start = 0;
    for(auto obj:objects){
        Lever* lever = dynamic_cast<Lever*>(obj);
        if(lever->started == 1){
            start = 1;
            break;
        }
    }
    if(start && !opened){
        getPlayScene()->mapState[y/64][init_x/64] = getPlayScene()->TILE_FLOOR;
        getPlayScene()->mapState[y/64][init_x/64-1] = getPlayScene()->TILE_FLOOR;
        if(x > final_x){
            getPlayScene()->SwitchGroup->RemoveObject(GetObjectIterator());
            getPlayScene()->SwitchGroup->AddNewObject(new Switch("play/Switch.png",x-3,y,init_x,final_x,Switch_num,1));
        }
    }
}