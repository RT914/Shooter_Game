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

// ゲームの初期化
bool Game::Initialize() {

	// SDL初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());
		return false;
	}

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"ball avoidance game", // ウィンドウのタイトル
		-200, // ウィンドウ左上隅のx座標
		50, // ウィンドウ左上隅のy座標
		ScreenWidth, // ウィンドウの幅(width)
		ScreenHeight, // ウィンドウの高さ(height)
		SDL_WINDOW_FULLSCREEN_DESKTOP
	);

	if (!mWindow) {
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// シーン初期化
	InitScene();

	// SDL_mixerの初期化
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Log("SDL_mixerを初期化できません : %s", SDL_GetError());
		return false;
	}
		

	Mix_Init(MIX_INIT_MP3); // 利用するフォーマットを指定
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048); // オーディオの設定

	
	// 音声の読み込み
	if (!ReadBGMs()) {
		return false;
	}
	
	// SEの読み込み
	if (!ReadSEs()) {
		return false;
	}
	

	TTF_Init();  // TrueTypeフォントライブラリの初期化
	

	return true;
}

// シーン初期化処理（一度だけ呼ばれる）
void Game::InitScene()
{
	
	// 背景の作成
	// 背景をスクロールさせる処理
	auto* bgBack = new Actor(this);
	bgBack->SetPosition(Vector2(ScreenWidth / 2, ScreenHeight / 2));
	auto* bgBackSprite = new ScrollSpriteComponent(bgBack, 10);
	bgBackSprite->SetTexture(LoadTexture(GetAssetsPath() + "factory.png"));
	bgBackSprite->SetScrollSpeedX(100.0f); // 速度：100
	
}

void Game::RunLoop() {

	// 開始シーンを設定
	mScene = new LaunchScene(this);
	mNextScene = mScene;
	StartScene();
	
	while (mIsRunning) 
	{
		// シーン更新処理
		UpdateScene();
		//cout << mScene->GetSceneName() << endl;

		if (mScene->GetSceneName().compare(mNextScene->GetSceneName()) != 0)
		{
			delete mScene;
			mScene = mNextScene;
			StartScene();
		}
		
	}
	SDL_Delay(1000); //1秒遅延
}

// シャットダウン
void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

	Mix_CloseAudio();
	Mix_Quit();
}

// アクタ追加処理
void Game::AddActor(Actor* actor)
{
	// アクタ更新中ならPendingに格納
	mUpdatingActors
		? mPendingActors.emplace_back(actor)
		: mActors.emplace_back(actor);
}

// シーン開始処理
void Game::StartScene()
{
	mScene->Start();
}

// シーン更新処理
void Game::UpdateScene()
{
	ProcessInput();

	//printf("chech\n");

	//1frame=16ms
	//前のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//ms→s
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatimeを最大値で制限する
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	
	// ここ重要かもしれないから理解したい
	// アクタ更新処理
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待機中のアクタを追加
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 各シーンの更新処理
	mScene->Update(deltaTime);


	// 死亡したアクタを破棄
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

// アクタ削除処理
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

// 描画中のスプライト追加処理
void Game::AddSprite(SpriteComponent* sprite)
{
	// 描画順にソートして追加
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

// 描画中のスプライト削除処理
void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

// エネミー追加
void Game::AddEnemy(Enemy* enemy)
{
	mEnemies.emplace_back(enemy);
}

// エネミー削除
void Game::RemoveEnemy(Enemy* enemy)
{
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end())
	{
		mEnemies.erase(iter);
	}
}

// クロック追加
void Game::AddClock(Clock* clock)
{
	mClocks.emplace_back(clock);
}

// クロック削除
void Game::RemoveClock(Clock* clock)
{
	auto iter = std::find(mClocks.begin(), mClocks.end(), clock);
	if (iter != mClocks.end())
	{
		mClocks.erase(iter);
	}
}

// 音声の読み込み
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

	// 音量調整(0~128)
	Mix_VolumeMusic(10);

	return true;
}

// SEの読み込み
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

	// 音量調整(0~128)
	Mix_Volume(-1, 10);

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	// キー入力を検知
	// ESCAPE入力されると終了
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mScene->ProcessInput(state);

}

void Game::GenerateOutput() {

	// 描画色を青色に設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 176, 255, 255 // (R,G,B,A)
	);

	// バックバッファを青色で塗りつぶす
	SDL_RenderClear(mRenderer);

	// スプライトを描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// バックバッファとフロントバッファを入れ替え
	SDL_RenderPresent(mRenderer);
}

// ファイル名からテクスチャをロードする
SDL_Texture* Game::LoadTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mCachedTextures.find(fileName);
	if (iter != mCachedTextures.end())
	{
		// キャッシュ済なら変数から取得
		tex = iter->second;
	}
	else
	{
		// テクスチャをロードする
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
		// 変数にキャッシュする
		mCachedTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

// ファイル名からテクスチャをロードする
SDL_Texture* Game::LoadFont(const std::string& fileName, const std::string& str, const std::vector<int> rgb, const int &size) {
	
	SDL_Texture* tex = nullptr;

	TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);  // フォントの読み込み
	
	// テキストの色設定
	SDL_Color color = { rgb[0], rgb[1], rgb[2] };

	// テクスチャをロードする
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