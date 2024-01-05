#include "ReadyScene.h"
#include "Scene.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "Player.h"
#include <iostream>

ReadyScene::ReadyScene(class Game* game)
    :Scene(game)
{
}

ReadyScene::~ReadyScene()
{
}

void ReadyScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Ready");

    // クリアフラグを初期化
    mGame->SetGameClear(false);
    // 開始メッセージの作成
    mReadyMsg = new Actor(mGame);
    mReadyMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    auto* startMsgSprite = new SpriteComponent(mReadyMsg, 200);
    std::vector<int> rgb = { 255, 255, 255 };
    int size = 150;
    startMsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Ready...?", rgb, size));
    
    // プレーヤーの作成
    Player* player = new Player(mGame);
    player->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight - 200.0f));
    mGame->SetPlayer(player);

    // ポーズフラグをtrueへ
    mGame->SetPauseFlag(true);

    // ルーレットフラグをtrueへ
    mGame->SetRouletteFlag(true);

    // 初期化フラグをtrueへ
    mGame->SetInitFlag(true);

    mGame->SetGameLevel(0);

    startTime = SDL_GetTicks64();
}

void ReadyScene::Update(float deltaTime)
{
}

void ReadyScene::ProcessInput(const Uint8* state)
{
    int difftime = SDL_GetTicks64() - startTime;

    if (difftime > 400) {
        // スペースかエンター押下でゲームシーンに遷移
        if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN])
        {
            mGame->SetNextScene(new GameScene(mGame));
            mReadyMsg->SetState(Actor::EDead);
        }
    }
    
}

std::string const ReadyScene::GetSceneName()
{
    return "READY";
}