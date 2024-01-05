#pragma once
#include "Actor.h"
#include <vector>

// �~�T�C���N���X
class Missile : public Actor {
public:
    Missile(class Game* game, int num);
    void UpdateActor(float deltaTime) override; // �A�N�^�X�V
    void SetMissileSpeed(float x, float y) { mMissileSpeed = { x, y }; }

private:
    int MissileNum; // �~�T�C���̎��

    int GetMissileNum() { return MissileNum; }

    std::vector<float> mMissileSpeed = { 1000.0f, 1000.0f }; // �~�T�C���ړ����x
    class ColliderComponent* mCollider;  // �R���C�_
    bool missile_boomerang_flag;

    void SetBoomerangFlag(bool tf) { missile_boomerang_flag = tf; }
    bool GetBoomerangFlag() { return missile_boomerang_flag; }
};

