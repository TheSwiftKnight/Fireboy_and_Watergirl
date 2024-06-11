#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <string>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include "Engine/IScene.hpp"

class WinScene final : public Engine::IScene {
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
public:
	std::string playerName;
	void OnKeyDown(int keyCode) override;
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void BackOnClick(int stage);
	void WriteNametoFile(std::string& playerName);
	Engine::Label* playerNameLabel;
	int update_once;
};

#endif // WINSCENE_HPP
