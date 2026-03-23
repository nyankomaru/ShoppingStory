#pragma once

//シーン種別の列挙型をつかうのでinclude
#include "Scene.h"

//ステージ選択シーンクラス
class SceneStageSelect:public Scene
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

private:
	//現在選択中のステージ番号
	int mSelectedIndex;
};