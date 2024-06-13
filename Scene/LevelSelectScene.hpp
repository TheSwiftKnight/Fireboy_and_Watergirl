#ifndef LEVELSELECTSCENE_HPP
#define LEVELSELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class LevelSelectScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	explicit LevelSelectScene() = default;
	void Initialize() override;
	void Terminate() override;
	void PlayOnClick(int stage);
    void ScoreboardOnClick();
	void BackOnClick(int stage);
};

#endif // STAGESELECTSCENE_HPP
