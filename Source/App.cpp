#include "App.h"

#include "Scene/SceneMng.h"
#include "GamePlayData/GamePlayData.h"
#include "Mst/StageDataMst.h"

SceneMng sceneMng;

//---------------------------------------------------------
//ゲームの初期化処理を行う関数
//---------------------------------------------------------
void App::Init() {

	//UI用の描画レイヤーを追加する
	RenderManager_I->AddLayer("UI");

	//画面の左上がゲーム空間の原点(0,0)となるようにカメラを移動させる
	{
		Camera camera;
		camera.SetPosition(WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f);
		RenderManager_I->SetCamera(camera);
	}

	//Uiレイヤー用のカメラの追加・設定
	{
		Camera camera;
		//カメラ名を指定する
		camera.SetName("UI");
		//カメラをセットする
		RenderManager_I->SetCamera(camera);
		//UIカメラをアクティブにする
		RenderManager_I->SetCameraActive("UI", true);
		//UIカメラの位置を設定
		RenderManager_I->SetCameraPosition("UI", Vector2f(WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f));
		//UIカメラの描画対象からUIレイヤーを取り除く
		RenderManager_I->AddCameraRenderLayer("UI", "UI");
	}
	//ゲームプレイデータを初期化する
	GetGamePlayData().Init();
	//ステージ情報マスクを初期化する
	GetStageDataMst().Init();

	//シーン管理クラスを初期化し、最初のシーンはタイトルシーン
	sceneMng.Init(SceneType::Title);
}

//---------------------------------------------------------
//ゲームの更新処理を行う関数
//---------------------------------------------------------
void App::Update() {
	//シーン更新
	sceneMng.Update();
}

//---------------------------------------------------------
//ゲームの描画処理を行う関数
//---------------------------------------------------------
void App::Render() {
	//シーン描画
	sceneMng.Render();
}
//---------------------------------------------------------
// ゲームの後片付け処理を行う関数
//---------------------------------------------------------
void App::Term() {
	//シーン解放
	sceneMng.Term();
}
