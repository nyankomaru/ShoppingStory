#pragma once

//SpriteやTexture,Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

//Vector2fクラスを使う為のヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//向きの定数を使うのでinclude
#include "GameDef.h"

//コリジョンクラスを使うのでinclude
#include"Fwk/Collision/Collider.h"
//コリジョンクラスの名前空間を使う
using namespace Fwk::Collision;

//音源クラスを#include
#include "Fwk/Audio/SoundSource.h"
//Waveファイルを扱うクラスを#include
#include "Fwk/Audio/WaveSound.h"

//武器：剣クラス
class Sword
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

	//位置設定
	void SetPosition(Vector2f position);

	//指定の方向に剣を振る
	void Swing(Direction direction);

	//剣が振られているか
	bool IsSwinged();

private:

	//アニメーションの初期化
	void _initAnimation();

	//アニメーションイベントのコールバック関数
	void OnAnimationEvent(const AnimationEvent& animEvent);

	//衝突イベントのコールバック関数
	void OnHit(const CollisionEvent& collisionEvent);

private:
	Texture mTexture;
	Sprite mSprite;
	Direction mDirection;
	Collider mCollision;
	//効果音
	WaveSound mSound;
	//音源
	SoundSource mSoundSoure;
};