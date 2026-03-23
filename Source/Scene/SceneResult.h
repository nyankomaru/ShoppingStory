#pragma once

//シーンクラスの継承するのでinclude
#include "Scene.h"

//ゲームオーバーシーンクラス
class SceneResult :public Scene
{
public:
	//初期化
	void Init();
	//終了
	void Term();
	//更新
	void Update();
	//描画
	void Render();
};