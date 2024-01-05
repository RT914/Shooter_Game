#include "ExplosionEffect.h"
#include "Game.h"
#include "SpriteComponent.h"

ExplosionEffect::ExplosionEffect(class Game* game, int num)
    :Actor(game)
    , mTimeCount(0.0f)
{
    // �X�v���C�g�ݒ�
    auto* sprite = new SpriteComponent(this);
    if (num == 1) { // �v���C���[
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost_back.png"));
    }
    else if (num == 2) { // �~�T�C��
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost.png"));
    }
    else {
        sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ghost.png"));
    }
    
    // �ŏ��͑傫��1�ɂ���
    SetScale(0.2f);
}

void ExplosionEffect::UpdateActor(float deltaTime)
{
    // �e�̃��\�b�h�Ăяo��
    Actor::UpdateActor(deltaTime);
    Vector2 pos = GetPosition();

    if (GetGame()->GetPauseFlag()) {
        // ���X�ɏ㏸������
        pos.y -= mTimeCount / DisplayTime * 7.0f;
    }
    
    if (pos.y < - 100)
    {
        SetState(EDead);
    }

    mTimeCount += deltaTime;
    SetPosition(pos);
}