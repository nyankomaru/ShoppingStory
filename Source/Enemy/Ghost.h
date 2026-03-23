#pragma once

//(敵)幽霊クラス

//親クラス
#include "Enemy.h"
//列挙型Directionを使うので#include
#include "GameDef.h"

class Ghost : public Enemy
{
public:
	//初期化
	void Init();
	//更新
	void Update();
	//生成された時に呼び出される
	void OnCreated();

private:
	//原点
	Vector2f mOrigin;
	//経過時間
	float mTimer;
	//水平方向の秒間移動スピード
	float mSpeed;
	//垂直方向の最大単位(振幅)
	float mAmp;
	//周期(秒)
	float mPeriodTime;
	//何周期で進行方向を反転するか
	int mTurnPeriod;
	//現在の向き
	Direction mDirection;
};