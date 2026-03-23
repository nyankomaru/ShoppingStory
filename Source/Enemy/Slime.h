#pragma once

//(敵)スライムクラス

//親クラス
#include "Enemy.h"

//Directionの定数を使いたいので#include
#include "GameDef.h"

class Slime : public Enemy
{
	//敵の情報
	enum class Status {
		Falling,	//落下中
		Moving,		//水平移動中
	};

public:

	//初期化
	void Init();
	//更新
	void Update();

private:
	//落下時の処理
	void _updateFalling();
	//移動時の処理
	void _updateMoving();

private:
	//状態
	Status mStatus;
	//加速度
	Vector2f mVelocity;
	//水平移動時の向き
	Direction mDirection;
};