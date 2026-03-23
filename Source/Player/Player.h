//Player.h

#pragma once

//SpriteやTexture,Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

//Vector2fクラスを使う為のヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//向きの定数を使うのでinclude
#include "GameDef.h"

//Swordクラスを使うのでinclude
#include "Weapon/Sword.h"

//コリジョンクラスを使うのでinclude
#include "Fwk/Collision/Collider.h"
//コリジョンクラスの名前空間を使う
using namespace Fwk::Collision;

//音源クラスのを#include
#include "Fwk/Audio/SoundSource.h"
//Waveファイルを扱うクラスを#include
#include "Fwk/Audio/WaveSound.h"
#include "Product/Product.h"


//プレイヤークラス
class Player 
{
	//プレイヤーの状態
	enum class Status {
		Moving,
		Attack,
		Damaged,
		Dying,
		Dead,
		Goal,
	};

	//使う効果音の種類
	enum class Sound {
		Jump,
		Damaged,

		Count
	};

protected:

public:

	//プレイヤーの初期化
	void Init();

	//プレイヤーの更新
	void Update();

	//プレイヤーの描画
	void Render();

	//プレイヤーの後片付け
	void Term();

	//現在位置を取得する
	Vector2f GetPosition();
	//現在位置を設定する
	void SetPosition(Vector2f pos);

	//現在の体力を取得する
	int GetHp();
	float GetNormalizedHp();

	//自分がやられ状態でなければtrueを返す
	bool IsAlive();
	//自分がやられ状態であればtrueを返す
	bool IsDead();

	//ゴールに到達したか否かを取得する
	bool IsReacheGoal();

private:
	//アニメーションの初期化
	void _initAnimation();
	//現在のステータスが移動中の場合の更新処理
	void _updateMoving();
	//現在のステータスが攻撃の場合の更新処理
	void _updateAttack();
	//現在のステータスがダメージを受けている場合の更新処理
	void _updateDamaged();
	//現在のステータスがやられ中の場合の更新処理
	void _updateDying();
	//衝突イベントのコールバック関数
	void OnHit(const Collider& CollisionA, const Collider& CollisionB);
	//頭の衝突イベントのコールバック関数
	void OnHeadHit(const Collider& CollisionA, const Collider& CollisionB);
	//体の衝突イベントのコールバック関数
	void OnBodyHit(const Collider& CollisionA, const Collider& CollisionB);
	//脚の衝突イベントのコールバック関数
	void OnLegsHit(const Collider& CollisionA, const Collider& CollisionB);
	//与えた移動ベクトル
	void _translate(Vector2f vMove);
	//無敵関連の処理
	void _updateInvincibility();
	//速度を更新する
	void _updateVelocity(Vector2f vInput);
	//今自分が地面に立っているか
	bool _isGround();
	//今自分が天井に接しているか
	bool _isCeil();
	//自分をやられ状態にする
	void _die();
	//部位ごとの共通処理
	void _handlePartHit(const Collider& selfCollider, const Collider& otherCollider, ProductSlot part);

private:
	//表示用スプライト
	Sprite mSprite;
	//テクスチャ
	Texture mTexture;
	//オブジェクトの位置
	Vector2f mPosition;
	//現在の状態
	Status mStatus;
	//現在の向き
	Direction mDirection;
	//剣
	Sword mSword;
	//衝突処理用
	Collider mCollision;
	//汎用タイマー
	float mTimer;
	//汎用ステップカウンタ
	int mStep;
	//無敵中の残りフレーム
	int mInvicibilityFrames;
	//速度
	Vector2f mVelocity;
	//体力
	int mHp;
	//体力(最大値)
	int mHpMax;
	//効果音
	WaveSound mSounds[(int)Sound::Count];
	//音源
	SoundSource mSoundSoureces[(int)Sound::Count];
	//ジャンプをしているかどうか
	bool mIsJumping = false;
	//ジャンプ時のX座標の固定
	float mJumpVelocityX = 0.0f;
	//ジャンプ時の向き
	Direction mJumpDirection = Direction::Right;
	//頭の判定
	Collider mHeadCollision;
	//体の判定
	Collider mBodyCollision;
	//脚の判定
	Collider mLegsCollision;
	//他の部位から代わりに奪う回数
	int mForceStealCount = 0;  
};