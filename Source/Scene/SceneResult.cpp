#include "SceneResult.h"

#include "Fwk/Framework.h"

#include"GameObjectMng/GameObjectMng.h"

//初期化
void SceneResult::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;

	//カメラを初期化
	{
		Camera camera;
		camera.SetPosition(0.f,0.f);
		RenderManager_I->SetCamera(camera);
	}
}

//終了
void SceneResult::Term() {
	//なし
}

//更新
void SceneResult::Update() {
	//Aボタンを押したら次のシーンにTitleを設定する
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A)) {
		mNextScene = SceneType::Title;
	}
}

//描画
void SceneResult::Render() {
	GetProduct()->PrintStolenSummary();
}