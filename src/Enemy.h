#pragma once
#include "Actor.h"
#include "Vector2.h"

// �G�l�~�[�N���X
class Enemy : public Actor {
public:
    Enemy(class Game* game);
    ~Enemy();
    void UpdateActor(float deltaTime) override;
    void SetPosition(const Vector2& pos);

    // �ړ��^�C�v
    enum MoveType
    {
        STRAIGHT, // �����ړ�
        SHAKE,    // �h��Ȃ���ړ�
        BOUND,    // �������˕Ԃ�
        TRACKING  // �ǔ�
    };

private:
    MoveType mEnemyMoveType; // �ړ��^�C�v
    std::vector<float> mEnemySpeed = { 150.0f, 150.0f }; // �ړ����x
    float mEnemyShakeHeight;  // �h��镝�i�c�j
    float mEnemyShakeWidth;  // �h��镝�i���j
    Vector2* mInitPosition;  // �ŏ��̈ʒu
    float mTimeCount; // �o�ߎ���
    float mWaitTime;  // �ҋ@����
    class ColliderComponent* mCollider;  // �R���C�_
    int bound_time; // ���˕Ԃ��
    bool boundflag;
    int bounddirection; // �o�E���h�^�̓G�������㉺�����̃p�����[�^

public:
    // Getter, Setter
    void SetEnemyMoveType(const MoveType moveType) { mEnemyMoveType = moveType; }
    void SetEnemySpeed(const float x, const float y) { mEnemySpeed = { x, y }; }
    void SetEnemyShakeHeight(const float height) { mEnemyShakeHeight = height; }
    void SetEnemyShakeWidth(const float width) { mEnemyShakeWidth = width; }
    void SetWaitTime(const float time) { mWaitTime = time; }
    class ColliderComponent* GetCollider() const { return mCollider; }
};
