#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "Scene.h"

// �������V�[���N���X
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
    class Actor* mBack; // ���j���[��ʂ̔w�i
    class Actor* mButton; // �{�^��
    class Actor* mButtonMsg; // �{�^���̃��b�Z�[�W
    class Actor* mFinishButton; // �{�^��
    class Actor* mFinishButtonMsg; // �{�^���̃��b�Z�[�W
    
};

#endif