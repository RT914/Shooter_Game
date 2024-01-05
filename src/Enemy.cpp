#include "Enemy.h"
#include <cmath>
#include <iostream>
#include "Game.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "Math.h"

Enemy::Enemy(Game* game)
    :Actor(game)
    , mEnemyMoveType(STRAIGHT)
    , mTimeCount(0.0f)
    , mEnemyShakeHeight(5.0f)
    , mEnemyShakeWidth(5.0f)
    , mWaitTime(0.0f)
    , bound_time(0)
    , boundflag(false)
    , bounddirection(1)
{
    // �X�v���C�g�ݒ�
    auto* sprite = new SpriteComponent(this);
    int num = Math::GetRand(1, 5);
    sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "virus_" + std::to_string(num) + ".png"));
    // �R���C�_�ǉ�
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(50.0f * GetScale());
    // �G�l�~�[�ǉ�
    game->AddEnemy(this);

    // ��������
    num = Math::GetRand(0, 2);
    if (num == 0) {
        bounddirection = -1;
    }
    
}

Enemy::~Enemy()
{
    // �G�l�~�[�폜
    GetGame()->RemoveEnemy(this);
}

void Enemy::UpdateActor(float deltaTime)
{
    // �e�̃��\�b�h�Ăяo��
    Actor::UpdateActor(deltaTime);

    // �X�g�b�v��Ԃ̎��͍X�V�����͂��Ȃ�
    if (GetState() != EStop) {


        // �ҋ@���ԕ��͑҂�
        mTimeCount++;
        if (mTimeCount < mWaitTime)
        {
            return;
        }

        // �ړ�����
        Vector2 pos = GetPosition();
        float p = GetGame()->GetPlayer()->GetPosition().y - pos.y;

        switch (mEnemyMoveType)
        {
        case STRAIGHT:
            pos.x -= mEnemySpeed[0] * deltaTime;

            // ��ʊO�ɏo����폜
            if (pos.x < 0)
            {
                SetState(EDead);
            }

            break;

        case SHAKE:
            pos.x -= mEnemySpeed[0] * deltaTime;
            pos.y = mInitPosition->y + (sinf(mTimeCount / 10.0f) * mEnemyShakeHeight);

            // ��ʊO�ɏo����폜
            if (pos.x < 0)
            {
                SetState(EDead);
            }

            break;

        case BOUND:
            pos.x -= mEnemySpeed[0] * deltaTime * 0.5;
            pos.y -= mEnemySpeed[1] * deltaTime * 1.2;

            // �ǂ�10��ڐG�ō폜
            if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight || pos.x < 0.0f || pos.x > GetGame()->ScreenWidth) {
                
                if (boundflag) {

                    if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight) {
                        SetEnemySpeed(mEnemySpeed[0], (-1) * mEnemySpeed[1]);
                    }
                    else {
                        SetEnemySpeed((-1) * mEnemySpeed[0], mEnemySpeed[1]);
                    }

                    if (bound_time >= 10) {
                        SetState(EDead);
                    }

                    bound_time++;

                }
                
                
            }
            else if (pos.y >= 0.0f && pos.y <= GetGame()->ScreenHeight && pos.x >= 0.0f && pos.x <= GetGame()->ScreenWidth) {
                
                if (!boundflag) {
                    boundflag = true;
                }
                
            }

            break;

        case TRACKING:
            pos.x -= mEnemySpeed[0] * deltaTime;
            

            if (p > 0) {
                pos.y -= (-1) * mEnemySpeed[1] * deltaTime * 0.2;
            }
            else {
                pos.y -= mEnemySpeed[1] * deltaTime * 0.2;
            }

            // ��ʊO�ɏo����폜
            if (pos.x < 0)
            {
                SetState(EDead);
            }

            break;

        default:
            break;
        }

        SetPosition(pos);

    }

    
}

void Enemy::SetPosition(const Vector2& pos)
{
    // �ʒu��ێ�����
    mInitPosition = new Vector2(pos.x, pos.y);
    Actor::SetPosition(pos);
}
