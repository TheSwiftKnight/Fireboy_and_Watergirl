#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "StageSelectScene.hpp"

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    //background
    AddNewObject(new Engine::Image("play/background.png",0, 00));
    //back
    btn = new Engine::ImageButton("stage-select/back_1.png", "stage-select/back_2.png", halfW - 200, halfH * 3 / 2 , 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    //level1
    AddNewObject(new Engine::Image("stage-select/diamond.png",halfW - 257, halfH / 2-40 ));
    AddNewObject(new Engine::Image("stage-select/diamond.png",halfW + 170, halfH / 2-40 ));
    btn = new Engine::ImageButton("stage-select/level1_1.png", "stage-select/level1_2.png", halfW - 200, halfH / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Level 1", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));
    //level2
    AddNewObject(new Engine::Image("stage-select/diamond.png", halfW - 257, halfH /2 + 110));
    AddNewObject(new Engine::Image("stage-select/diamond.png", halfW + 170, halfH /2 + 110));
    btn = new Engine::ImageButton("stage-select/level2_1.png", "stage-select/level2_2.png", halfW - 200, halfH /2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Level 2", "pirulen.ttf", 48, halfW, halfH / 2 +150, 0, 0, 0, 255, 0.5, 0.5));
    //scoreboard
    btn = new Engine::ImageButton("stage-select/scoreboard_1.png", "stage-select/scoreboard_2.png", halfW - 200, halfH / 2 + 300, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 36, halfW, halfH / 2 + 300, 0, 0, 0, 255, 0.5, 0.5));
    //twins
    for(int i=0;i<1600;i+=100){
        AddNewObject(new Engine::Image("play/boy_char.png", i, 12*64, 38, 64));
	    AddNewObject(new Engine::Image("play/girl_char.png", i+50, 12*64, 38, 64));
    }
    for(int i=0;i<1600;i+=100){
        AddNewObject(new Engine::Image("play/boy_char.png", i, 0, 38, 64));
	    AddNewObject(new Engine::Image("play/girl_char.png", i+50, 0, 38, 64));
    }

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("start.ogg", true, AudioHelper::BGMVolume);
}
void StageSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void StageSelectScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void StageSelectScene::PlayOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void StageSelectScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreBoard");
}