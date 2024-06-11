#ifndef ScoreBoardScene_HPP
#define ScoreBoardScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class ScoreBoardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit ScoreBoardScene() = default;

    void Initialize() override;

    void Terminate() override;

    void PlayOnClick(int stage);
    void BackOnClick(int stage);
    void PrevOnClick();
    void NextOnClick();
    void BGMSlideOnValueChanged(float value);

    void SFXSlideOnValueChanged(float value);

    int page;
    void LoadScores();
    std::vector<std::string> scores;
    void DisplayScores();
    std::vector<std::list<std::pair<bool, IObject*>>::iterator> displayedScores;
};

#endif // SettingsScene_HPP
