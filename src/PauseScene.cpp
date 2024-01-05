#include "PauseScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "Scene.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Enemy.h"
#include "Clock.h"
#include "Timer.h"
#include <iostream>

int pause_start_time;

PauseScene::PauseScene(class Game* game)
    :Scene(game)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Pause");

    // メニューへ戻るボタンの作成
    mBackButton = new Actor(mGame);
    mBackButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* backbuttonSprite = new SpriteComponent(mBackButton, 200);
    backbuttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

    
    // 戻るボタンの作成
    mCancelButton = new Actor(mGame);
    mCancelButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
    auto* cancelbuttonSprite = new SpriteComponent(mCancelButton, 200);
    cancelbuttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

    // メニューへ戻るボタンのメッセージの作成
    mBackButtonMsg = new Actor(mGame);
    mBackButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* backbuttonmsgSprite = new SpriteComponent(mBackButtonMsg, 200);
    std::vector<int> rgb = { 255, 255, 255 };
    int size = 100;
    backbuttonmsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Menu", rgb, size));

    // メニューへ戻るボタンのメッセージの作成
    mCancelButtonMsg = new Actor(mGame);
    mCancelButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
    auto* cancelbuttonmsgSprite = new SpriteComponent(mCancelButtonMsg, 200);
    // std::vector<int> rgb = { 255, 255, 255 };
    // int size = 150;
    cancelbuttonmsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Game", rgb, size));

    // ポーズフラグをfalseへ
    if (mGame->GetPauseFlag()) {
        mGame->SetPauseFlag(false);
    }

    pause_start_time = SDL_GetTicks64();
    
}

void PauseScene::Update(float deltaTime)
{
    /*
    // 時間の更新
    int end = SDL_GetTicks64();
    // ゲーム開始からの経過時間の取得
    int diff_time = end - (mGame->GetTimer()->GetStartTime());
    mGame->GetTimer()->SetDiffTime(diff_time - mGame->GetRouletteTime());
    */
}

