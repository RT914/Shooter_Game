#include "ReadyScene.h"
#include "Scene.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "Player.h"
#include <iostream>

ReadyScene::ReadyScene(class Game* game)
    :Scene(game)
{
}

ReadyScene::~ReadyScene()
{
}

void ReadyScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Ready");

    // �N���A�t���O��������
    mGame->SetGameClear(false);
    // �J�n���b�Z�[�W�̍쐬
    mReadyMsg = new Actor(mGame);
    mReadyMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    auto* startMsgSprite = new SpriteComponent(mReadyMsg, 200);
    std::vector<int> rgb = { 255, 255, 255 };
    int size = 150;
    startMsgSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Ready...?", rgb, size));
    
    // �v���[���[�̍쐬
    Player* player = new Player(mGame);
    player->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight - 200.0f));
    mGame->SetPlayer(player);

    // �|�[�Y�t���O��true��
    mGame->SetPauseFlag(true);

    // ���[���b�g�t���O��true��
    mGame->SetRouletteFlag(true);

    // �������t���O��true��
    mGame->SetInitFlag(true);

    mGame->SetGameLevel(0);

    startTime = SDL_GetTicks64();
}

void ReadyScene::Update(float deltaTime)
{
}

void ReadyScene::ProcessInput(const Uint8* state)
{
    int difftime = SDL_GetTicks64() - startTime;

    if (difftime > 400) {
        // �X�y�[�X���G���^�[�����ŃQ�[���V�[���ɑJ��
        if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN])
        {
            mGame->SetNextScene(new GameScene(mGame));
            mReadyMsg->SetState(Actor::EDead);
        }
    }
    
}

std::string const ReadyScene::GetSceneName()
{
    return "READY";
}