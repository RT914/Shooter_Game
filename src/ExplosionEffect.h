#pragma once
#include "Actor.h"

// 爆発エフェクト
class ExplosionEffect : public Actor {
public:
    ExplosionEffect(class Game* game, int num);
    void UpdateActor(float deltaTime) override;

private:
    float mTimeCount; // 経過時間
    const float DisplayTime = 1.0f; // 表示時間
};

