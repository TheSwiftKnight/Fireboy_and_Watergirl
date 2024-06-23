#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <sstream>
#include <chrono>

#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/Turret.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/Shovel.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/MissileTurret.hpp"
#include "Turret/MisteryTurret.hpp"
#include "UI/Animation/Plane.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "PlayScene.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/MisteryEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Turret/TurretButton.hpp"
#include "Engine/LOG.hpp"
#include "Engine/IObject.hpp"
#include "Twins/Twins.hpp"
#include "machine/Diamond.hpp"
#include "machine/Button.hpp"
#include "machine/Elevator.hpp"
#include "machine/Lever.hpp"
#include "machine/Stone.hpp"
#include "machine/Switch.hpp"
bool write_score_once = false;
bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
// const int PlayScene::MapWidth = 32, PlayScene::MapHeight = 24;
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
	// TODO: [HACKATHON-3-BUG] (1/5): There's a bug in this file, which crashes the game when you lose. Try to find it.
	// TODO: [HACKATHON-3-BUG] (2/5): Find out the cheat code to test.
    // TODO: [HACKATHON-3-BUG] (2/5): It should generate a Plane, and add 10000 to the money, but it doesn't work now.
	mapState.clear();
	keyStrokes.clear();
	spriteTick = 0;
	ticks = 0;
	deathCountDown = -1;
	SpeedMult = 1;
	score = 1000;
	ElapsedTime = 0.0f;
	score = 0;
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	AddNewObject(ButtonGroup = new Group());
	AddNewObject(DiamondGroup = new Group());
	AddNewObject(ElevatorGroup = new Group());
	AddNewObject(SwitchGroup = new Group());
	AddNewObject(LeverGroup = new Group());
	AddNewObject(StoneGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	
	ReadMap();
	// ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	preview = nullptr;
	UIGroup->AddNewObject(imgTarget);
	// Preload Lose Scene
	deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
	Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
	// Start BGM.
	bgmId = AudioHelper::PlayBGM("play.ogg");

	boy = new Twins("boy.png", 96, 500,1,BOY);
	girl = new Twins("girl.png", 96, 700,1,GIRL);
	// AddNewObject(boy);
	// AddNewObject(girl);
}
void PlayScene::Terminate() {
	AudioHelper::StopBGM(bgmId);
	AudioHelper::StopSample(deathBGMInstance);
	deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void PlayScene::UpdateTimer(){
	int minutes = static_cast<int>(ElapsedTime) / 60;
    int seconds = static_cast<int>(ElapsedTime) % 60;
    std::string timeStr = std::string("Time ") + 
                          (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + 
                          (seconds < 10 ? "0" : "") + std::to_string(seconds);
    UITime->Text = timeStr;
}

void PlayScene::Update(float deltaTime) {
	ElapsedTime += deltaTime;
	UpdateTimer();
	spriteTick+=0.1;
	boy->updateTime(spriteTick);
	girl->updateTime(spriteTick);
	UIScore->Text = std::string("Score ") + std::to_string(this->score);
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	if (SpeedMult == 0)
		deathCountDown = -1;
	else if (deathCountDown != -1)
		SpeedMult = 1;
	// Calculate danger zone.
	std::vector<float> reachEndTimes;
	for (auto& it : EnemyGroup->GetObjects()) {
		reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
	}
	// Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
	std::sort(reachEndTimes.begin(), reachEndTimes.end());
	for (int i = 0; i < SpeedMult; i++) {
		IScene::Update(deltaTime);
		// Check if we should create new enemy.
		ticks += deltaTime;
	}
	// int Px = ((int)(boy->Position.x)%64==0) ? (boy->Position.x)/64:(boy->Position.x)/64+1;
	// int Py = ((int)(boy->Position.y)%64==0) ? (boy->Position.y)/64:(boy->Position.y)/64+1;
	int Px = boy->Position.x;
	int Py = boy->Position.y;
	// Engine::LOG(Engine::INFO) << "Pos("<<Px/64<<","<<Py/64<<")";
	if(!boy->jump && !boy->arrived){
		if(mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_DIRT &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_RED_WATER &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_BLUE_WATER &&
		mapState[(Py+1+20)/64][(Px+12)/64]!=TILE_ELEVATOR &&
		mapState[(Py+1+45)/64][(Px+12)/64]!=TILE_SWITCH &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_STONE &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_GREEN_WATER &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_DIRT &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_RED_WATER &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_BLUE_WATER &&
		mapState[(Py+1+20)/64][(Px+30)/64]!=TILE_ELEVATOR &&
		mapState[(Py+1+45)/64][(Px+30)/64]!=TILE_SWITCH &&
		mapState[(Py+1+45)/64][(Px+30)/64]!=TILE_STONE &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_GREEN_WATER)boy->YUpdate();
	}
	else if(!boy->arrived){
		if(mapState[(Py-2)/64][(Px+1)/64]!=TILE_DIRT &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_RED_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_BLUE_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_ELEVATOR &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_SWITCH &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_GREEN_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_STONE &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_DIRT &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_RED_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_BLUE_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_ELEVATOR &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_SWITCH &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_GREEN_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_STONE)boy->YUpdate();
		else {
			boy->jumpTimestamp = true;
			boy->jump = false;
		}
	}
	
	switch(boy->dir){
		case RIGHT:
			// Engine::LOG(Engine::INFO) << "Pos"<<Px<<","<<Py;
			if(!boy->arrived&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_DIRT&&
			   mapState[(Py+55)/64][(Px+43+2)/64]!=TILE_DIRT &&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_ELEVATOR&&
			   mapState[(Py+15)/64][(Px+43+2)/64]!=TILE_ELEVATOR&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_SWITCH&&
			   mapState[(Py+40)/64][(Px+43+2)/64]!=TILE_SWITCH&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_STONE&&
			   mapState[(Py+40)/64][(Px+43+2)/64]!=TILE_STONE)
				boy->XUpdate();
			break;
		case LEFT:
		// Engine::LOG(Engine::INFO) << "Pos"<<Px<<","<<Py;
			if(!boy->arrived&&
			   mapState[Py/64][(Px-2)/64]!=TILE_DIRT&&
			   mapState[(Py+55)/64][(Px-2)/64]!=TILE_DIRT&&
			   mapState[Py/64][(Px-2)/64]!=TILE_ELEVATOR&&
			   mapState[(Py+15)/64][(Px-2)/64]!=TILE_ELEVATOR&&
			   mapState[Py/64][(Px-2)/64]!=TILE_SWITCH&&
			   mapState[(Py+40)/64][(Px-2)/64]!=TILE_SWITCH&&
			   mapState[Py/64][(Px-2)/64]!=TILE_STONE&&
			   mapState[(Py+40)/64][(Px-2)/64]!=TILE_STONE)
				boy->XUpdate();
			break;
	}


	Px = girl->Position.x;
	Py = girl->Position.y;

	if(!girl->jump && !girl->arrived){
		if(mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_DIRT &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_RED_WATER &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_BLUE_WATER &&
		mapState[(Py+1+20)/64][(Px+12)/64]!=TILE_ELEVATOR &&
		mapState[(Py+1+45)/64][(Px+12)/64]!=TILE_SWITCH &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_STONE &&
		mapState[(Py+1+60)/64][(Px+12)/64]!=TILE_GREEN_WATER &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_DIRT &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_RED_WATER &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_BLUE_WATER &&
		mapState[(Py+1+20)/64][(Px+30)/64]!=TILE_ELEVATOR &&
		mapState[(Py+1+45)/64][(Px+30)/64]!=TILE_SWITCH &&
		mapState[(Py+1+45)/64][(Px+30)/64]!=TILE_STONE &&
		mapState[(Py+1+60)/64][(Px+30)/64]!=TILE_GREEN_WATER)girl->YUpdate();
	}
	else if(!girl->arrived){
		if(mapState[(Py-2)/64][(Px+1)/64]!=TILE_DIRT &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_RED_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_BLUE_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_ELEVATOR &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_SWITCH &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_GREEN_WATER &&
		mapState[(Py-2)/64][(Px+1)/64]!=TILE_STONE &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_DIRT &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_RED_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_BLUE_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_ELEVATOR &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_SWITCH &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_GREEN_WATER &&
		mapState[(Py-2)/64][(Px+43-1)/64]!=TILE_STONE)girl->YUpdate();
		else {
			girl->jumpTimestamp = true;
			girl->jump = false;
		}
	}
	
	switch(girl->dir){
		case RIGHT:
			// Engine::LOG(Engine::INFO) << "Pos"<<Px<<","<<Py;
			if(!girl->arrived&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_DIRT&&
			   mapState[(Py+55)/64][(Px+43+2)/64]!=TILE_DIRT &&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_ELEVATOR&&
			   mapState[(Py+15)/64][(Px+43+2)/64]!=TILE_ELEVATOR&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_SWITCH&&
			   mapState[(Py+40)/64][(Px+43+2)/64]!=TILE_SWITCH&&
			   mapState[Py/64][(Px+43+2)/64]!=TILE_STONE&&
			   mapState[(Py+40)/64][(Px+43+2)/64]!=TILE_STONE)
				girl->XUpdate();
			break;
		case LEFT:
		// Engine::LOG(Engine::INFO) << "Pos"<<Px<<","<<Py;
			if(!girl->arrived&&
			   mapState[Py/64][(Px-2)/64]!=TILE_DIRT&&
			   mapState[(Py+55)/64][(Px-2)/64]!=TILE_DIRT&&
			   mapState[Py/64][(Px-2)/64]!=TILE_ELEVATOR&&
			   mapState[(Py+15)/64][(Px-2)/64]!=TILE_ELEVATOR&&
			   mapState[Py/64][(Px-2)/64]!=TILE_SWITCH&&
			   mapState[(Py+40)/64][(Px-2)/64]!=TILE_SWITCH&&
			   mapState[Py/64][(Px-2)/64]!=TILE_STONE&&
			   mapState[(Py+40)/64][(Px-2)/64]!=TILE_STONE)
				girl->XUpdate();
			break;
	}
}
void PlayScene::Draw() const {
	IScene::Draw();
	boy->MeDraw();
	girl->MeDraw();
	// if (DebugMode) {
	// 	// Draw reverse BFS distance on all reachable blocks.
	// 	for (int i = 0; i < MapHeight; i++) {
	// 		for (int j = 0; j < MapWidth; j++) {
	// 			if (mapDistance[i][j] != -1) {
	// 				// Not elegant nor efficient, but it's quite enough for debugging.
	// 				Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
	// 				label.Anchor = Engine::Point(0.5, 0.5);
	// 				label.Draw();
	// 			}
	// 		}
	// 	}
	// }
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
		if (mapState[y][x] == TILE_OCCUPIED && preview->Tool) {
			std::list<std::pair<bool, Engine::IObject*>> pair;
			for(auto t:TowerGroup->GetObjects()){
				pair.emplace_back(true,t);
			}
			for(auto t:pair){
				if(((int)(t.second->Position.x/BlockSize))==x && ((int)(t.second->Position.y/BlockSize))==y){
					TowerGroup->RemoveObject(t.second->GetObjectIterator());
					mapState[y][x] = TILE_FLOOR;
				}
			}
			// EarnMoney(50);
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;
			OnMouseMove(mx, my);
		}
		else if (mapState[y][x] != TILE_OCCUPIED) {
			if (!preview || preview->Tool)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			// EarnMoney(-preview->GetPrice());
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			TowerGroup->AddNewObject(preview);
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;

			mapState[y][x] = TILE_OCCUPIED;
			OnMouseMove(mx, my);
		}
	}
}
void PlayScene::OnKeyUp(int keyCode) {
	IScene::OnKeyUp(keyCode);
	if(keyCode == ALLEGRO_KEY_UP){
		girl->OnKeyUp(keyCode);
	}
	else if(keyCode == ALLEGRO_KEY_LEFT){
		girl->OnKeyUp(keyCode);
	}
	else if(keyCode == ALLEGRO_KEY_RIGHT){
		girl->OnKeyUp(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_W) {
		boy->OnKeyUp(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_A){
		boy->OnKeyUp(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_D){
		boy->OnKeyUp(keyCode);
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
		DebugMode = !DebugMode;
	}
	else if(keyCode == ALLEGRO_KEY_DOWN){
		girl->OnKeyDown(keyCode);
	}
	else if(keyCode == ALLEGRO_KEY_UP){
		girl->OnKeyDown(keyCode);
	}
	else if(keyCode == ALLEGRO_KEY_LEFT){
		girl->OnKeyDown(keyCode);
	}
	else if(keyCode == ALLEGRO_KEY_RIGHT){
		girl->OnKeyDown(keyCode);
	}
	// else {
	// 	keyStrokes.push_back(keyCode);
	// 	if (keyStrokes.size() > code.size())
	// 		keyStrokes.pop_front();
	// 	if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
	// 		auto it = keyStrokes.begin();
	// 		for (int c : code) {
	// 			if (!((*it == c) ||
	// 				(c == ALLEGRO_KEYMOD_SHIFT &&
	// 				(*it == ALLEGRO_KEY_LSHIFT || *it == ALLEGRO_KEY_RSHIFT))))
	// 				return;
	// 			++it;
	// 		}
	// 		EffectGroup->AddNewObject(new Plane());
	// 		money +=10000;
	// 	}
	// }
	else if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for MachineGunTurret.
		UIBtnClicked(0);
	}
	else if (keyCode == ALLEGRO_KEY_W) {
		boy->OnKeyDown(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_A){
		boy->OnKeyDown(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_S){
		boy->OnKeyDown(keyCode);
	}
	else if (keyCode == ALLEGRO_KEY_D){
		boy->OnKeyDown(keyCode);
	}
	// TODO: [CUSTOM-TURRET]: Make specific key to create the turret.
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
}
void PlayScene::Hit() {
	WriteScoretoFile(score);
	Engine::GameEngine::GetInstance().ChangeScene("lose");
}

void PlayScene::Arrived() {
	if(boy->arrived && girl->arrived){
		WriteScoretoFile(score);
		Engine::GameEngine::GetInstance().ChangeScene("win");
	}
	else return;
}

int PlayScene::GetScore(){
	return score;
}
void PlayScene::ReadMap() {
	std::string filename = std::string("Resource/level") + std::to_string(MapId) + ".txt";
	// Read map file.
	char c;
	std::vector<char> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '#': 
		case '-': 
		case '[':
		case ']':
		case '1':
		case '2':
		case 'B':
		case 'R':
		case 'G':
		case 'L':
		case 'E': 
		case 'b': mapData.push_back(c); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.1");
			break;
		default: mapData.push_back(c); break;
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.3");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	int btn_num = 0, elevator_num = 0;
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			char num = mapData[i * MapWidth + j];
			if (num == '-'){
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				mapState[i][j] = TILE_FLOOR;
			}
			else if(num == '#'){
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				mapState[i][j] = TILE_DIRT;
			}		
			else if(num == 'R'){
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				AddNewObject(new Twins("play/red_water.png", j * BlockSize+32, i * BlockSize+32, 1,NEITHER));
				mapState[i][j] = TILE_RED_WATER;
			}
			else if(num == 'B'){
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				AddNewObject(new Twins("play/blue_water.png", j * BlockSize+32, i * BlockSize+32, 1,NEITHER));
				mapState[i][j] = TILE_BLUE_WATER;
			}
			else if(num == 'G'){
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				AddNewObject(new Twins("play/green_water.png", j * BlockSize+32, i * BlockSize+32, 1,NEITHER));
				mapState[i][j] = TILE_GREEN_WATER;
			}
			else if(num == 'E'){
				mapState[i][j] = TILE_ELEVATOR;
				mapState[i][j-1] = TILE_ELEVATOR;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize - 64, i * BlockSize, BlockSize, BlockSize));
				int count = 0;
				AddNewObject(new Elevator("play/elevator.png", j * BlockSize, i * BlockSize+75, i * BlockSize+75, i * BlockSize + 128+ 75, 0, 0));
			}
			else if(num == 'L'){
				mapState[i][j] = TILE_LEVER;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				AddNewObject(new Lever("play/lever_left.png", j * BlockSize+32, i * BlockSize +32, 0, 0,NEITHER));
			}
			else if(num == '1'){
				mapState[i][j] = TILE_BLUE_DOOR;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				TileMapGroup->AddNewObject(new Engine::Image("play/blue_door.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			}
			else if(num == '2'){
				mapState[i][j] = TILE_RED_DOOR;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				TileMapGroup->AddNewObject(new Engine::Image("play/red_door.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			}
			else if(num == '['){
				mapState[i][j] = TILE_DIAMOND;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				DiamondGroup->AddNewObject(new Diamond("play/blue_diamond.png", j * BlockSize+32, i * BlockSize+32, 1, "blue"));
			}
			else if(num == ']'){
				mapState[i][j] = TILE_DIAMOND;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				DiamondGroup->AddNewObject(new Diamond("play/red_diamond.png", j * BlockSize+32, i * BlockSize+32, 1, "red"));
			}
			else if(num == 'S'){
				mapState[i][j] = TILE_STONE;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				StoneGroup->AddNewObject(new Stone("play/stone.png", j * BlockSize+32, i * BlockSize+32, BlockSize));
			}
			else if(num == 'b'){
				mapState[i][j] = TILE_BUTTON;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				ButtonGroup->AddNewObject(new Button("play/button0.png", j * BlockSize+32, i * BlockSize +32, BlockSize, 0));
			}
			else if(num == 's'){
				mapState[i][j-1] = TILE_SWITCH;
				mapState[i][j] = TILE_SWITCH;
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize - 64, i * BlockSize, BlockSize, BlockSize));
				AddNewObject(new Switch("play/switch.png", j * BlockSize, i * BlockSize+52-21, j * BlockSize, j * BlockSize -128+1, 0, 0));
			}
		}
	}
	// for(int i=0;i<MapHeight;i++){
	// 	for(int j=0;j<MapWidth;j++){
	// 		std::cout << mapState[i][j];
	// 	}
	// 	std::cout << "\n";
	// }
	// boy->getMapState(mapState,MapHeight,MapWidth);
	// girl->getMapState(mapState,MapHeight,MapWidth);
}
void PlayScene::ReadEnemyWave() {
    // TODO: [HACKATHON-3-BUG] (3/5): Trace the code to know how the enemies are created.
    // TODO: [HACKATHON-3-BUG] (3/5): There is a bug in these files, which let the game only spawn the first enemy, try to fix it.
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	std::string line;
	int type,repeat;
	float wait;
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while(std::getline(fin,line)){
		//Engine::LOG(Engine::INFO) << line;
		std::stringstream iss(line);
        if (!(iss >> type >> wait >> repeat) || type<1) {
            // Log the error if any of the inputs are incorrect.
            Engine::LOG(Engine::ERROR) << "Error reading data from enemy wave file. Skipping bad input.";
            fin.clear();  // Clear error state
			continue;  // Skip the rest of the processing for this line
        }

		//Engine::LOG(Engine::INFO) << type<<' '<<wait<<' '<<repeat;
        // If inputs are correct, use the data.
        for (int i = 0; i < repeat; i++) {
            enemyWaveData.emplace_back(type, wait);
        }
	}

	fin.close();
}
void PlayScene::ConstructUI() {
	//title
	UIGroup->AddNewObject(new Engine::Image("play/game_title.png", 40, 600, 300, 78));
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/floor.png", 1280, 0, 320, 832));
	// Text
	UIGroup->AddNewObject(UITime = new Engine::Label(std::string("Time 00:00"), "pirulen.ttf", 32, 1294, 64));
	UIGroup->AddNewObject(new Engine::Label(std::string("Level ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIScore = new Engine::Label(std::string("Score ") + std::to_string(score), "pirulen.ttf", 24, 1294, 280));
	TurretButton* btn;
	// // Button 1
	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
	// 	Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0)
	// 	, 1294, 136, MachineGunTurret::Price);
	// // Reference: Class Member Function Pointer and std::bind.
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
	// UIGroup->AddNewControlObject(btn);
	// // Button 2
	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
	// 	Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0)
	// 	, 1370, 136, LaserTurret::Price);
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
	// UIGroup->AddNewControlObject(btn);
	// // Button 3
	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/tower-base.png", 1446, 136, 0, 0, 0, 0),
	// 	Engine::Sprite("play/turret-3.png", 1446, 136, 0, 0, 0, 0)
	// 	, 1446, 136, MissileTurret::Price);
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
	// UIGroup->AddNewControlObject(btn);
	// // TODO: [CUSTOM-TURRET]: Create a button to support constructing the turret.
	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/tower-base.png", 1522, 136, 0, 0, 0, 0),
	// 	Engine::Sprite("play/turret-4.png", 1522, 136, 0, 0, 0, 0)
	// 	, 1522, 136, MissileTurret::Price);
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
	// UIGroup->AddNewControlObject(btn);

	// btn = new TurretButton("play/floor.png", "play/dirt.png",
	// 	Engine::Sprite("play/gnome.png", 1294, 216, 0, 0, 0, 0),
	// 	Engine::Sprite("play/gnome.png", 1294, 216, 0, 0, 0, 0)
	// 	, 1294, 216, Shovel::Price);
	// btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
	// UIGroup->AddNewControlObject(btn);

	// int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	// int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	// int shift = 135 + 25;
	// dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
	// dangerIndicator->Tint.a = 0;
	// UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::UIBtnClicked(int id) {
	if (preview)
		UIGroup->RemoveObject(preview->GetObjectIterator());
    // TODO: [CUSTOM-TURRET]: On callback, create the turret.
	// if (id == 0 && money >= MachineGunTurret::Price)
	// 	preview = new MachineGunTurret(0, 0);
	// else if (id == 1 && money >= LaserTurret::Price)
	// 	preview = new LaserTurret(0, 0);
	// else if (id == 2 && money >= MissileTurret::Price)
	// 	preview = new MissileTurret(0, 0);
	// else if (id == 3 && money >= MisteryTurret::Price)
	// 	preview = new MisteryTurret(0, 0);
	// else if (id == 4 && money >= Shovel::Price)
	// 	preview = new Shovel(0, 0);
	// if (!preview)
	// 	return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	UIGroup->AddNewObject(preview);
	
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	// Push end point.
	// BFS from end point.
	if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
		return map;
	que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
	map[MapHeight - 1][MapWidth - 1] = 0;
	while (!que.empty()) {
		Engine::Point p = que.front();
		que.pop();
		// TODO: [BFS PathFinding] (1/1): Implement a BFS starting from the most right-bottom block in the map.
		//               For each step you should assign the corresponding distance to the most right-bottom block.
		//               mapState[y][x] is TILE_DIRT if it is empty.
		for (const Engine::Point& dir : directions) {
            int x = p.x + dir.x;
            int y = p.y + dir.y;

            // Check if neighbor is within map boundaries and is not occupied
            if (x >= 0 && x < MapWidth && y >= 0 && y < MapHeight && mapState[y][x] == TILE_DIRT) {
                // If neighbor is unvisited
                if (map[y][x] == -1) {
                    map[y][x] = map[p.y][p.x] + 1; // Set distance from bottom-right
                    que.push(Engine::Point(x, y)); // Enqueue neighbor
                }
            }
        }
	}
	return map;
}

void PlayScene::WriteScoretoFile(int score){
    const std::string filepath = "C:\\I2P\\Fireboy-and-Watergirl\\score.txt";
    
    // Open the file for appending using ofstream
    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }

    // Write the score to the file, followed by a newline
    file << "\n" << score;
}