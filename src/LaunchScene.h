#ifndef _LAUNCHSCENE_H_
#define _LAUNCHSCENE_H_

#include "Scene.h"

// �������V�[���N���X
class LaunchScene : public Scene {
public:
    LaunchScene(class Game* game);
    ~LaunchScene();
    void Start() override;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* state) override;
    std::string const GetSceneName() override;

private:
    class Actor* mLaunch;
    class Actor* mCharacter;
    class Actor* mTitle;
    class Actor* mFrame;
};

#endif