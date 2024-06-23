#include <allegro5/base.h>
#include <cmath>
#include <string>
#include <vector>
#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
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
#include "machine/Button.hpp"
PlayScene* Switch::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Switch::Switch(std::string imgSwitch,int x, int y, int init_y, int final_y, int num, int opening):
    Sprite(imgSwitch, x, y), x(x), y(y), init_y(init_y), final_y(final_y), Switch_num(num), opening(opening){
}
void Switch::Draw() {
	Sprite::Draw();
}
void Switch::Update(float deltaTime) {
    std::list<IObject*> objects = getPlayScene()->ButtonGroup->GetObjects();
    bool start = 0;
    for(auto obj:objects){
        Button* btn = dynamic_cast<Button*>(obj);
        if(btn->started == 1){
            start = 1;
            break;
        }
    }
    if(start){
        getPlayScene()->mapState[init_y/64][x/64] = getPlayScene()->TILE_FLOOR;
        if(y < final_y){
            getPlayScene()->SwitchGroup->RemoveObject(GetObjectIterator());
            getPlayScene()->SwitchGroup->AddNewObject(new Switch("play/Switch.png",x,y+2,init_y,final_y,Switch_num,1));
        }
    }
    else if(!start){
        // getPlayScene()->mapState[init_y/64][x/64] = getPlayScene()->TILE_SWITCH;
        // if(y > init_y) {
        //     getPlayScene()->SwitchGroup->RemoveObject(GetObjectIterator());
        //     getPlayScene()->SwitchGroup->AddNewObject(new Switch("play/Switch.png",x,y-2,init_y,final_y,Switch_num,0));
        // }
    }
}