#include <Eigen/Dense>
#include <stdio.h>
#include <iostream>
#include "Player.h"
#include "Game.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "Component.h"
#include "ColliderComponent.h"
#include "Missile.h"
#include "ExplosionEffect.h"
#include "GameOverScene.h"
#include "Enemy.h"


Player::Player(Game* game)
    :Actor(game)
    , mRightMove(0.0f)
    , mDownMove(0.0f)
    , mIsCanShot(false)
    , mDeltaShotTime(0.0f)
    , mTexWidth(0.0f)
    , mTexHeight(0.0f)
{
    
    // �X�v���C�g�ݒ�
    auto* sprite = new SpriteComponent(this);
    sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "player_dot.png"));
    mTexWidth = sprite->GetTexWidth();
    mTexHeight = sprite->GetTexHeight();
    // �R���C�_�ǉ�
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(70.0f * GetScale());

}

// �A�N�^�X�V
void Player::UpdateActor(float deltaTime)
{
    // �e�̃��\�b�h�Ăяo��
    Actor::UpdateActor(deltaTime);
    // �Q�[���I�����Ă����瓮�����Ȃ�
    if (GetGame()->GetPauseFlag() && GetGame()->GetRouletteFlag() && GetGame()->GetScene()->GetSceneName().compare("GAMEOVER") != 0)
    {
        // �v���C���[���ړ�������
        float texwidth = mTexWidth;
        float texheight = mTexHeight;
        Vector2 pos = GetPosition();

        pos.x += mRightMove * deltaTime;
        pos.y += mDownMove * deltaTime;
        if (pos.x < texwidth / 2)
        {
            pos.x = texwidth / 2;
        }
        else if (pos.x > Game::ScreenWidth - texwidth / 2)
        {
            pos.x = Game::ScreenWidth - texwidth / 2;
        }
        if (pos.y < texheight / 2)
        {
            pos.y = texheight / 2;
        }
        else if (pos.y > Game::ScreenHeight - texheight / 2)
        {
            pos.y = Game::ScreenHeight - texheight / 2;
        }
        SetPosition(pos);
    }


    
    // �G�l�~�[�ƏՓ˂����玀�S
    for (auto enemy : GetGame()->GetEnemies())
    {
        if (Intersect(*mCollider, *(enemy->GetCollider())))
        {
            // �Q�[���I��
            GetGame()->SetNextScene(new GameOverScene(GetGame()));
            SetState(EDead);
            // �F���D�̈ʒu�Ŕ����G�t�F�N�g
            auto* explosion = new ExplosionEffect(GetGame(), 1);
            explosion->SetPosition(Vector2(GetPosition()));

            // SE�̍Đ�
            Mix_PlayChannel(-1, GetGame()->GetSoundEffects()["falling"], 0);
            SDL_Delay(400);
        }
    }
    
    

    // �~�T�C�������Ԋu���J����
    if (!mIsCanShot)
    {
        mDeltaShotTime += deltaTime;
        if (mDeltaShotTime > CanShotTime)
        {
            mIsCanShot = true;
            mDeltaShotTime = 0.0f;
        }
    }
    
}

// �L�[�{�[�h����
void Player::ProcessKeyboard(const uint8_t* state)
{
    mRightMove = 0.0f;
    mDownMove = 0.0f;
    // �L�[���͂ŏ㉺���E�Ɉړ�������
    if (state[SDL_SCANCODE_D])
    {
        mRightMove += PlayerSpeed;
    }
    if (state[SDL_SCANCODE_A])
    {
        mRightMove -= PlayerSpeed;
    }
    if (state[SDL_SCANCODE_S])
    {
        mDownMove += PlayerSpeed;
    }
    if (state[SDL_SCANCODE_W])
    {
        mDownMove -= PlayerSpeed;
    }

    if (state[SDL_SCANCODE_UP]) {
        mDownMove -= PlayerSpeed;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        mRightMove -= PlayerSpeed;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        mDownMove += PlayerSpeed;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        mRightMove += PlayerSpeed;
    }

    if (GetGame()->GetRoulette() == 0) {
        // �`�̎�~�T�C��������
        if (state[SDL_SCANCODE_K])
        {
            if (mIsCanShot)
            {
                // ���Ԋu���J���邽�߃t���O��ύX
                mIsCanShot = false;
                mDeltaShotTime = 0.0f;
                // �~�T�C������
                auto* missile = new Missile(GetGame(), 1);
                Vector2 pos = GetPosition();
                missile->SetPosition(Vector2(pos.x, pos.y - 30.0f));
            }
        }
    }
    else if(GetGame()->GetRoulette() == 1){
        // �n�C�`���E�~�T�C��������
        if (state[SDL_SCANCODE_I])
        {
            if (mIsCanShot)
            {
                // ���Ԋu���J���邽�߃t���O��ύX
                mIsCanShot = false;
                mDeltaShotTime = 0.0f;
                // �~�T�C������
                auto* missile = new Missile(GetGame(), 2);
                Vector2 pos = GetPosition();
                missile->SetPosition(Vector2(pos.x, pos.y - 30.0f));
            }
        }
        else if (state[SDL_SCANCODE_M]) {

            if (mIsCanShot)
            {
                // ���Ԋu���J���邽�߃t���O��ύX
                mIsCanShot = false;
                mDeltaShotTime = 0.0f;
                // �~�T�C������
                auto* missile = new Missile(GetGame(), 3);
                Vector2 pos = GetPosition();
                missile->SetPosition(Vector2(pos.x, pos.y - 30.0f));
            }
        }
    }else if (GetGame()->GetRoulette() == 2) {
        // �s�����O�~�u�[�������~�T�C��������
        if (state[SDL_SCANCODE_K])
        {
            if (mIsCanShot)
            {
                // ���Ԋu���J���邽�߃t���O��ύX
                mIsCanShot = false;
                mDeltaShotTime = 0.0f;
                // �~�T�C������
                auto* missile = new Missile(GetGame(), 4);
                Vector2 pos = GetPosition();
                missile->SetPosition(Vector2(pos.x, pos.y - 30.0f));
            }
        }       
    }
    
  

}