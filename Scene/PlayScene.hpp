#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class Turret;
class Twins;
namespace Engine {
	class Group;
	class Image;
	class Label;
	class Sprite;
}  // namespace Engine

class PlayScene final : public Engine::IScene {
private:
	float ElapsedTime;
	ALLEGRO_SAMPLE_ID bgmId;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
protected:
	int SpeedMult;
public:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_BLUE_DOOR,
		TILE_RED_DOOR,
		TILE_BUTTON,
		TILE_LEVER,
		TILE_ELEVATOR,
		TILE_STONE,
		TILE_RED_WATER,
		TILE_BLUE_WATER,
		TILE_GREEN_WATER,
		TILE_DIAMOND
	};
	int score;
	float spriteTick;
	static bool DebugMode;
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const float DangerTime;
	static const Engine::Point SpawnGridPoint;
	static const Engine::Point EndGridPoint;
	static const std::vector<int> code;
	int MapId;
	float ticks;
	float deathCountDown;
	// Map tiles.
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Group* WaterGroup;
	Group* ButtonGroup;
	Engine::Label* UITime;
	Group* DiamondGroup;
	Group* ElevatorGroup;
	Group* LeverGroup;
	Group* StoneGroup;
	Engine::Label* UIScore;
	Engine::Image* imgTarget;
	Engine::Sprite* dangerIndicator;
	Turret* preview;
	Twins* boy;
	Twins* girl;
	Twins* water;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<int>> mapDistance;
	std::list<std::pair<int, float>> enemyWaveData;
	std::list<int> keyStrokes;
	static Engine::Point GetClientSize();
	explicit PlayScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void OnKeyUp(int keyCode) override;
	void Hit();
	void Arrived();
	int GetScore();
	void ReadMap();
	void ReadEnemyWave();
	void ConstructUI();
	void UIBtnClicked(int id);
	bool CheckSpaceValid(int x, int y);
	void UpdateTimer();
	std::vector<std::vector<int>> CalculateBFSDistance();
	// void ModifyReadMapTiles();
	void WriteScoretoFile(int score);
};
#endif // PLAYSCENE_HPP
