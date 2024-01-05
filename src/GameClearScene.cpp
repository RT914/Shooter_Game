#include "GameClearScene.h"
#include <iostream>


GameClearScene::GameClearScene(class Game* game)
    :Scene(game)
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Start()
{
    std::cout << GetSceneName() << std::endl;
}

void GameClearScene::Update(float deltaTime)
{
}

void GameClearScene::ProcessInput(const Uint8* state)
{

}


std::string const GameClearScene::GetSceneName()
{
    return "GAMECLEAR";
}