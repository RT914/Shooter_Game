#include <iostream>
#include <vector>
#include "GameScene.h"
#include "Game.h"
#include "Math.h"
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
#include "Clock.h"
#include "PauseScene.h"
#include "SpriteComponent.h"
#include "RouletteScene.h"
#include <string>

GameScene::GameScene(class Game* game)
    :Scene(game)
{
}

GameScene::~GameScene()
{
} 

void GameScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Game");

    // 初期化しておらず、ポーズから帰っていないとき
    if (mGame->GetInitFlag()) {
        // タイマークラスの作成
        Timer* timer = new Timer(mGame);
        mGame->SetTimer(timer);
        

        // 開始時間を取得
        int start = SDL_GetTicks64();
        // std::cout << start << std::endl;
        mGame->GetTimer()->SetStartTime(start);

        
        // クロッククラスの作成 for分でもできるかも
        auto clock1 = new Clock(mGame);
        auto clock2 = new Clock(mGame);
        auto clock3 = new Clock(mGame);
        auto clock4 = new Clock(mGame);
        // Clock* clock5 = new Clock(mGame);
        // Clock* clock6 = new Clock(mGame);

        clock1->SetPosition(Vector2(100.0f, 100.0f));
        clock2->SetPosition(Vector2(200.0f, 100.0f));
        clock3->SetPosition(Vector2(300.0f, 100.0f));
        clock4->SetPosition(Vector2(400.0f, 100.0f));
        // clock5->SetPosition(Vector2(500.0f, 100.0f));
        // clock6->SetPosition(Vector2(600.0f, 100.0f));

        clock1->SetScale(0.2);
        clock2->SetScale(0.2);
        clock3->SetScale(0.2);
        clock4->SetScale(0.2);
        
        // ルーレットクラスの生成
        mRouletteBlock = new Actor(mGame);
        mRouletteBlock->SetPosition(Vector2(mGame->ScreenWidth - 200.0f, 150.0f));
        mRouletteBlock->SetScale(0.3);
        auto* rouletteblocksprite = new SpriteComponent(mRouletteBlock, 200);
        rouletteblocksprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "roulette_block_dot.png"));

        mRoulette = new Actor(mGame);
        mRoulette->SetPosition(Vector2(mGame->ScreenWidth - 200.0f, 150.0f));
        mRoulette->SetScale(1.0);
        auto* sprite = new SpriteComponent(mRoulette, 200);
        sprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "kakinotane_dot.png"));
        mGame->SetRoulette(0);
        
        // エネミーをランダム作成
        for (int i = 0; i < 200; i++)
        {

            auto* enemy = new Enemy(mGame);
            // enemy->SetPosition(Vector2(Math::GetRand(100.0f, mGame->ScreenWidth - 100.0f), -100.0f)); // 敵の初期位置設定（縦）
            enemy->SetPosition(Vector2(mGame->ScreenWidth + 100.0f, Math::GetRand(100.0f, mGame->ScreenHeight - 100.0f))); // 敵の初期位置設定（横）
            enemy->SetEnemySpeed(Math::GetRand(300.0f, 500.0f), Math::GetRand(300.0f, 500.0f));
            enemy->SetScale(Math::GetRand(0.5f, 1.5f));
            enemy->SetEnemyMoveType(Enemy::TRACKING);

            
            // 追尾タイプ
            if (i % 2 == 1)
            {
                enemy->SetEnemyMoveType(Enemy::TRACKING);
            }

            /*
            else if (i % 4 == 3) {
                enemy->SetEnemyMoveType(Enemy::BOUND);
            }
            */

            // 数匹ずつ動かす
            enemy->SetWaitTime(i / 3 * Math::GetRand(80.0f, 100.0f) * 0.35);

            // 音楽の再生（ループ）
            Mix_PlayMusic(mGame->GetBGMs()["game"], -1);


            // 初期化フラグをfalseへ
            mGame->SetInitFlag(false);

            mRouletteFlag = true;
        }
        

        // 開始メッセージの作成
        mStartMsgFlag = true;
        mStartMsg = new Actor(mGame);
        mStartMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
        auto* startMsgSprite = new SpriteComponent(mStartMsg, 200);
        std::vector<int> rgb = { 255, 255, 255 };
        int size = 150;
        startMsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Go!!", rgb, size));
    
    }
    
}

