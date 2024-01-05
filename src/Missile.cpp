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
    // �X�v���C�g�ǉ�
    auto* sprite = new SpriteComponent(this, 90);
    if (num == 1) { //�`�̎�~�T�C��
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "kakinotane_dot.png")); // �~�T�C���̉摜���Z�b�g
    }
    else if (num == 2 || num == 3) { // �n�C�`���E�~�T�C��
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "haityu_dot.png")); // �~�T�C���̉摜���Z�b�g
    }
    else if (num == 4) {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "pyuregumi_dot.png")); // �~�T�C���̉摜���Z�b�g
    }
    else {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "kakinotane_dot.png")); // �~�T�C���̉摜���Z�b�g
    }

    
    
    // �R���C�_�ǉ�
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(50.0f * GetScale());
    SetBoomerangFlag(false);
}

// �A�N�^�X�V
void Missile::UpdateActor(float deltaTime)
{
    // �e�̃��\�b�h�Ăяo��
    Actor::UpdateActor(deltaTime);

    int missilenum = GetMissileNum();

    if (GetGame()->GetPauseFlag()) {
        // �~�T�C�����ړ�������
        Vector2 pos = GetPosition();
        if (missilenum == 1) { // �ʏ�̃~�T�C��
            pos.x += mMissileSpeed[0] * deltaTime;

            // ��ʍ��E���Ŕj��
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 2) { // ���e�~�T�C���̏����
            // ��ʏ㉺���Œ��e
            if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight)
            {
                SetMissileSpeed(mMissileSpeed[0], -mMissileSpeed[1]);
            }
            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime * (-1);

            // ��ʍ��E���Ŕj��
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 3) { // ���e�~�T�C���̏����
            // ��ʏ㉺���Œ��e
            if (pos.y < 0.0f || pos.y > GetGame()->ScreenHeight)
            {
                SetMissileSpeed(mMissileSpeed[0], -mMissileSpeed[1]);
            }

            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime;

            // ��ʍ��E���Ŕj��
            if (pos.x < 0.0f || pos.x > GetGame()->ScreenWidth)
            {
                SetState(EDead);
            }

        }
        else if (missilenum == 4) { // �u�[�������~�T�C��
            // Player�֖߂��Ă���
            double playerDirectionX = GetGame()->GetPlayer()->GetPosition().x - pos.x;
            double playerDirectionY = GetGame()->GetPlayer()->GetPosition().y - pos.y;
            double length = sqrt(playerDirectionX * playerDirectionX + playerDirectionY * playerDirectionY);

            // ���K��
            playerDirectionX /= length;
            playerDirectionY /= length;

            SetMissileSpeed(mMissileSpeed[0] + playerDirectionX * 80, mMissileSpeed[1] + playerDirectionY * 80);

            pos.x += 0.5 * mMissileSpeed[0] * deltaTime;
            pos.y += 0.5 * mMissileSpeed[1] * deltaTime;

            // �v���C���[�Ƃ�1��ڂ̏Փ˔���i�ˏo���j
            if (Intersect(*mCollider, *(GetGame()->GetPlayer()->GetCollider())) && GetBoomerangFlag())
            {
                SetState(EDead);
            }
            else if (!Intersect(*mCollider, *(GetGame()->GetPlayer()->GetCollider()))) {
                SetBoomerangFlag(true);
            }

            // ���[���b�g��ʂł͍폜
            if (GetGame()->GetSceneName() == "Roulette" || GetGame()->GetSceneName() == "GameOver") {
                SetState(EDead);
            }
        }

        // �ʒu�X�V
        SetPosition(pos);
    }

    // �G�l�~�[�Ƃ̏Փ˔���
    for (auto enemy : GetGame()->GetEnemies())
    {
        if (Intersect(*mCollider, *(enemy->GetCollider())))
        {
            SetState(EDead);
            enemy->SetState(EDead);
            // �G�l�~�[�̈ʒu�Ŕ���������
            // enemy->GetScale();
            auto* explosioneffect = new ExplosionEffect(GetGame(), 2);
            explosioneffect->SetPosition(Vector2(enemy->GetPosition()));
            break;
        }
    }
    
}