#ifndef _GAMECLEARSCENE_H_
#define _GAMECLEARSCENE_H_

#include "Scene.h"

// �������V�[���N���X
class GameClearScene : public Scene {
public:
    GameClearScene(class Game* game);
    ~GameClearScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    
};

#endif