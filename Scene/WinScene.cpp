#include <allegro5/allegro.h>
#include <functional>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <sstream>
#include <ctime>


#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"
#include "Engine/LOG.hpp"

void WinScene::Initialize() {
	ticks = 0;
	update_once = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("win/tay_win.png", halfW, halfH , 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 -10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	AudioHelper::SFXVolume = 0.6;
	bgmId = AudioHelper::PlayAudio("win.ogg");

	//name input
	playerName = "";
	playerNameLabel = new Engine::Label("Name: "+ playerName, "pirulen.ttf", 48, halfW, halfH / 4 +20, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(playerNameLabel);
	Engine::LOG(Engine::INFO) <<"basic check";

	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
}
void WinScene::Terminate() {
	AudioHelper::SFXVolume =1.0;
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
	ticks += deltaTime;
	// if (ticks > 4 && ticks < 100 &&
	// 	dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
	// 	ticks = 100;
	// 	bgmId = AudioHelper::PlayBGM("happy.ogg");
	// }
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	Engine::LOG(Engine::INFO)<<"key is "<<keyCode;
	if (keyCode == ALLEGRO_KEY_ENTER) {
		if(!update_once)WriteNametoFile(playerName);
		update_once++;
	}
	else if(keyCode == ALLEGRO_KEY_BACKSPACE){
		if (!playerName.empty()) {
            playerName.pop_back();
			playerNameLabel->Text = "name: "+playerName;
        }
	}
	else if (isalnum(keyCode-1+'a') && playerName.length() < 10) {
        // Append the character to the player name if it's alphanumeric and the name length is less than 10
        playerName += ('a'+ keyCode-1);
		playerNameLabel->Text = "name: "+ playerName;
    }
}

void WinScene::WriteNametoFile(std::string& playerName){
    const std::string filepath = "C:\\I2P\\Fireboy-and-Watergirl\\score.txt";
	std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    // Create a buffer to hold the formatted time
    char timeBuffer[20];
    strftime(timeBuffer, sizeof(timeBuffer), "%m-%d,%H:%M", ltm);


    file << " " << playerName << "-"<< timeBuffer;
	file.close();

	std::vector<std::pair<int, std::string>> entries;
    std::ifstream readFile(filepath);
    if (readFile.is_open()) {
        std::string line;
        while (std::getline(readFile, line)) {
            std::istringstream iss(line);
            int score;
            std::string name;
            if (iss >> score >> name) {
                entries.emplace_back(score, name);
            }
        }
        readFile.close();
	
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;});

		std::ofstream writeFile(filepath);
		if (writeFile.is_open()) {
			for (const auto& entry : entries) {
				writeFile << entry.first << " " << entry.second << "\n";
			}
			writeFile.close();
		}
    }
}
