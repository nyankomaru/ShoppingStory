#include "SceneTitle.h"

#include "Fwk/Framework.h"

#include "GameObjectMng/GameObjectMng.h"

//初期化
void SceneTitle::Init() {
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;

	//カメラを初期化
	{
		Camera camera;
		camera.SetPosition(0.f, 0.f);
		RenderManager_I->SetCamera(camera);
	}

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/Title.png");

	//スプライトの初期化
	mSprite.Init();
	//テクスチャの設定
	mSprite.SetTexture(mTexture);
	//スプライトのサイズ設定
	mSprite.SetSize(1024.f, 768.f);
}

//終了
void SceneTitle::Term() {
	//無し
}

//更新
void SceneTitle::Update() {
	//Aボタンを押したら次のシーンにInGameを設定する
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A)) {
		mNextScene = SceneType::StageSelect;
	}
}

//描画
void SceneTitle::Render() {
	//タイトルを描画する
	mSprite.Draw();
}