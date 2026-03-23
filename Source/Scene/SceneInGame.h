#pragma once

//シーンクラスを継承するのでinclude
#include "Scene.h"

//音源クラスの定義を#include
#include "Fwk/Audio/SoundSource.h"
//MP3ファイルを扱うクラスの#include
#include "Fwk/Audio/MP3Sound.h"

//ゲームオーバーシーンクラス
class SceneInGame:public Scene
{
public:
	//初期化
	void Init();
	//終了
	void Term();
	//更新
	void Update();
	//描画
	void Render();

private:
	//BGM読み込み用
	MP3Sound mSound;
	//音源
	SoundSource mSoundSource;
};