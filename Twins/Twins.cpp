#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Twins.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Collider.hpp"
#include "Scene/PlayScene.hpp"


PlayScene* Twins::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Twins::Twins(std::string imgTwins, int x, int y, float radius,ID id) : Sprite(imgTwins, x, y),x(x),y(y),id(id){
	speed = 3;
	jump = false;
	velx = vely = 0;
	CollisionRadius = radius;
	jumpTimestamp = true;
}
void Twins::XUpdate(){
	Position.x += velx;
	checkHit();
}
void Twins::YUpdate() {
	if(jump && moveCD-jumpCD>=5){
		jumpTimestamp = true;
		jump = false;
	}
	if(!jump)vely=jumpspeed+gravity;
	else{
		vely = -jumpspeed;
		sourceX = 0;
	}
	Position.y += vely;
	checkHit();
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
	if(jumpTimestamp) jumpCD = deltaTime;
}

void Twins::OnKeyDown(int keyCode){
	switch(keyCode){
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_A:
			velx  = -speed;
			sourceX = 4;
			dir = LEFT;
			break;
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_D:
			velx = speed;
			sourceX = 2;
			dir = RIGHT;
			break;
		default: break;
	}
	switch(keyCode){
		case ALLEGRO_KEY_UP:
		case ALLEGRO_KEY_W:
			jump = true;
			jumpTimestamp = false;
			break;
	}
}
void Twins::OnKeyUp(int keyCode){
	switch(keyCode){
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_A:
			if(dir == LEFT){
				velx = 0;
				dir = NO;
				sourceX = 0;
			}
			break;
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_D:
			if(dir == RIGHT){
				velx = 0;
				dir = NO;
				sourceX = 0;
			}
			break;
	}
}

void Twins::checkHit(){
	PlayScene* scene = getPlayScene();
	if(id == GIRL&&
	  (scene->mapState[(Position.y+60)/64][(Position.x)/64]==scene->TILE_BLUE_DOOR ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64]==scene->TILE_BLUE_DOOR ||
	   scene->mapState[(Position.y)/64][(Position.x)/64]==scene->TILE_BLUE_DOOR ||
	   scene->mapState[(Position.y)/64][(Position.x+43)/64]==scene->TILE_BLUE_DOOR)){
		arrived = true;
		scene->Arrived();
	}
	else if(id == BOY&&
	  (scene->mapState[(Position.y+60)/64][(Position.x)/64]==scene->TILE_RED_DOOR ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64]==scene->TILE_RED_DOOR ||
	   scene->mapState[(Position.y)/64][(Position.x)/64]==scene->TILE_RED_DOOR ||
	   scene->mapState[(Position.y)/64][(Position.x+43)/64]==scene->TILE_RED_DOOR)){
		arrived = true;
		scene->Arrived();
	}
	if(scene->mapState[(Position.y+60)/64][(Position.x)/64]==scene->TILE_GREEN_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64]==scene->TILE_GREEN_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64-1]==scene->TILE_GREEN_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64-1]==scene->TILE_GREEN_WATER||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64+1]==scene->TILE_GREEN_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64+1]==scene->TILE_GREEN_WATER){
		scene->Hit();
		return;
	}
	if(id == BOY&&
	  (scene->mapState[(Position.y+60)/64][(Position.x)/64]==scene->TILE_BLUE_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64]==scene->TILE_BLUE_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64-1]==scene->TILE_BLUE_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64-1]==scene->TILE_BLUE_WATER||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64+1]==scene->TILE_BLUE_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64+1]==scene->TILE_BLUE_WATER)){
		scene->Hit();
		return;
	}
	if(id == GIRL&&
	  (scene->mapState[(Position.y+60)/64][(Position.x)/64]==scene->TILE_RED_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64]==scene->TILE_RED_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64-1]==scene->TILE_RED_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64-1]==scene->TILE_RED_WATER||
	   scene->mapState[(Position.y+60)/64][(Position.x)/64+1]==scene->TILE_RED_WATER ||
	   scene->mapState[(Position.y+60)/64][(Position.x+43)/64+1]==scene->TILE_RED_WATER)){
		scene->Hit();
		return;
	}
}