#include "RouletteScene.h"
#include "Scene.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "Timer.h"
#include "Math.h"
#include <iostream>

int start_time, end_time;


RouletteScene::RouletteScene(class Game* game)
    :Scene(game)
{
}

RouletteScene::~RouletteScene()
{
}

void RouletteScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Roulette");

    // ルーレットが画面中央に移動する処理
    mRouletteBlock = new Actor(mGame);
    mRouletteBlock->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    mRouletteBlock->SetScale(0.3);
    auto* rouletteblocksprite = new SpriteComponent(mRouletteBlock, 200);
    rouletteblocksprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "roulette_block_dot.png"));

    mRoulette = new Actor(mGame);
    mRoulette->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    mRoulette->SetScale(1.0);
    auto* sprite = new SpriteComponent(mRoulette, 200);
    int num = mGame->GetRoulette();

    switch (num) {
        case 0:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "kakinotane_dot.png"));
            break;

        case 1:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "haityu_dot.png"));
            break;

        case 2:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "pyuregumi_dot.png"));
            break;
    }

    start_time = SDL_GetTicks64();

    stopflag = false;

    if (mGame->GetRouletteFlag()) {
        mGame->SetRouletteFlag(false);
    }  

    SDL_Delay(200);
}

void RouletteScene::Update(float deltaTime)
{
    // ルーレットを動かす処理
    if (!stopflag) {
        mRoulette->SetState(Actor::EDead);
        mRoulette = new Actor(mGame);
        mRoulette->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
        mRoulette->SetScale(1);
        auto* sprite = new SpriteComponent(mRoulette, 200);

        int num = Math::GetRand(0, 3);
        mGame->SetRoulette(num);

        switch (num) {
        case 0:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "kakinotane_dot.png"));
            break;

        case 1:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "haityu_dot.png"));
            break;

        case 2:
            sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "pyuregumi_dot.png"));
            break;
        }
        

    }
    
}

void RouletteScene::ProcessInput(const Uint8* state)
{
    if (stopflag = true && mStopMsg != nullptr) {
        if (state[SDL_SCANCODE_SPACE] && SDL_GetTicks64() > mGame->GetReturnToTheRouletteTime() + 1000) {
            mStopMsg->SetState (Actor::EDead);
            mRoulette->SetState(Actor::EDead);
            mRouletteBlock->SetState(Actor::EDead);
            mGame->SetReturnToTheRouletteTime(SDL_GetTicks64());

            // ルーレット画面の時間の追加
            mGame->AddRouletteTime(SDL_GetTicks64() - start_time);
            
            mGame->SetRouletteFlag(true);

            // ゲームシーンへ戻る
            mGame->SetNextScene(new GameScene(mGame));
        }
    }
    else {
        if (state[SDL_SCANCODE_SPACE]) {
            // ルーレットストップメッセージの作成
            mStopMsg = new Actor(mGame);
            mStopMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2 + 300.0f));
            auto* stopMsgSprite = new SpriteComponent(mStopMsg, 200);
            std::vector<int> rgb = { 255, 255, 255 };
            int size = 150;
            stopMsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Stop", rgb, size));
            stopflag = true;
            mGame->SetReturnToTheRouletteTime(SDL_GetTicks64());
    
        }
    }


    
}

std::string const RouletteScene::GetSceneName()
{
    return "ROULETTE";
}