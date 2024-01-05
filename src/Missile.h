#pragma once
#include "Actor.h"
#include <vector>

// ミサイルクラス
class Missile : public Actor {
public:
    Missile(class Game* game, int num);
    void UpdateActor(float deltaTime) override; // アクタ更新
    void SetMissileSpeed(float x, float y) { mMissileSpeed = { x, y }; }

private:
    int MissileNum; // ミサイルの種類

    int GetMissileNum() { return MissileNum; }

    std::vector<float> mMissileSpeed = { 1000.0f, 1000.0f }; // ミサイル移動速度
    class ColliderComponent* mCollider;  // コライダ
    bool missile_boomerang_flag;

    void SetBoomerangFlag(bool tf) { missile_boomerang_flag = tf; }
    bool GetBoomerangFlag() { return missile_boomerang_flag; }
};

