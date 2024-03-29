#include "ScrollSpriteComponent.h"
#include "Game.h"
#include "Actor.h"

ScrollSpriteComponent::ScrollSpriteComponent(class Actor* actor, int drawOrder)
    :SpriteComponent(actor, drawOrder)
    , mScrollSpeedX(0.0f)
    , mScrollSpeedY(0.0f)
    , mOffsetX(0.0f)
    , mOffsetY(0.0f)
{
}

void ScrollSpriteComponent::Update(float deltaTime)
{
    // 親のメソッド呼び出し
    SpriteComponent::Update(deltaTime);

    
    // 横方向
    // オフセット値を更新
    mOffsetX += mScrollSpeedX * deltaTime;
    if (mOffsetX > Game::ScreenWidth)
    {
        mOffsetX -= Game::ScreenWidth;
    }
    

    /*
    // 縦方向
    // オフセット値を更新
    mOffsetY += mScrollSpeedY * deltaTime;
    if (mOffsetY > Game::ScreenHeight)
    {
        mOffsetY -= Game::ScreenHeight;
    }
    */
}

void ScrollSpriteComponent::Draw(SDL_Renderer* renderer)
{
    // スクロールさせるため２枚並べて描画する
    // 通常のテクスチャ描画
    
    /*
    // 縦方向
    SDL_Rect r_bottom;
    r_bottom.w = static_cast<int>(mTexWidth * mActor->GetScale());
    r_bottom.h = static_cast<int>(mTexHeight * mActor->GetScale());
    r_bottom.x = static_cast<int>(mActor->GetPosition().x - r_bottom.w / 2);
    r_bottom.y = static_cast<int>(mActor->GetPosition().y - r_bottom.h / 2 + mOffsetY); // オフセットを加える
    SDL_RenderCopy(renderer,
        mTexture,
        nullptr,
        &r_bottom);

    // 通常テクスチャの上部にもう一枚描画
    SDL_Rect r_top;
    r_top.w = static_cast<int>(mTexWidth * mActor->GetScale());
    r_top.h = static_cast<int>(mTexHeight * mActor->GetScale());
    r_top.x = static_cast<int>(mActor->GetPosition().x - r_top.w / 2);
    r_top.y = static_cast<int>(mActor->GetPosition().y - r_top.h / 2 - Game::ScreenHeight + mOffsetY); // スクリーンサイズ分引く
    SDL_RenderCopy(renderer,
        mTexture,
        nullptr,
        &r_top);

    */
    
    // 横方向
    SDL_Rect r_bottom;
    r_bottom.w = static_cast<int>(mTexWidth * mActor->GetScale());
    r_bottom.h = static_cast<int>(mTexHeight * mActor->GetScale());
    r_bottom.x = static_cast<int>(mActor->GetPosition().x - r_bottom.w / 2  - mOffsetX); // オフセットを加える
    r_bottom.y = static_cast<int>(mActor->GetPosition().y - r_bottom.h / 2);
    SDL_RenderCopy(renderer,
        mTexture,
        nullptr,
        &r_bottom);
    
    // 通常テクスチャの上部にもう一枚描画
    // 1スクリーンサイズ分左にずらす
    SDL_Rect r_top;
    r_top.w = static_cast<int>(mTexWidth * mActor->GetScale());
    r_top.h = static_cast<int>(mTexHeight * mActor->GetScale());
    r_top.x = static_cast<int>(mActor->GetPosition().x - r_top.w / 2 + Game::ScreenWidth - mOffsetX); // スクリーンサイズ分引く
    r_top.y = static_cast<int>(mActor->GetPosition().y - r_top.h / 2);
    SDL_RenderCopy(renderer,
        mTexture,
        nullptr,
        &r_top);

}