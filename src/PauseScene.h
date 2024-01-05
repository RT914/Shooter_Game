#ifndef _PAUSESCENE_H_
#define _PAUSESCENE_H_

#include "Scene.h"

// 初期化シーンクラス
class PauseScene : public Scene {
public:
    PauseScene(class Game* game);
    ~PauseScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;
    int NowButtonNum = 0;
    int NextButtonNum = 0;
    int startTime;

private:
    class Actor* mPauseBack;
    class Actor* mBackButton; // メニューへ戻るボタン
    class Actor* mCancelButton; // ゲームシーンへ戻るボタン
    class Actor* mBackButtonMsg; // メニューへ戻るボタンのメッセージ
    class Actor* mCancelButtonMsg; // ゲームシーンへ戻るボタンのメッセージ
};

#endif