#ifndef _SCROLLSPRITECOMPONENT_H_
#define _SCROLLSPRITECOMPONENT_H_

#include "SpriteComponent.h"
#include "Vector2.h"

// スプライトスクロールコンポーネント
// *設定した速度でスプライトをスクロールする
class ScrollSpriteComponent : public SpriteComponent {
public:
    ScrollSpriteComponent(class Actor* actor, int drawOrder = 100);
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

private:
    float mScrollSpeedX; // X方向のスクロール速度
    float mOffsetX;      // X方向のオフセット値

    float mScrollSpeedY; // Y方向のスクロール速度
    float mOffsetY;      // Y方向のオフセット値

public:
    // Setter, Getter
    void SetScrollSpeedX(float speed) { mScrollSpeedX = speed; }
    void SetScrollSpeedY(float speed) { mScrollSpeedY = speed; }

};

#endif
