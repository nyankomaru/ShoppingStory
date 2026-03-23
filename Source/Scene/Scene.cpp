#include "Scene.h"

#include "Fwk/Framework.h"

//初期化
void Scene::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
	//falseに戻しておく
	mPrintedSummary = false;
}

//終了
void Scene::Term() {
	//無し
}

//更新
void Scene::Update() {
	//なし
}

//描画
void Scene::Render() {
	//なし
}

//次のシーンを取得する
SceneType Scene::GetNextScene() {
	return mNextScene;
}