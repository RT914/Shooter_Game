#ifndef _ClOCK_H_
#define _CLOCK_H_

#include "Actor.h"
class Clock : public Actor {
public:
    Clock(class Game* game);
    ~Clock();
    void UpdateActor(float deltaTime) override; // アクタ更新
    void SetSprite(class SpriteComponent* sprite) { mSprite = sprite; }
    SpriteComponent* GetSprite() { return mSprite; }
private:
    // スプライト設定
    class SpriteComponent* mSprite;
};

#endif

