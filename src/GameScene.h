#pragma once
#include "Scene.h"


// �Q�[���V�[���N���X
class GameScene : public Scene {
public:
    GameScene(class Game* game);
    ~GameScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;
private:
    class Actor* mStartMsg; // �J�n���b�Z�[�W
    class Actor* mRoulette; // ���[���b�g
    class Actor* mRouletteBlock; // ���[���b�g
    bool mStartMsgFlag; // �J�n���b�Z�[�W�����݂��邩�ǂ����̃t���O
    bool mRouletteFlag; // �J�n���b�Z�[�W�����݂��邩�ǂ����̃t���O
    
};
