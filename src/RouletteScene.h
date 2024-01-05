#ifndef _ROULETTESCENE_H_
#define _ROULETTESCENE_H_

#include "Scene.h"

// 初期化シーンクラス
class RouletteScene : public Scene {
public:
    RouletteScene(class Game* game);
    ~RouletteScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    class Actor* mRouletteMsg; // ルーレットメッセージ
    class Actor* mStopMsg;
    class Actor* mRoulette; // ルーレット
    class Actor* mRouletteBlock; // ルーレット
    bool stopflag; // ルーレット
};

#endif