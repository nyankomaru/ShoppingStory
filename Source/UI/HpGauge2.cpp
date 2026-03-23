#include "HpGauge2.h"

//初期化
void HpGauge2::Init() {

	//画像の読み込み
	mTexture.Load("Images/2dAction/hp_gauge.png");
	//スプライトの設定
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(256.0f, 32.0f);
	mSprite.SetPosition(8.0f, -80.0f);

	//手前に表示されるように
	mSprite.SetPriority(100);

	//スプライトのPivotを左端に設定
	//これによりスプライトの左端を原点としてスケーリングするようになる
	mSprite.SetPivot(Pivot::Left);

	//UIレイヤーに描画する
	mSprite.SetRenderLayer("UI");
}

//後片付け
void HpGauge2::Term() {
	//テクスチャの後片付け
	mTexture.Unload();
	//スプライトの後片付け
	mSprite.Term();
}

//更新
void HpGauge2::Update() {
	
}

//描画
void HpGauge2::Render() {
	mSprite.Draw();
}

//体力設定0~1.0で指定
void HpGauge2::SetHp(float hp) {

	if (hp > 1.0f) {
		hp = 1.0f;
	}
	if (hp < 0.0f) {
		hp = 0.0f;
	}

	//スプライトをスケーリング
	mSprite.SetScale(hp, 1.0f);
	//スケーリングにテクスチャ座標の幅もあわせる
	mSprite.SetTexCoord(0.0f, 0.0f, hp, 1.0f);
}