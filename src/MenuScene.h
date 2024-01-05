#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "Scene.h"

// 初期化シーンクラス
class MenuScene : public Scene {
public:
    MenuScene(class Game* game);
    ~MenuScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;
    int NowButtonNum = 0;
    int NextButtonNum = 0;
    int startTime;

private:
    class Actor* mBack; // メニュー画面の背景
    class Actor* mButton; // ボタン
    class Actor* mButtonMsg; // ボタンのメッセージ
    class Actor* mFinishButton; // ボタン
    class Actor* mFinishButtonMsg; // ボタンのメッセージ
    
};

#endif