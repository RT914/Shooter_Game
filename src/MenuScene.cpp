#include "MenuScene.h"
#include "Scene.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ReadyScene.h"
#include <iostream>


MenuScene::MenuScene(class Game* game)
    :Scene(game)
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Menu");
    
    mBack = new Actor(mGame);
    mBack->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    auto* startMsgSprite = new SpriteComponent(mBack, 200);
    startMsgSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "menu_dot.png"));

    // ボタンの作成
    mButton = new Actor(mGame);
    mButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* buttonSprite = new SpriteComponent(mButton, 200);
    buttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

    mButtonMsg = new Actor(mGame);
    mButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* PressSpaceImageSprite = new SpriteComponent(mButtonMsg, 200);
    std::vector<int> rgb = { 255, 255, 255 };
    int size = 150;
    PressSpaceImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Stage 1", rgb, size));

    // ゲーム終了ボタンの作成
    mFinishButton = new Actor(mGame);
    mFinishButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
    auto* finishButtonSprite = new SpriteComponent(mFinishButton, 200);
    finishButtonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

    mFinishButtonMsg = new Actor(mGame);
    mFinishButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
    auto* finishButtonImageSprite = new SpriteComponent(mFinishButtonMsg, 200);
    finishButtonImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Quit", rgb, size));

    startTime = SDL_GetTicks64();
}

void MenuScene::Update(float deltaTime)
{
}

void MenuScene::ProcessInput(const Uint8* state)
{
    int scale = mButton->GetScale();
    Vector2 bp = mButton->GetPosition();

    int weight, height;
    SDL_QueryTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button.png"), NULL, NULL, &weight, &height);
    
    // マウスの左クリックでReadySceneに遷移
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if ((bp.x - weight / 2 <= mouseX && mouseX <= bp.x + weight / 2) && (bp.y - height / 2 <= mouseY && mouseY <= bp.y + height / 2)) {
            mGame->SetNextScene(new ReadyScene(mGame));
            mBack->SetState(Actor::EDead);
            mButton->SetState(Actor::EDead);
            mButtonMsg->SetState(Actor::EDead);
            mFinishButton->SetState(Actor::EDead);
            mFinishButtonMsg->SetState(Actor::EDead);
            Mix_HaltMusic();

            // SEの再生
            Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
            SDL_Delay(400);
        }
    }

    int difftime = SDL_GetTicks64() - startTime;

    if (difftime > 400) {

        if (state[SDL_SCANCODE_W])
        {
            NextButtonNum = 0;
        }
        else if (state[SDL_SCANCODE_S]) {
            NextButtonNum = 1;
        }
        else if (state[SDL_SCANCODE_SPACE]) {
            if (NowButtonNum == 0) {
                mGame->SetNextScene(new ReadyScene(mGame));
                mBack->SetState(Actor::EDead);
                mButton->SetState(Actor::EDead);
                mButtonMsg->SetState(Actor::EDead);
                mFinishButton->SetState(Actor::EDead);
                mFinishButtonMsg->SetState(Actor::EDead);
                Mix_HaltMusic();

                // SEの再生
                Mix_PlayChannel(-1, mGame->GetSoundEffects()["button"], 0);
                SDL_Delay(400);
            }
            else if (NowButtonNum == 1) {
                if (state[SDL_SCANCODE_SPACE]) {
                    mGame->SetlsRunning(false);
                }
            }
        }

    }
    
    if (NextButtonNum == 0 && NowButtonNum != 0) {

        mButton->SetState(Actor::EDead);
        mButtonMsg->SetState(Actor::EDead);

        mButton = new Actor(mGame);
        mButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* buttonSprite = new SpriteComponent(mButton, 200);
        buttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

        mButtonMsg = new Actor(mGame);
        mButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* PressSpaceImageSprite = new SpriteComponent(mButtonMsg, 200);
        std::vector<int> rgb = { 255, 255, 255 };
        int size = 150;
        PressSpaceImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Stage 1", rgb, size));


        mFinishButton->SetState(Actor::EDead);
        mFinishButtonMsg->SetState(Actor::EDead);

        mFinishButton = new Actor(mGame);
        mFinishButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* finishButtonSprite = new SpriteComponent(mFinishButton, 200);
        finishButtonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

        mFinishButtonMsg = new Actor(mGame);
        mFinishButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* finishButtonImageSprite = new SpriteComponent(mFinishButtonMsg, 200);
        finishButtonImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Quit", rgb, size));

        NowButtonNum = 0;
    }else if (NextButtonNum == 1 && NowButtonNum != 1) 
    {

        mButton->SetState(Actor::EDead);
        mButtonMsg->SetState(Actor::EDead);

        mButton = new Actor(mGame);
        mButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* buttonSprite = new SpriteComponent(mButton, 200);
        buttonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot.png"));

        mButtonMsg = new Actor(mGame);
        mButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
        auto* PressSpaceImageSprite = new SpriteComponent(mButtonMsg, 200);
        std::vector<int> rgb = { 255, 255, 255 };
        int size = 150;
        PressSpaceImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Stage 1", rgb, size));


        mFinishButton->SetState(Actor::EDead);
        mFinishButtonMsg->SetState(Actor::EDead);

        mFinishButton = new Actor(mGame);
        mFinishButton->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* finishButtonSprite = new SpriteComponent(mFinishButton, 200);
        finishButtonSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "button_dot_selected.png"));

        mFinishButtonMsg = new Actor(mGame);
        mFinishButtonMsg->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
        auto* finishButtonImageSprite = new SpriteComponent(mFinishButtonMsg, 200);
        finishButtonImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Quit", rgb, size));

        NowButtonNum = 1;
    }

    

}


std::string const MenuScene::GetSceneName()
{
    return "MENU";
}