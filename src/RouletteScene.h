#ifndef _ROULETTESCENE_H_
#define _ROULETTESCENE_H_

#include "Scene.h"

// �������V�[���N���X
class RouletteScene : public Scene {
public:
    RouletteScene(class Game* game);
    ~RouletteScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    class Actor* mRouletteMsg; // ���[���b�g���b�Z�[�W
    class Actor* mStopMsg;
    class Actor* mRoulette; // ���[���b�g
    class Actor* mRouletteBlock; // ���[���b�g
    bool stopflag; // ���[���b�g
};

#endif