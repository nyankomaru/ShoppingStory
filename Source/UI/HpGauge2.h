#pragma once

#include "Fwk/Graphics.h"

class HpGauge2
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
	
	//体力設定(0~1.0の範囲で指定)
	void SetHp(float hp);

private:

	Texture mTexture;
	Sprite mSprite;
};