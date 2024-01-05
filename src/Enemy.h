#pragma once
#include "Actor.h"
#include "Vector2.h"

// エネミークラス
class Enemy : public Actor {
public:
    Enemy(class Game* game);
    ~Enemy();
    void UpdateActor(float deltaTime) override;
    void SetPosition(const Vector2& pos);

    // 移動タイプ
    enum MoveType
    {
        STRAIGHT, // 直線移動
        SHAKE,    // 揺れながら移動
        BOUND,    // 高速跳ね返り
        TRACKING  // 追尾
    };

private:
    MoveType mEnemyMoveType; // 移動タイプ
    std::vector<float> mEnemySpeed = { 150.0f, 150.0f }; // 移動速度
    float mEnemyShakeHeight;  // 揺れる幅（縦）
    float mEnemyShakeWidth;  // 揺れる幅（横）
    Vector2* mInitPosition;  // 最初の位置
    float mTimeCount; // 経過時間
    float mWaitTime;  // 待機時間
    class ColliderComponent* mCollider;  // コライダ
    int bound_time; // 跳ね返り回数
    bool boundflag;
    int bounddirection; // バウンド型の敵が動く上下方向のパラメータ

public:
    // Getter, Setter
    void SetEnemyMoveType(const MoveType moveType) { mEnemyMoveType = moveType; }
    void SetEnemySpeed(const float x, const float y) { mEnemySpeed = { x, y }; }
    void SetEnemyShakeHeight(const float height) { mEnemyShakeHeight = height; }
    void SetEnemyShakeWidth(const float width) { mEnemyShakeWidth = width; }
    void SetWaitTime(const float time) { mWaitTime = time; }
    class ColliderComponent* GetCollider() const { return mCollider; }
};
