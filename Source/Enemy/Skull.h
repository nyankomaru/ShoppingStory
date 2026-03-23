#pragma once

//(敵)骸骨クラス

//親クラス
#include "Enemy.h"

//Directionの定数を使いたいので#include
#include "GameDef.h"

class Skull : public Enemy
{
	//敵の情報
	enum class Status {
		Moving,		//移動中
		Idle,		//待機中
	};

public:

	//初期化
	void Init();
	//更新
	void Update();

private:
	//移動時の処理
	void _updateMoving();
	//待機状態の更新処理
	void _updateIdle();
	//加速度の更新
	void _updateVelocity();
	//アニメーションの初期化
	void _initAnimation();
	//やられたときに呼び出される
	void OnDefeated();

private:
	//状態
	Status mStatus;
	//経過時間
	float mTimer;
	//加速度
	Vector2f mVelocity;
	//水平移動時の向き
	Direction mDirection;
};