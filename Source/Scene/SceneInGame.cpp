#include "SceneInGame.h"

#include "Fwk/Framework.h"

#include "GameObjectMng/GameObjectMng.h"

//ゲームプレイデータ
#include "GamePlayData/GamePlayData.h"
//ステージ情報マスタ
#include "Mst/StageDataMst.h"

//初期化
void SceneInGame::Init() {

	//遊ぶステージの番号を取得
	int stageNo = GetGamePlayData().GetStageNo();
	//遊ぶステージの情報を取得
	StageData* pStageData = GetStageDataMst().Get(stageNo);

	//ゲームオブジェクトの初期化
	GameObjectInit(pStageData);

	//BGM読み込み用
	mSound.Load("Sound/bgmA.mp3");
	//音源の初期化
	mSoundSource.Init(mSound);
	//BGMを繰り返し再生
	mSoundSource.Play(-1);
	
	//次のシーンの初期値をなしにしておく
	mNextScene = SceneType::None;
}

//終了
void SceneInGame::Term() {
	//ゲームオブジェクトの解放
	GameObjectTerm();

	//BGMを停止
	mSoundSource.Stop();
	//音源を解放
	mSoundSource.Term();
	//読み込んだ音声ファイルを破棄
	mSound.Unload();
}

//更新
void SceneInGame::Update() {
	//ゲームオブジェクトの更新
	GameObjectUpdate();

	//プレイヤーを取得する
	Player* player = GetPlayer();
	if (player) {
		//プレイヤーがゴールに到達していて、ゲームパッドのAボタンが押されていたら
		if (player->IsReacheGoal()
			&& (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A))
			) {
			//遊んだステージの番号を取得
			int stageNo = GetGamePlayData().GetStageNo();
			//遊んだステージのクリアフラグを立っておく
			GetGamePlayData().SetStageState(stageNo, true);
			//次のシーンをゲームクリアシーンにする
			mNextScene = SceneType::GameClear;
		}

		//プレイヤーがやられ状態で、ゲームパッドのAボタンが押されていたら
		if (player->IsDead()
			&& (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A))
			) {
			//次のシーンをゲームオーバーシーンにする
			mNextScene = SceneType::GameOver;
		}
	}
}

//描画
void SceneInGame::Render() {

	//ゲームオブジェクトの描画
	GameObjectRender();

	//プレイヤーを取得
	Player* player = GetPlayer();

	if (player) {
		//ゴールに到達していた時の表示
		if (player->IsReacheGoal()) {
			PrintText("クリア！", 450.0f, 250.0f);
		}
		//やられた時の表示
		if (player->IsDead()) {
			PrintText("やられた・・・", 450.0f, 250.0f);
		}
	}
}