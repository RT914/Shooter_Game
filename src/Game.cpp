#include <iostream>
#include <Eigen/Dense>
#include <time.h>
#include "SDL_image.h"
#include <vector>
#include "Player.h"
#include "Game.h"
#include "Math.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "Actor.h"
#include "Enemy.h"
#include "ScrollSpriteComponent.h"
#include "LaunchScene.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>


std::vector<int> inputkeyboard{0, 0};

int window_length[2] = { Game::ScreenWidth, Game::ScreenHeight};

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mUpdatingActors(false), mGameClear(false)
{

}

// �Q�[���̏�����
bool Game::Initialize() {

	// SDL������
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDL���������ł��܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"ball avoidance game", // �E�B���h�E�̃^�C�g��
		-200, // �E�B���h�E�������x���W
		50, // �E�B���h�E�������y���W
		ScreenWidth, // �E�B���h�E�̕�(width)
		ScreenHeight, // �E�B���h�E�̍���(height)
		SDL_WINDOW_FULLSCREEN_DESKTOP
	);

	if (!mWindow) {
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[���쐬
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// �V�[��������
	InitScene();

	// SDL_mixer�̏�����
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Log("SDL_mixer���������ł��܂��� : %s", SDL_GetError());
		return false;
	}
		

	Mix_Init(MIX_INIT_MP3); // ���p����t�H�[�}�b�g���w��
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048); // �I�[�f�B�I�̐ݒ�

	
	// �����̓ǂݍ���
	if (!ReadBGMs()) {
		return false;
	}
	
	// SE�̓ǂݍ���
	if (!ReadSEs()) {
		return false;
	}
	

	TTF_Init();  // TrueType�t�H���g���C�u�����̏�����
	

	return true;
}

// �V�[�������������i��x�����Ă΂��j
void Game::InitScene()
{
	
	// �w�i�̍쐬
	// �w�i���X�N���[�������鏈��
	auto* bgBack = new Actor(this);
	bgBack->SetPosition(Vector2(ScreenWidth / 2, ScreenHeight / 2));
	auto* bgBackSprite = new ScrollSpriteComponent(bgBack, 10);
	bgBackSprite->SetTexture(LoadTexture(GetAssetsPath() + "factory.png"));
	bgBackSprite->SetScrollSpeedX(100.0f); // ���x�F100
	
}

void Game::RunLoop() {

	// �J�n�V�[����ݒ�
	mScene = new LaunchScene(this);
	mNextScene = mScene;
	StartScene();
	
	while (mIsRunning) 
	{
		// �V�[���X�V����
		UpdateScene();
		//cout << mScene->GetSceneName() << endl;

		if (mScene->GetSceneName().compare(mNextScene->GetSceneName()) != 0)
		{
			delete mScene;
			mScene = mNextScene;
			StartScene();
		}
		
	}
	SDL_Delay(1000); //1�b�x��
}

// �V���b�g�_�E��
void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

	Mix_CloseAudio();
	Mix_Quit();
}

// �A�N�^�ǉ�����
void Game::AddActor(Actor* actor)
{
	// �A�N�^�X�V���Ȃ�Pending�Ɋi�[
	mUpdatingActors
		? mPendingActors.emplace_back(actor)
		: mActors.emplace_back(actor);
}

// �V�[���J�n����
void Game::StartScene()
{
	mScene->Start();
}

// �V�[���X�V����
void Game::UpdateScene()
{
	ProcessInput();

	//printf("chech\n");

	//1frame=16ms
	//�O�̃t���[������16ms���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//ms��s
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatime���ő�l�Ő�������
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	
	// �����d�v��������Ȃ����痝��������
	// �A�N�^�X�V����
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �ҋ@���̃A�N�^��ǉ�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// �e�V�[���̍X�V����
	mScene->Update(deltaTime);


	// ���S�����A�N�^��j��
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors)
	{
		delete actor;
	}

	GenerateOutput();
}

// �A�N�^�폜����
void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		mPendingActors.erase(iter);
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}

