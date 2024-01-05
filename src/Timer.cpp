#include "Timer.h"
#include "Game.h"
#include "SpriteComponent.h"
#include <iostream>


Timer::Timer(class Game* game) 
    :Actor(game)
    ,mStart(0), mEnd(0), mDifftime(0)
{
    mSprite = new SpriteComponent(this);
    
}

// アクタ更新
void Timer::UpdateActor(float deltaTime)
{
}
