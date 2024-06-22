#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Lever.hpp"
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
#include "machine/Button.hpp"
PlayScene* Lever::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Lever::Lever(std::string imgLever,int x, int y, std::string s):
    Sprite(imgLever, x, y), x(x), y(y){
    if(s == "right") state = right;
    else if(s == "left") state = left;
    else state = middle;
}
void Lever::Draw() {
	Sprite::Draw();
}
void Lever::Update(float deltaTime) {
    bool collision = Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->boy->Position, getPlayScene()->boy->Position + Size / 2) || \
       Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->girl->Position, getPlayScene()->girl->Position + Size / 2);
    std::list<IObject*> objects = getPlayScene()->ButtonGroup->GetObjects();
    for(auto obj:objects){
        Button* btn = dynamic_cast<Button*>(obj);
        if(btn->button_num == Lever_num){
            if(btn->started && x < final_y){
                getPlayScene()->LeverGroup->RemoveObject(GetObjectIterator());
                getPlayScene()->LeverGroup->AddNewObject(new Lever("play/right_lever.png",x,y,"middle"));
            }
            else if(!btn->started && x > init_y){
                getPlayScene()->LeverGroup->RemoveObject(GetObjectIterator());
                getPlayScene()->LeverGroup->AddNewObject(new Lever("play/Lever.png",x,y,"middle"));
            }
        }
    }
    
}