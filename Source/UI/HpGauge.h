#pragma once

//Sprite.Textureを使うのでinclude
#include "Fwk/Graphics.h"

//UI 体力ゲージクラス
class HpGauge
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
	//HPの設定
	void SetHp(int hp);

private:

	//ハートマークの最大数
	enum {
		HEART_NUM = 5
	};

	Texture mTexture;
	Sprite mSprite[HEART_NUM];
};