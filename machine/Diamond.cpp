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
#include "Diamond.hpp"

PlayScene* Diamond::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Diamond::Diamond(std::string imgDiamond,int x, int y):
    Sprite(imgDiamond, x, y), x(x), y(y){
    
}
void Diamond::Draw() const {
	Sprite::Draw();

}
void Diamond::CreateDiamond(){
    getPlayScene()->DiamondGroup->AddNewObject(new Diamond("play/red_diamond.png",x ,y));
}