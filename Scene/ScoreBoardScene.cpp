#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Slider.hpp"
#include "ScoreBoardScene.hpp"
#include "Engine/LOG.hpp"

int halfW;
int halfH;

void ScoreBoardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
    page = 0;
    scores.clear();
    displayedScores.clear();

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 800, halfH * 7 / 4 - 50, 400,
                                  100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::PrevOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, halfW - 600, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 400, halfH * 7 / 4 - 50,
                                  400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::NextOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48, halfW + 600, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    
    AddNewObject(
        new Engine::Label("SCOREBOARD", "pirulen.ttf", 60, halfW, halfH / 6, 0, 202, 0, 255,
                          0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
    LoadScores();
}

void ScoreBoardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void ScoreBoardScene::PlayOnClick(int stage) {
    PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void ScoreBoardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void ScoreBoardScene::PrevOnClick() {
    if(page>0){
        page--;
        LoadScores();
    }
}
void ScoreBoardScene::NextOnClick() {
    page++;
    LoadScores();
}

void ScoreBoardScene::LoadScores() {
    scores.clear();
    const char* filepath = "C:\\I2P\\Fireboy-and-Watergirl\\score.txt";
    FILE* file;
    freopen_s(&file, filepath, "r", stdin);
    std::string line;

    char buffer[25];
    int currentLine = 0;
    int startLine = page * 10;
    int endLine = startLine + 10;

    while (fgets(buffer, sizeof(buffer), stdin)) { 
        if (currentLine >= startLine && currentLine < endLine) {
            scores.push_back(std::string(buffer)); // Store the line if it's within the page limits
        }else if (currentLine >= endLine) {
            break; // Stop reading if the end of the current page segment is reached
        }
        currentLine++;
    }

    // Close the file and reset stdin
    fclose(file);
    freopen_s(&file, "CON", "r", stdin);
    // Now display the scores
    if(!scores.empty())DisplayScores();
    else page--;
}

void ScoreBoardScene::DisplayScores(){
    for (auto& label : displayedScores) {
        RemoveObject(label); // Assuming a method like this exists to remove objects from the scene
    }
    displayedScores.clear();

    int startY = 150; // Start Y position for the first score
    int stepY = 50; // Vertical space between scores
    for (const auto& score : scores) {
        Engine::Label* scoreLabel = new Engine::Label(score, "pirulen.ttf", 24, halfW, startY, 255,255,255, 255, 0.5, 0.5);
        // AddNewObject(scoreLabel);
        // startY += stepY;
        AddNewObject(scoreLabel);
        displayedScores.push_back(scoreLabel->GetObjectIterator()); // Keep track of this label
        startY += stepY;
    }
}
