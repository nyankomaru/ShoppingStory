#include "HpGauge.h"

//ハートマークの大きさ
const float HeartSize = 40.0f;
//一番左のハートマークの中心点のX座標
const float Heart_PosX = 32.0f;
//一番左のハートマークの中心点のY座標
const float Heart_PosY = -32.0f;
//ハートマークの感覚
const float Heart_Interval = HeartSize + 8.0f;

//初期化
void HpGauge::Init()
{
	mTexture.Load("Images/2dAction/heart.png");

	for (int i = 0; i < HEART_NUM; ++i)
	{
		mSprite[i].Init();
		mSprite[i].SetTexture(mTexture);
		mSprite[i].SetSize(HeartSize, HeartSize);
		mSprite[i].SetPosition(Heart_PosX + (Heart_Interval * i), Heart_PosY);
		mSprite[i].SetVisible(false);
		mSprite[i].SetPriority(100);

		//UIレイヤーに描画する
		mSprite[i].SetRenderLayer("UI");
	}
}

//後片付け
void HpGauge::Term()
{
	for (int i = 0; i < HEART_NUM; ++i) {
		mSprite[i].Term();
	}
	mTexture.Unload();
}

//更新
void HpGauge::Update()
{
	for (int i = 0; i < HEART_NUM; ++i) {
		mSprite[i].Update();
	}
}

//描画
void HpGauge::Render()
{
	for (int i = 0; i < HEART_NUM; ++i) {
		mSprite[i].Draw();
	}
}

//HPの設定
void HpGauge::SetHp(int hp)
{
	for (int i = 0; i < HEART_NUM; ++i) {
		mSprite[i].SetVisible(i < hp);
	}
}