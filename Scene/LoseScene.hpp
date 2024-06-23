#ifndef LOSESCENE_HPP
#define LOSESCENE_HPP
#include <allegro5/allegro_audio.h>
#include <string>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include "Engine/IScene.hpp"

class LoseScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	std::string playerName;
	void OnKeyDown(int keyCode) override;
	explicit LoseScene() = default;
	void Initialize() override;
	void Terminate() override;
	void BackOnClick(int stage);
	void WriteNametoFile(std::string& playerName);
	int update_once;
};

#endif // LOSESCENE_HPP
