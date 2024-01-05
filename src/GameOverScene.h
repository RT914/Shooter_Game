#ifndef _GAMEOVERSCENE_H_
#define _GAMEOVERSCENE_H_

#include "Scene.h"

// �������V�[���N���X
class GameOverScene : public Scene {
public:
    GameOverScene(class Game* game);
    ~GameOverScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    class Actor* mGameOverMsg;   // �Q�[���I�[�o�[���b�Z�[�W
    class Actor* mRecordMsg;   // �L�^���b�Z�[�W
};

#endif