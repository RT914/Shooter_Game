#include "ExplosionEffect.h"
#include "Game.h"
#include "SpriteComponent.h"

ExplosionEffect::ExplosionEffect(class Game* game, int num)
    :Actor(game)
    , mTimeCount(0.0f)
{
    // スプライト設定
    auto* sprite = new SpriteComponent(this);
    if (num == 1) { // プレイヤー
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost_back.png"));
    }
    else if (num == 2) { // ミサイル
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost.png"));
    }
    else {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost.png"));
    }
    
    // 最初は大きさ1にする
    SetScale(0.2f);
}

void ExplosionEffect::UpdateActor(float deltaTime)
{
    // 親のメソッド呼び出し
    Actor::UpdateActor(deltaTime);
    Vector2 pos = GetPosition();

    if (GetGame()->GetPauseFlag()) {
        // 徐々に上昇させる
        pos.y -= mTimeCount / DisplayTime * 7.0f;
    }
    
    if (pos.y < - 100)
    {
        SetState(EDead);
    }

    mTimeCount += deltaTime;
    SetPosition(pos);
}