// �`�撆�̃X�v���C�g�ǉ�����
void Game::AddSprite(SpriteComponent* sprite)
{
	// �`�揇�Ƀ\�[�g���Ēǉ�
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

// �`�撆�̃X�v���C�g�폜����
void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

// �G�l�~�[�ǉ�
void Game::AddEnemy(Enemy* enemy)
{
	mEnemies.emplace_back(enemy);
}

// �G�l�~�[�폜
void Game::RemoveEnemy(Enemy* enemy)
{
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end())
	{
		mEnemies.erase(iter);
	}
}

// �N���b�N�ǉ�
void Game::AddClock(Clock* clock)
{
	mClocks.emplace_back(clock);
}

// �N���b�N�폜
void Game::RemoveClock(Clock* clock)
{
	auto iter = std::find(mClocks.begin(), mClocks.end(), clock);
	if (iter != mClocks.end())
	{
		mClocks.erase(iter);
	}
}

// �����̓ǂݍ���
bool Game::ReadBGMs() {
	auto bgm = Mix_LoadMUS("music/canon.mp3");
	if (bgm == NULL) {
		return false;
	}
	else {
		mBGMs.insert(std::make_pair("start", bgm));
	}

	bgm = Mix_LoadMUS("music/Bright.mp3");
	if (bgm == NULL) {
		return false;
	}
	else {
		mBGMs.insert(std::make_pair("game", bgm));
	}

	// ���ʒ���(0~128)
	Mix_VolumeMusic(10);

	return true;
}

// SE�̓ǂݍ���
bool Game::ReadSEs() {
	auto se = Mix_LoadWAV("SE/start.wav");
	if (se == NULL) {
		return false;
	}
	else {
		mSoundEffects.insert(std::make_pair("start", se));
	}

	se = Mix_LoadWAV("SE/falling.wav");
	if (se == NULL) {
		return false;
	}
	else {
		mSoundEffects.insert(std::make_pair("falling", se));
	}

	se = Mix_LoadWAV("SE/button.wav");
	if (se == NULL) {
		return false;
	}
	else {
		mSoundEffects.insert(std::make_pair("button", se));
	}

	// ���ʒ���(0~128)
	Mix_Volume(-1, 10);

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	// �L�[���͂����m
	// ESCAPE���͂����ƏI��
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mScene->ProcessInput(state);

}

void Game::GenerateOutput() {

	// �`��F��F�ɐݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 176, 255, 255 // (R,G,B,A)
	);

	// �o�b�N�o�b�t�@��F�œh��Ԃ�
	SDL_RenderClear(mRenderer);

	// �X�v���C�g��`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
	SDL_RenderPresent(mRenderer);
}

// �t�@�C��������e�N�X�`�������[�h����
SDL_Texture* Game::LoadTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mCachedTextures.find(fileName);
	if (iter != mCachedTextures.end())
	{
		// �L���b�V���ςȂ�ϐ�����擾
		tex = iter->second;
	}
	else
	{
		// �e�N�X�`�������[�h����
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Error load texture file %s", fileName.c_str());
			return nullptr;
		}
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Error convert surface to texture %s", fileName.c_str());
			return nullptr;
		}
		// �ϐ��ɃL���b�V������
		mCachedTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

// �t�@�C��������e�N�X�`�������[�h����
SDL_Texture* Game::LoadFont(const std::string& fileName, const std::string& str, const std::vector<int> rgb, const int &size) {
	
	SDL_Texture* tex = nullptr;

	TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);  // �t�H���g�̓ǂݍ���
	
	// �e�L�X�g�̐F�ݒ�
	SDL_Color color = { rgb[0], rgb[1], rgb[2] };

	// �e�N�X�`�������[�h����
	SDL_Surface* surf = TTF_RenderText_Solid(font, str.c_str(), color);
	if (!surf)
	{
		SDL_Log("Error load font file %s", fileName.c_str());
		return nullptr;
	}
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Error convert surface to font %s", fileName.c_str());
		return nullptr;
	}

	return tex;
}