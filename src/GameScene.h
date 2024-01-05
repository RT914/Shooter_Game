#pragma once
#include "Scene.h"


// ゲームシーンクラス
class GameScene : public Scene {
public:
    GameScene(class Game* game);
    ~GameScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;
private:
    class Actor* mStartMsg; // 開始メッセージ
    class Actor* mRoulette; // ルーレット
    class Actor* mRouletteBlock; // ルーレット
    bool mStartMsgFlag; // 開始メッセージが存在するかどうかのフラグ
    bool mRouletteFlag; // 開始メッセージが存在するかどうかのフラグ
    
};
