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
#include "Button.hpp"
#include "Engine/Collider.hpp"
#include "Twins/Twins.hpp"
PlayScene* Button::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Button::Button(std::string imgbutton,int x, int y, float started):
    Sprite(imgbutton, x, y), x(x), y(y), started(started){
}
void Button::Draw() {
	Sprite::Draw();
}
void Button::Update(float deltaTime) {
    bool collision = Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->boy->Position, getPlayScene()->boy->Position + Size / 2) || \
       Engine::Collider::IsRectOverlap(Position, Position + Size / 2 , getPlayScene()->girl->Position, getPlayScene()->girl->Position + Size / 2);
    
    if(!started && collision){
        std::cout << started << " " << collision << "\n";
        started = 1;
        std::cout << "start:" << started << "\n";
        getPlayScene()->ButtonGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ButtonGroup->AddNewObject(new Button("play/button1.png", Position.x, Position.y, 1));
    }
    if(started && !collision){
        started = 0;
        std::cout << started << " " << collision << "\n";
        getPlayScene()->ButtonGroup->RemoveObject(GetObjectIterator());
        getPlayScene()->ButtonGroup->AddNewObject(new Button("play/button0.png", Position.x, Position.y, 0));
    }
}
bool Button::getStartedValue(){
    return started;
}