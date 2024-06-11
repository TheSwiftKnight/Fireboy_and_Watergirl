#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/Group.hpp"
#include "Shovel.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int Shovel::Price = 0;
Shovel::Shovel(float x, float y) :
	Turret("play/gnome.png", "play/gnome.png", x, y, 5, Price, 0) {
	this->Tool = true;
}

void Shovel::CreateBullet(){
}
