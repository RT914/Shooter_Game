#include "GameOverScene.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MenuScene.h"
#include "Scene.h"
#include "Enemy.h"
#include "Clock.h"
#include "Timer.h"
#include <iostream>


GameOverScene::GameOverScene(class Game* game)
    :Scene(game)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("GameOver");

    // ゲームオーバーメッセージの作成
    mGameOverMsg = new Actor(mGame);
    mGameOverMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    auto* gameoverMsgSprite = new SpriteComponent(mGameOverMsg, 200);
    std::vector<int> rgb = { 255, 255, 255 };
    int size = 100;
    gameoverMsgSprite->SetTexture(
        mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Game Over", rgb, size));

    // タイムを表記
    mRecordMsg = new Actor(mGame);
    mRecordMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.7));
    auto* recordMsgSprite = new SpriteComponent(mRecordMsg, 200);
    rgb = { 255, 255, 255 };
    size = 100;

    int difftime = mGame->GetTimer()->GetDiffTime();
    int n1 = 0;
    int n2 = 0;
    int minute_1 = 0;
    int minute_2 = 0;
    int second_1 = 0;
    int second_2 = 0;
    n1 = difftime / 1000 / 60;
    minute_1 = n1 % 60 / 10;
    minute_2 = n1 % 10;

    n2 = difftime / 1000;
    second_1 = n2 % 60 / 10;
    second_2 = n2 % 10;

    std::string record = std::to_string(minute_1) + std::to_string(minute_2) + ":" + std::to_string(second_1) + std::to_string(second_2);

    recordMsgSprite->SetTexture(
        mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", record, rgb, size));

}

void GameOverScene::Update(float deltaTime)
{
    
}

void GameOverScene::ProcessInput(const Uint8* state)
{
    // スペースかエンター押下でメニューシーンに遷移
    if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN])
    {
        mGame->SetNextScene(new MenuScene(mGame)); 
        mGameOverMsg->SetState(Actor::EDead);
        mRecordMsg->SetState(Actor::EDead);
        
        for (auto enemy : mGame->GetEnemies())
        {
            enemy->SetState(Actor::EDead);
        }
        

        for (auto clock : mGame->GetClocks())
        {
            clock->SetState(Actor::EDead);
        }

        // 音楽停止
        Mix_HaltMusic();

    }
}


std::string const GameOverScene::GetSceneName()
{
    return "GAMEOVER";
}