#pragma once

//体力ゲージ
#include "HpGauge.h"
#include "HpGauge2.h"

//UIマネージャー

class UIMng
{
public:

	//初期化
	void Init();
	//後片付け
	void Term();
	//更新
	void Update();
	//描画
	void Render();
	//体力の設定
	void SetHp(int hp);
	void SetHp(float hp);

private:
	//体力ゲージ
	HpGauge mHpGauge;
	//体力ゲージ２
	HpGauge2 mHpGauge2;
};