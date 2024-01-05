#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"

/*
struct Player {
	Player(Eigen::Vector2d& pos) : position(pos) {};

	Eigen::Vector2d position;
	int size = 4;
};
*/

// 追加分
class Player : public Actor {
public:
    Player(class Game* game);
    void UpdateActor(float deltaTime) override; // アクタ更新
    void ProcessKeyboard(const uint8_t* state); // キーボード入力

private:
    float mRightMove; // 右方向への移動量
    float mDownMove;  // 下方向への移動量
    const float PlayerSpeed = 480.0f; // 移動速度
    float mTexWidth; // テクスチャの幅
    float mTexHeight; //テクスチャの高さ

    bool mIsCanShot;      // ミサイルを撃てるかどうか？
    float mDeltaShotTime; // ミサイルを撃ってからの時間
    const float CanShotTime = 0.15f; // ミサイルを撃てるようになるまでの間隔

    class ColliderComponent* mCollider;  // コライダ
public:
    class ColliderComponent* GetCollider() const { return mCollider; }
};

#endif
