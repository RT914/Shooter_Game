#ifndef _GAME_H_
#define _GAME_H_

#include <Eigen/Dense>
#include <vector>
#include <unordered_map>
#include <string>
#include "SDL.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>


class Game {
public:
	Game();
	//�Q�[��������������
	bool Initialize();
	//�Q�[���I�[�o�[�܂ŃQ�[�����[�v�����s����
	void RunLoop();
	//�Q�[�����V���b�g�_�E������
	void Shutdown();

	// BGM�t�@�C���̓ǂݍ���
	bool ReadBGMs();

	// SoundEffect�t�@�C���̓ǂݍ���
	bool ReadSEs();

	void AddActor(class Actor* actor);    // �A�N�^�ǉ�
	void RemoveActor(class Actor* actor); // �A�N�^�폜

	void AddSprite(class SpriteComponent* sprite);    // �`�撆�̃X�v���C�g��ǉ�
	void RemoveSprite(class SpriteComponent* sprite); // �`�撆�̃X�v���C�g���폜

	void AddEnemy(class Enemy* enemy);    // �G�l�~�[�ǉ�
	void RemoveEnemy(class Enemy* enemy); // �G�l�~�[�폜

	void AddClock(class Clock* clock); // �N���b�N�ǉ�
	void RemoveClock(class Clock* clock); // �N���b�N�폜

	SDL_Texture* LoadTexture(const std::string& fileName); // �e�N�X�`�����[�h����

	SDL_Texture* LoadFont(const std::string& fileName, const std::string& str, const std::vector<int> rgb, const int& size); // �t�H���g���[�h����

	/*
	constexpr static const float ScreenWidth = 2048.0f;  // �X�N���[������
	constexpr static const float ScreenHeight = 768.0f; // �X�N���[���c��
	*/
	
	constexpr static const float ScreenWidth = 1920.0f;  // �X�N���[������
	constexpr static const float ScreenHeight = 1080.0f; // �X�N���[���c��

private:
	
	void InitScene();      // �V�[������������
	void StartScene();     // �V�[���J�n����
	void UpdateScene();    // �V�[���X�V����
	void ProcessInput();   // ���͌��m 
	void GenerateOutput(); // �o�͏���

	class Scene* mScene;     // ���݂̃V�[��
	class Scene* mNextScene; // �J�ڂ���V�[��

	std::vector<class Actor*> mActors;            // �A�N�^���X�g
	std::vector<class Actor*> mPendingActors;     // �ҋ@���̃A�N�^���X�g
	std::vector<class SpriteComponent*> mSprites; // �`�撆�̃X�v���C�g���X�g
	std::unordered_map<std::string, SDL_Texture*> mCachedTextures; // �L���b�V�������e�N�X�`�����X�g
	// std::unordered_map<std::string, SDL_Texture*> mCachedFonts; // �L���b�V�������t�H���g���X�g

	// ���͂ɂ��ړ�����
	Eigen::Vector2d move_distance;

	//SDL�����E�B���h�E
	SDL_Window* mWindow;

	//2D�`��̂��߂̃����_�����O
	SDL_Renderer* mRenderer;

	// Number of ticks since start of game
	Uint32 mTicksCount;

	//�Q�[���̑��s���x������
	bool mIsRunning;

	// �A�N�^�X�V�����ۂ��H
	bool mUpdatingActors;

	// �Q�[�����N���A�������ۂ��H
	bool mGameClear;

	// ���݂̃Q�[���V�[��
	std::string SceneName;

	// �摜�p�X
	//const std::string AssetsPath = "";
	const std::string AssetsPath = "images\\"; // Win + VisualStudio
	// �t�H���g�p�X
	const std::string FontAssetsPath = "font\\"; // Win + VisualStudio

	class Player* mPlayer; // �v���C���[

	std::vector<class Enemy*> mEnemies; // �G�l�~�[�Q

	class Timer* mTimer; // �^�C�}�[

	int mRoulette; // ���[���b�g

	std::vector<class Clock*> mClocks; // �N���b�N�Q

	bool pauseFlag; // �|�[�Y��Ԃ��ۂ�

	bool rouletteFlag; // ���[���b�g��Ԃ��ۂ�

	bool initFlag; // ���������������Ă��Ȃ���
	
	int ReturnToTheRouletteTime; // ���[���b�g��ʂ���߂�������

	int RouletteTime = 0; // ���[���b�g��ʂŌo�߂�������

	int mLevel; // �Q�[���̓�Փx

	std::unordered_map<std::string, Mix_Music*> mBGMs; // bgm�Q

	std::unordered_map<std::string, Mix_Chunk*> mSoundEffects; // ���ʉ��Q

public:
	
	Scene* GetNextScene() const { return mNextScene; }
	void SetNextScene(class Scene* scene) { mNextScene = scene; }
	void SetGameClear(const bool isClear) { mGameClear = isClear; }
	std::string GetSceneName() const { return SceneName; }
	void SetSceneName( std::string str ) { SceneName = str; }
	Scene* GetScene() const { return mScene; }
	void SetScene(class Scene* scene) { mScene = scene; }
	std::vector<class Enemy*> GetEnemies() { return mEnemies; }
	std::string GetAssetsPath() const { return AssetsPath; }
	std::string GetFontAssetsPath() const { return FontAssetsPath; }
	void SetPlayer(class Player* player) { mPlayer = player; }
	Player* GetPlayer() const { return mPlayer; }
	void SetTimer(class Timer* timer) { mTimer = timer; }
	Timer* GetTimer() const { return mTimer; }
	void SetRoulette(int roulette) { mRoulette = roulette; }
	int GetRoulette() const { return mRoulette; }
	std::vector<class Clock*> GetClocks() { return mClocks; }
	void SetPauseFlag(const bool isPause) { pauseFlag = isPause; }
	bool GetPauseFlag() const { return pauseFlag; }
	void SetRouletteFlag(const bool isRoulette) { rouletteFlag = isRoulette; }
	bool GetRouletteFlag() const { return rouletteFlag; }
	void SetInitFlag(const bool isInit) { initFlag = isInit; }
	bool GetInitFlag() const { return initFlag; }
	void SetGameLevel(const bool level) { mLevel = level; }
	int GetGameLevel() const { return mLevel; }
	void SetReturnToTheRouletteTime(const int time) { ReturnToTheRouletteTime = time; }
	int GetReturnToTheRouletteTime() const { return ReturnToTheRouletteTime; }
	void AddRouletteTime(const int time) { RouletteTime += time; }
	int GetRouletteTime() const { return RouletteTime; }
	std::unordered_map<std::string, Mix_Music*> GetBGMs() { return mBGMs; }
	std::unordered_map<std::string, Mix_Chunk*> GetSoundEffects() { return mSoundEffects; }
	void SetlsRunning(bool tf) { mIsRunning = tf; }

};

#endif