void GameScene::Update(float deltaTime)
{
    
    // エネミーを全部撃破したらlevel up
    if (mGame->GetEnemies().empty())
    {
        if (mRouletteFlag) {
            // レベルアップ
            mGame->SetGameLevel(mGame->GetGameLevel() + 1);
            // ルーレットフラグをfalse
            mRouletteFlag = false;
            mRoulette->SetState(Actor::EDead);
            mRouletteBlock->SetState(Actor::EDead);

            // ルーレットシーンへ
            mGame->SetNextScene(new RouletteScene(mGame));
        }
        else {
            // ルーレットクラスの生成
            if (mRoulette == nullptr) {

                mRouletteBlock = new Actor(mGame);
                mRouletteBlock->SetPosition(Vector2(mGame->ScreenWidth - 200.0f, 150.0f));
                mRouletteBlock->SetScale(0.3);
                auto* rouletteblocksprite = new SpriteComponent(mRouletteBlock, 200);
                rouletteblocksprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "roulette_block_dot.png"));

                mRoulette = new Actor(mGame);
                mRoulette->SetPosition(Vector2(mGame->ScreenWidth - 200.0f, 150.0f));
                mRoulette->SetScale(1);
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
            }
            

            // 時間経過させる処理（4秒経過）
            int time = SDL_GetTicks64() - mGame->GetReturnToTheRouletteTime();
            if (time > 4000 ) {
                // ルーレットフラグをfalse
                mRouletteFlag = true;

                // エネミーをランダム作成
                for (int i = 0; i < 200; i++)
                {
                    auto* enemy = new Enemy(mGame);
                    // enemy->SetPosition(Vector2(Math::GetRand(100.0f, mGame->ScreenWidth - 100.0f), -100.0f)); // 敵の初期位置設定（縦）
                    enemy->SetPosition(Vector2(mGame->ScreenWidth + 100.0f, Math::GetRand(100.0f, mGame->ScreenHeight - 100.0f))); // 敵の初期位置設定（横）
                    enemy->SetEnemySpeed(Math::GetRand(300.0f + mGame->GetGameLevel() * 50, 500.0f + mGame->GetGameLevel() * 50), Math::GetRand(300.0f + mGame->GetGameLevel() * 50, 500.0f + mGame->GetGameLevel() * 50));
                    enemy->SetScale(Math::GetRand(0.5f, 1.5f));

                    // 数匹ごとにタイプ変化
                    if (i % 2 == 0)
                    {
                        enemy->SetEnemyMoveType(Enemy::TRACKING);
                        // enemy->SetEnemyShakeWidth(Math::GetRand(5.0f, 15.0f));
                    }

                    // 数匹ずつ動かす
                    enemy->SetWaitTime(i / 3 * Math::GetRand(80.0f, 100.0f) * 0.35);

                    // 初期化フラグをfalseへ
                    mGame->SetInitFlag(false);
                }
            }

        }
        
    }
    

    // 時間の更新
    int end = SDL_GetTicks64();
    
    // ゲーム開始からの経過時間の取得
    int diff_time = end - ( mGame->GetTimer()->GetStartTime() );
    mGame->GetTimer()->SetDiffTime(diff_time - mGame->GetRouletteTime());
    // std::cout << diff_time << std::endl;

    // 開始メッセージのアップデート
    if (mStartMsgFlag) {
        mStartMsg->SetScale(mStartMsg->GetScale() + diff_time * 0.002);
        if (diff_time >= 700) {
            mStartMsg->SetState(Actor::EDead);
            mStartMsgFlag = false;
        }
    }
    

    
    
}

void GameScene::ProcessInput(const Uint8* state)
{
    // プレイヤーのキー入力操作
    mGame->GetPlayer()->ProcessKeyboard(state);

    // スペースかエンター押下でポーズシーンに遷移
    if (state[SDL_SCANCODE_P])
    {
        mGame->SetNextScene(new PauseScene(mGame));

        for (auto enemy : mGame->GetEnemies())
        {
            enemy->SetState(Actor::EStop);
        }

    }
}

std::string const GameScene::GetSceneName()
{
    return "GAME";
}
