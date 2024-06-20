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
#include "Twins.hpp"
#include "Engine/LOG.hpp"


PlayScene* Twins::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Twins::Twins(std::string imgTwins, int x, int y, float radius) : Sprite(imgTwins, x, y),x(x),y(y){
	speed = 2;
	jump = false;
	CollisionRadius = radius;
}
void Twins::Update(float deltaTime) {
	switch(dir){
		case UP:
			Position.y-=speed;
			break;
		case DOWN:
			Position.y+=speed;
			break;
		case RIGHT:
			Position.x+=speed;
			break;
		case LEFT:
			Position.x-=speed;
			break;
	}
	PlayScene* scene = getPlayScene();
	if (!Enabled)
		return;
}
void Twins::MeDraw() {
	if (moveCD%4 == 0 || moveCD%4 == 1) {
		Sprite::TDraw(sourceX,0);
	}
	else{
		Sprite::TDraw(sourceX+1,0);
	}
}

void Twins::updateTime(int deltaTime){
	moveCD = deltaTime;
}

void Twins::OnKeyDown(int keyCode){
	active = true;
	switch(keyCode){
		case ALLEGRO_KEY_UP:
		case ALLEGRO_KEY_W:
			sourceX = 0;
			dir = UP;
			// Position.y-=speed;
			break;
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_A:
			sourceX = 4;
			dir = LEFT;
			// Position.x-=speed;
			break;
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_D:
			sourceX = 2;
			dir = RIGHT;
			// Position.x+=speed;
			break;
		default: active = false; break;
	}
}
void Twins::OnKeyUp(int keyCode){
	switch(keyCode){
		case ALLEGRO_KEY_UP:
		case ALLEGRO_KEY_W:
			if(dir == UP){
				dir = NO;
				sourceX = 0;
			}
			break;
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_A:
			if(dir == LEFT){
				dir = NO;
				sourceX = 0;
			}
			break;
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_D:
			if(dir == RIGHT){
				dir = NO;
				sourceX = 0;
			}
			break;
	}
}