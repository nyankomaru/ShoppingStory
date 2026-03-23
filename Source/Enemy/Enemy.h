//Enemy.h

#pragma once

//(敵)基底クラス

//SpriteやTexture,Animationを使う為のヘッダファイル
#include "Fwk/Graphics.h"

//Vector2fクラスを使う為のヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//コリジョンクラスを使うのでinclude
#include "Fwk/Collision/Collider.h"
//コリジョンクラスの名前空間を使う
using namespace Fwk::Collision;

//音源クラスを#include
#include "Fwk/Audio/SoundSource.h"
//Waveファイルを扱うクラス#include
#include "Fwk/Audio/WaveSound.h"

class Enemy
{
public:
	//初期化
	virtual void Init();
	//後片付け
	virtual void Term();
	//更新
	virtual void Update();
	//描画
	virtual void Render();

	//アクティブか？
	bool  IsActive();
	//アクティブ状態の設定
	void SetActive(bool isActive);

	//現在位置を取得
	Vector2f GetPosition();
	//現在位置を設定
	void SetPosition(Vector2f position);

	//ダメージを受ける
	virtual void OnDamaged(int damage);

	//生成された時に呼び戻す
	//派生クラスで上書きして使う
	virtual void OnCreated();

	//やられたときに呼び出される
	//派生クラスで上書きして使う
	virtual void OnDefeated();

protected:

	//アクティブ状態か否か
	bool mIsActive;
	//描画テクスチャ
	Texture mTexture;
	//描画用スプライト
	Sprite mSprite;
	//現在の位置
	Vector2f mPosition;
	//コリジョン
	Collider mCollision;
	//体力
	int mHP;
	//ダメージ時に加算する赤色の強さ(0.0~1.0)
	float mDamagedColor;
	//効果音
	WaveSound mSound;
	//音源
	SoundSource mSoundSource;
	//汎用タイマー
	float mTimer;
};