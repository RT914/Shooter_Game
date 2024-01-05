#include "Missile.h"
#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "ExplosionEffect.h"
#include <iostream>

Missile::Missile(class Game* game, int num)
    :Actor(game)
    , MissileNum(num)
{
    // スプライト追加
    auto* sprite = new SpriteComponent(this, 90);
    if (num == 1) { //柿の種ミサイル
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "kakinotane_dot.png")); // ミサイルの画像をセット
    }
    else if (num == 2 || num == 3) { // ハイチュウミサイル
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "haityu_dot.png")); // ミサイルの画像をセット
    }
    else if (num == 4) {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "pyuregumi_dot.png")); // ミサイルの画像をセット
    }
    else {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "kakinotane_dot.png")); // ミサイルの画像をセット
    }

    
    
    // コライダ追加
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(50.0f * GetScale());
    SetBoomerangFlag(false);
}

// アクタ更新
void Missile::UpdateActor(float deltaTime)
{
    // 親のメソッド呼び出し
    Actor::UpdateActor(deltaTime);

    int missilenum = GetMissileNum();

    if (GetGame()->GetPauseFlag()) {
        // ミサイルを移動させる
        Vector2 pos = GetPosition();
        if (missilenum == 1) { // 通常のミサイル
            pos.x += mMissileSpeed[0] * deltaTime;

            // 画面左右部で破棄
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 2) { // 跳弾ミサイルの上方向
            // 画面上下部で跳弾
            if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight)
            {
                SetMissileSpeed(mMissileSpeed[0], -mMissileSpeed[1]);
            }
            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime * (-1);

            // 画面左右部で破棄
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 3) { // 跳弾ミサイルの上方向
            // 画面上下部で跳弾
            if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight)
            {
                SetMissileSpeed(mMissileSpeed[0], -mMissileSpeed[1]);
            }

            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime;

            // 画面左右部で破棄
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 4) { // ブーメランミサイル
            // Playerへ戻ってくる
            double playerDirectionX = GetGame()->GetPlayer()->GetPosition().x - pos.x;
            double playerDirectionY = GetGame()->GetPlayer()->GetPosition().y - pos.y;
            double length = sqrt(playerDirectionX * playerDirectionX + playerDirectionY * playerDirectionY);

            // 正規化
            playerDirectionX /= length;
            playerDirectionY /= length;

            SetMissileSpeed(mMissileSpeed[0] + playerDirectionX * 80, mMissileSpeed[1] + playerDirectionY * 80);

            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime;

            // プレイヤーとの1回目の衝突判定（射出時）
            if (Intersect(*mCollider, *(GetGame()->GetPlayer()->GetCollider())) && GetBoomerangFlag())
            {
                SetState(EDead);
            }
            else if (!Intersect(*mCollider, *(GetGame()->GetPlayer()->GetCollider()))) {
                SetBoomerangFlag(true);
            }

            // ルーレット画面では削除
            if (GetGame()->GetSceneName() == "Roulette" || GetGame()->GetSceneName() == "GameOver") {
                SetState(EDead);
            }
        }

        // 位置更新
        SetPosition(pos);
    }

    // エネミーとの衝突判定
    for (auto enemy : GetGame()->GetEnemies())
    {
        if (Intersect(*mCollider, *(enemy->GetCollider())))
        {
            SetState(EDead);
            enemy->SetState(EDead);
            // エネミーの位置で爆発させる
            // enemy->GetScale();
            auto* explosioneffect = new ExplosionEffect(GetGame(), 2);
            explosioneffect->SetPosition(Vector2(enemy->GetPosition()));
            break;
        }
    }
    
}