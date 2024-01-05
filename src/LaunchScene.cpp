#include "LaunchScene.h"
#include "MenuScene.h"
#include "Scene.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include <iostream>


LaunchScene::LaunchScene(class Game* game)
    :Scene(game)
{
}

LaunchScene::~LaunchScene()
{
}

void LaunchScene::Start()
{
    std::cout << GetSceneName() << std::endl;
    mGame->SetSceneName("Launch");

    mLaunch = new Actor(mGame);
    mLaunch->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight / 2));
    auto* launchImageSprite = new SpriteComponent(mLaunch, 200);
    launchImageSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "start_dot.png"));

    /*
    mFrame = new Actor(mGame);
    mFrame->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* frameImageSprite = new SpriteComponent(mFrame, 200);
    frameImageSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "frame.png"));
    */

    mTitle = new Actor(mGame);
    mTitle->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.3));
    auto* titleImageSprite = new SpriteComponent(mTitle, 200);
    titleImageSprite->SetTexture(mGame->LoadTexture(mGame->GetAssetsPath() + "title_dot.png"));

    mCharacter = new Actor(mGame);
    mCharacter->SetPosition(Vector2(mGame->ScreenWidth / 2, mGame->ScreenHeight * 0.8));
    auto* PressSpaceImageSprite = new SpriteComponent(mCharacter, 200);
    std::vector<int> rgb = { 0, 0, 255 };
    int size = 50;
    PressSpaceImageSprite->SetTexture(mGame->LoadFont(mGame->GetFontAssetsPath() + "mg01_bmp8.ttf", "Press Space", rgb, size));
    
    // 音楽の再生（ループ）
    Mix_PlayMusic(mGame->GetBGMs()["start"], -1);
}

void LaunchScene::Update(float deltaTime)
{

}

void LaunchScene::ProcessInput(const Uint8* state)
{
    // スペースかエンター押下でメニューシーンに遷移
    if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN])
    {
        mGame->SetNextScene(new MenuScene(mGame));
        mLaunch->SetState(Actor::EDead);
        mCharacter->SetState(Actor::EDead);
        mTitle->SetState(Actor::EDead);
        // Mix_HaltMusic();

        // SEの再生
        Mix_PlayChannel(-1, mGame->GetSoundEffects()["start"], 0);

        SDL_Delay(1800);
    }
}


std::string const LaunchScene::GetSceneName()
{
    return "START";
}