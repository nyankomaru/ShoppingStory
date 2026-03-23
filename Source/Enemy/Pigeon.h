//Pigeon.h

#pragma once

//(敵)鳩クラス

//親クラス
#include "Enemy.h"

//Directionの定数を使いたいので#include
#include "GameDef.h"
#include <Lib/Math/Math.h>

class Pigeon : public Enemy
{
	//敵の情報
	enum class Status {
		Waiting,	//生成前
		Staying,	//待機中
		Reserving,	//準備中
		Flying,		//滑空中
	};

public:

	//初期化
	void Init();
	//更新
	void Update();

private:
	int mColNum;
	int mRowNum;
	float mDistanceX;						//プレイヤーと敵とのX距離
	bool mHasMeasuredDistance = false;		// プレイヤーとの距離を一度だけ測るフラグ
	float mInitialY = 0.0f;					// 生成時のY座標を保持
	Vector2f mStartPos;						// 飛行開始位置
	Vector2f mControlPos;					// 中間点（プレイヤー位置）
	Vector2f mEndPos;						// 終点（画面反転）
	float mFlyingTimer;						// 飛行時間の累積
	float mFlyingDuration;					// 飛行完了までの時間（例えば2秒）
	Vector2f mControlOffset;				// プレイヤー位置の差分補正用
	Vector2f mPlayerPosOnFlyStart;			// Flying開始時のプレイヤー位置（相対移動補正用）
	float mFlyingStartY;					// 追加：飛行開始Y座標を保持する変数
	Vector2f mInitialControlPos;			// 飛行開始時の制御点（Y座標固定のため）
	Vector2f mInitialEndPos;				// 飛行開始時の終点（X方向シフトに対応）
	Vector2f mInitialPos;					// Reserving 開始時の中心位置（回転の中心かつ飛行開始点）
	float mReservingRadius = 40.f;			// 回転半径
	float mReservingRotSpeed = 2.5f * PI;	// 回転速度（1秒で半周 = πラジアン）

private:
	//生成前の処理
	void _updateWaiting();
	//待機時の処理
	void _updateStaying();
	//飛立つ準備の処理
	void _updateReserving();
	//滑空時の処理
	void _updateFlying();

private:
	float _GetDistanceBetweenTwo();					//プレイヤーと鳩の二点間の距離を返す処理
	void _SetDistanceBetweenTwo();					//プレイヤーと鳩の二点間の距離を測る処理
	bool _IsInSafeView(float margin);				//安全マージン付きの画面内判定処理
	void _initAnimation();							//アニメーションの初期化

private:
	//状態
	Status mStatus;
	//加速度
	Vector2f mVelocity;
	//水平移動時の向き
	Direction mDirection;
};