void PauseScene::ProcessInput(const Uint8* state)
{
    
    int back_button_scale = mBackButton->GetScale();
    int cancel_button_scale = mCancelButton->GetScale();
    Vector2 back_button_position = mBackButton->GetPosition();
    Vector2 cancel_button_position = mCancelButton->GetPosition();

    int weight, height;
    SDL_QueryTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button.png"), NULL, NULL, &weight, &height);

    // マウスの左クリックでReadySceneに遷移
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if ((back_button_position.x - weight / 2 <= mouseX && mouseX <= back_button_position.x + weight / 2) && (back_button_position.y - height / 2 <= mouseY && mouseY <= back_button_position.y + height / 2)) {
            mGame->SetNextScene(new MenuScene(mGame));
            mBackButton->SetState(Actor::EDead);
            mCancelButton->SetState(Actor::EDead);
            mCancelButtonMsg->SetState(Actor::EDead);
            mBackButtonMsg->SetState(Actor::EDead);
            mGame->GetPlayer()->SetState(Actor::EDead);

            for (auto enemy : mGame->GetEnemies())
            {
                enemy->SetState(Actor::EDead);
            }

            for (auto clock : mGame->GetClocks())
            {
                clock->SetState(Actor::EDead);
            }

            Mix_HaltMusic();

            // SEの再生
            Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
            SDL_Delay(400);

            /*
            for (auto enemy : mGame->GetEnemies())
            {
                enemy->SetState(Actor::EDead);
            }
            */


        }else if ((cancel_button_position.x - weight / 2 <= mouseX && mouseX <= cancel_button_position.x + weight / 2) && (cancel_button_position.y - height / 2 <= mouseY && mouseY <= cancel_button_position.y + height / 2)) {
            mGame->SetNextScene(new GameScene(mGame));
            mBackButton->SetState(Actor::EDead);
            mCancelButton->SetState(Actor::EDead);
            mCancelButtonMsg->SetState(Actor::EDead);
            mBackButtonMsg->SetState(Actor::EDead);

            for (auto enemy : mGame->GetEnemies())
            {
                enemy->SetState(Actor::EActive);
            }

            mGame->SetPauseFlag(true);

            // SEの再生
            Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
            SDL_Delay(400);

            // ルーレット画面の時間の追加
            mGame->AddRouletteTime(SDL_GetTicks64() - pause_start_time);

            /*
            for (auto clock : mGame->GetClocks())
            {
                clock->SetState(Actor::EActive);
            }
            */
        }
        
    }

    int difftime = SDL_GetTicks64() - startTime;

    if (difftime > 400) {

        if (state[SDL_SCANCODE_W])
        {
            NextButtonNum = 0;
        }
        else if (state[SDL_SCANCODE_S]) {
            NextButtonNum = 1;
        }
        else if (state[SDL_SCANCODE_SPACE]) {
            if (NowButtonNum == 0) {
                mGame->SetNextScene(new MenuScene(mGame));
                mBackButton->SetState(Actor::EDead);
                mCancelButton->SetState(Actor::EDead);
                mCancelButtonMsg->SetState(Actor::EDead);
                mBackButtonMsg->SetState(Actor::EDead);
                mGame->GetPlayer()->SetState(Actor::EDead);

                for (auto enemy : mGame->GetEnemies())
                {
                    enemy->SetState(Actor::EDead);
                }

                for (auto clock : mGame->GetClocks())
                {
                    clock->SetState(Actor::EDead);
                }

                Mix_HaltMusic();

                // SEの再生
                Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
                SDL_Delay(400);
            }
            else if (NowButtonNum == 1) {
                mGame->SetNextScene(new GameScene(mGame));
                mBackButton->SetState(Actor::EDead);
                mCancelButton->SetState(Actor::EDead);
                mCancelButtonMsg->SetState(Actor::EDead);
                mBackButtonMsg->SetState(Actor::EDead);

                for (auto enemy : mGame->GetEnemies())
                {
                    enemy->SetState(Actor::EActive);
                }

                mGame->SetPauseFlag(true);

                // SEの再生
                Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
                SDL_Delay(400);

                // ルーレット画面の時間の追加
                mGame->AddRouletteTime(SDL_GetTicks64() - pause_start_time);
            }
        }

    }

    if (NextButtonNum == 0 && NowButtonNum != 0) {

        mBackButton->SetState(Actor::EDead);
        mBackButtonMsg->SetState(Actor::EDead);

        mBackButton = new Actor(mGame);
        mBackButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* backbuttonSprite = new SpriteComponent(mBackButton, 200);
        backbuttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

        mBackButtonMsg = new Actor(mGame);
        mBackButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* backbuttonmsgSprite = new SpriteComponent(mBackButtonMsg, 200);
        std::vector<int> rgb = { 255, 255, 255 };
        int size = 100;
        backbuttonmsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Menu", rgb, size));


        mCancelButton->SetState(Actor::EDead);
        mCancelButtonMsg->SetState(Actor::EDead);

        mCancelButton = new Actor(mGame);
        mCancelButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* CancelButtonSprite = new SpriteComponent(mCancelButton, 200);
        CancelButtonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

        mCancelButtonMsg = new Actor(mGame);
        mCancelButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* CancelButtonImageSprite = new SpriteComponent(mCancelButtonMsg, 200);
        CancelButtonImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Game", rgb, size));

        NowButtonNum = 0;
    }
    else if (NextButtonNum == 1 && NowButtonNum != 1)
    {

        mBackButton->SetState(Actor::EDead);
        mBackButtonMsg->SetState(Actor::EDead);

        mBackButton = new Actor(mGame);
        mBackButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* backbuttonSprite = new SpriteComponent(mBackButton, 200);
        backbuttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

        mBackButtonMsg = new Actor(mGame);
        mBackButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* backbuttonmsgSprite = new SpriteComponent(mBackButtonMsg, 200);
        std::vector<int> rgb = { 255, 255, 255 };
        int size = 100;
        backbuttonmsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Menu", rgb, size));


        mCancelButton->SetState(Actor::EDead);
        mCancelButtonMsg->SetState(Actor::EDead);

        mCancelButton = new Actor(mGame);
        mCancelButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* CancelButtonSprite = new SpriteComponent(mCancelButton, 200);
        CancelButtonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

        mCancelButtonMsg = new Actor(mGame);
        mCancelButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* CancelButtonImageSprite = new SpriteComponent(mCancelButtonMsg, 200);
        CancelButtonImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Back to Game", rgb, size));

        NowButtonNum = 1;
    }
}


std::string const PauseScene::GetSceneName()
{
    return "PAUSE";
}