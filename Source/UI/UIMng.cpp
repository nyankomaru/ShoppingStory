#include "UIMng.h"

//初期化
void UIMng::Init() {
	//体力ゲージの初期化
	mHpGauge.Init();
	mHpGauge2.Init();
}

//後片付け
void UIMng::Term() {
	//体力ゲージの後片付け
	mHpGauge.Term();
	mHpGauge2.Term();
}

//更新
void UIMng::Update() {
	//体力ゲージの更新
	mHpGauge.Update();
	mHpGauge2.Update();
}

//描画
void UIMng::Render() {
	//体力ゲージの描画
	mHpGauge.Render();
	mHpGauge2.Render();
}

//体力の設定
void UIMng::SetHp(int hp) {
	mHpGauge.SetHp(hp);
}

//体力の設定
void UIMng::SetHp(float hp) {
	mHpGauge2.SetHp(hp);
}