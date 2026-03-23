//Cat.h

#pragma once

//(敵)猫クラス

//親クラス
#include "Enemy.h"

//Directionの定数を使いたいので#include
#include "GameDef.h"
#include <Lib/Math/Math.h>

class Cat : public Enemy
{
	//敵の情報
	enum class Status {
		Waiting,	//生成前
		Falling,	//落下時
		Walking,	//移動
		Reserving,	//準備
		Jumping,	//ジャンプ
	};

public:

	//初期化
	void Init();
	//更新
	void Update();

private:
	//生成前の処理
	void _updateWaiting();
	//落下時の処理
	void _updateFalling();
	//歩き時の処理
	void _updateWalking();
	//準備の処理
	void _updateReserving();
	//ジャンプ時の処理
	void _updateJumping();

private:
	bool _IsOnGround();
	void _initAnimation();											//アニメーションの初期化
	float _GetDistanceBetweenTwo();									//プレイヤーとの距離を返す
	void _SetDistanceBetweenTwo();									//プレイヤーと猫の二点間の距離を測る処理
	void _CheckRound();		//地面と接しているか確認
	void _applyGravityIfNoGround();

private:
	//状態
	Status mStatus;
	//加速度
	Vector2f mVelocity;
	//水平移動時の向き
	Direction mDirection;
	Direction mJumpDirection; // ジャンプ方向を保持（ジャンプ中は変えない）

private:
	float mDistanceX;		//プレイヤーと猫のX軸の距離
};