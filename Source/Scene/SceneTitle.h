#pragma once

//シーン種別の列挙型をつかうのでinclude
#include "Scene.h"

//タイトルシーンクラス
class SceneTitle:public Scene
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