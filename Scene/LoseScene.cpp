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
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/LOG.hpp"

Engine::Label* playerNameLabel;
void LoseScene::Initialize() {
	update_once = 0;

	// TODO: [HACKATHON-1-SCENE] (1/4): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	 AddNewObject(new Engine::Image("play/background.png",0, 00));
	AddNewObject(new Engine::Image("lose/rick_lose.png", halfW-500, halfH+25, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Image("lose/rick_lose.png", halfW, halfH+25, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Image("lose/rick_lose.png", halfW+500, halfH+25, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Lose :(", "pirulen.ttf", 48, halfW, halfH / 4 , 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("stage-select/back_1.png", "stage-select/back_2.png", halfW - 200, halfH * 7 / 4 - 80, 400, 100);
	btn->SetOnClickCallback(std::bind(&LoseScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
    // bgmInstance = AudioHelper::PlaySample("lose.ogg", false, AudioHelper::BGMVolume, PlayScene::DangerTime);
	bgmId = AudioHelper::PlayAudio("lose.ogg");
//name input
	playerName = "";
	playerNameLabel = new Engine::Label("Name: "+ playerName, "pirulen.ttf", 48, halfW, halfH / 4 +50, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(playerNameLabel);
	Engine::LOG(Engine::INFO) <<"basic check";
for(int i=0;i<1600;i+=100){
        AddNewObject(new Engine::Image("play/boy_char.png", i, 12*64, 38, 64));
	    AddNewObject(new Engine::Image("play/girl_char.png", i+50, 12*64, 38, 64));
    }
}
void LoseScene::Terminate() {
	AudioHelper::StopBGM(bgmId);
	// bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void LoseScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}


void LoseScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	// Engine::LOG(Engine::INFO)<<"key is "<<keyCode;
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

void LoseScene::WriteNametoFile(std::string& playerName){
	system("echo Current: %cd%");
    const std::string filepath = "../score.txt";
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
