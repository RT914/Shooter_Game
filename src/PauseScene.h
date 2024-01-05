#ifndef _PAUSESCENE_H_
#define _PAUSESCENE_H_

#include "Scene.h"

// �������V�[���N���X
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
    class Actor* mBackButton; // ���j���[�֖߂�{�^��
    class Actor* mCancelButton; // �Q�[���V�[���֖߂�{�^��
    class Actor* mBackButtonMsg; // ���j���[�֖߂�{�^���̃��b�Z�[�W
    class Actor* mCancelButtonMsg; // �Q�[���V�[���֖߂�{�^���̃��b�Z�[�W
};

#endif