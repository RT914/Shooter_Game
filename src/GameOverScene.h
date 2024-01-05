#ifndef _GAMEOVERSCENE_H_
#define _GAMEOVERSCENE_H_

#include "Scene.h"

// 初期化シーンクラス
class GameOverScene : public Scene {
public:
    GameOverScene(class Game* game);
    ~GameOverScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    class Actor* mGameOverMsg;   // ゲームオーバーメッセージ
    class Actor* mRecordMsg;   // 記録メッセージ
};

#endif