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

PlayScene* Twins::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Twins::Twins(std::string imgTwins, int x, int y, float radius) : Sprite(imgTwins, x, y),x(x),y(y){
	speed = 5;
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
void Twins::Draw() const {
	if (Preview) {
		al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgba(0, 255, 0, 50));
	}
	Sprite::Draw();
}

void Twins::OnKeyDown(int keyCode){
	switch(keyCode){
		case ALLEGRO_KEY_DOWN:
		case ALLEGRO_KEY_S:
			dir = DOWN;
			// Position.y+=speed;
			break;
		case ALLEGRO_KEY_UP:
		case ALLEGRO_KEY_W:
			dir = UP;
			// Position.y-=speed;
			break;
		case ALLEGRO_KEY_LEFT:
		case ALLEGRO_KEY_A:
			dir = LEFT;
			// Position.x-=speed;
			break;
		case ALLEGRO_KEY_RIGHT:
		case ALLEGRO_KEY_D:
			dir = RIGHT;
			// Position.x+=speed;
			break;
	}
}
void Twins::OnKeyUp(int keyCode){
	dir = NO;
}