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
	//ゲームを初期化する
	bool Initialize();
	//ゲームオーバーまでゲームループを実行する
	void RunLoop();
	//ゲームをシャットダウンする
	void Shutdown();

	// BGMファイルの読み込み
	bool ReadBGMs();

	// SoundEffectファイルの読み込み
	bool ReadSEs();

	void AddActor(class Actor* actor);    // アクタ追加
	void RemoveActor(class Actor* actor); // アクタ削除

	void AddSprite(class SpriteComponent* sprite);    // 描画中のスプライトを追加
	void RemoveSprite(class SpriteComponent* sprite); // 描画中のスプライトを削除

	void AddEnemy(class Enemy* enemy);    // エネミー追加
	void RemoveEnemy(class Enemy* enemy); // エネミー削除

	void AddClock(class Clock* clock); // クロック追加
	void RemoveClock(class Clock* clock); // クロック削除

	SDL_Texture* LoadTexture(const std::string& fileName); // テクスチャロード処理

	SDL_Texture* LoadFont(const std::string& fileName, const std::string& str, const std::vector<int> rgb, const int& size); // フォントロード処理

	/*
	constexpr static const float ScreenWidth = 2048.0f;  // スクリーン横幅
	constexpr static const float ScreenHeight = 768.0f; // スクリーン縦幅
	*/
	
	constexpr static const float ScreenWidth = 1920.0f;  // スクリーン横幅
	constexpr static const float ScreenHeight = 1080.0f; // スクリーン縦幅

private:
	
	void InitScene();      // シーン初期化処理
	void StartScene();     // シーン開始処理
	void UpdateScene();    // シーン更新処理
	void ProcessInput();   // 入力検知 
	void GenerateOutput(); // 出力処理

	class Scene* mScene;     // 現在のシーン
	class Scene* mNextScene; // 遷移するシーン

	std::vector<class Actor*> mActors;            // アクタリスト
	std::vector<class Actor*> mPendingActors;     // 待機中のアクタリスト
	std::vector<class SpriteComponent*> mSprites; // 描画中のスプライトリスト
	std::unordered_map<std::string, SDL_Texture*> mCachedTextures; // キャッシュしたテクスチャリスト
	// std::unordered_map<std::string, SDL_Texture*> mCachedFonts; // キャッシュしたフォントリスト

	// 入力による移動距離
	Eigen::Vector2d move_distance;

	//SDLが作るウィンドウ
	SDL_Window* mWindow;

	//2D描画のためのレンダリング
	SDL_Renderer* mRenderer;

	// Number of ticks since start of game
	Uint32 mTicksCount;

	//ゲームの続行を支持する
	bool mIsRunning;

	// アクタ更新中か否か？
	bool mUpdatingActors;

	// ゲームをクリアしたか否か？
	bool mGameClear;

	// 現在のゲームシーン
	std::string SceneName;

	// 画像パス
	//const std::string AssetsPath = "";
	const std::string AssetsPath = "images\\"; // Win + VisualStudio
	// フォントパス
	const std::string FontAssetsPath = "font\\"; // Win + VisualStudio

	class Player* mPlayer; // プレイヤー

	std::vector<class Enemy*> mEnemies; // エネミー群

	class Timer* mTimer; // タイマー

	int mRoulette; // ルーレット

	std::vector<class Clock*> mClocks; // クロック群

	bool pauseFlag; // ポーズ状態か否か

	bool rouletteFlag; // ルーレット状態か否か

	bool initFlag; // 初期化したかしていないか
	
	int ReturnToTheRouletteTime; // ルーレット画面から戻った時間

	int RouletteTime = 0; // ルーレット画面で経過した時間

	int mLevel; // ゲームの難易度

	std::unordered_map<std::string, Mix_Music*> mBGMs; // bgm群

	std::unordered_map<std::string, Mix_Chunk*> mSoundEffects; // 効果音群

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
