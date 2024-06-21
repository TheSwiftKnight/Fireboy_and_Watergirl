#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Button.hpp"

PlayScene* Button::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Button::Button(std::string imgButton,int x, int y, float radius):
    Sprite(imgButton, x, y), x(x), y(y), imgButton(imgButton, x, y){
    CollisionRadius = radius;
}
void Button::Draw() const {
	Sprite::Draw();
    imgButton.Draw();
}
void Button::CreateButton(){
    getPlayScene()->ButtonGroup->AddNewObject(new Button("play/button0.png",x ,y, 1));
}