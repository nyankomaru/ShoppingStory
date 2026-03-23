//GameObjectMng.cpp

#include "GameObjectMng.h"

//プレイヤー
#include "Player/Player.h"
//敵マネージャー
#include "Enemy/EnemyMng.h"
//商品
#include "Product/Product.h"

//Key-Valudeファイルの読み込み
#include "Fwk/File/KeyValueFile.h"

//RenderMangerを使うのでinclude
#include "Fwk/Framework.h"

//画面サイズの定数を使うのでinclude
#include "AppDef.h"

//CSVファイルの読み込み
#include"Fwk/File/CSVFile.h"

//UIMngを使うのでinclude
#include "UI/UIMng.h"

//ゴールをつかうのでinclude
#include "Goal/Goal.h"
#include "Start/Start.h"

#include "BG/BG.h"

#include <iostream>

BG mBg;
Tilemap mMap;
Player mPlayer;
EnemyMng mEnemyMng;
Enemy mEnemy;
UIMng mUIMng;
Goal mGoal;
Start mStart;
Product mProduct;

//全オブジェクト初期化
void GameObjectInit(StageData* pStageData)
{
	mBg.Init();
	mMap.Init();
	mProduct.Init();
	mPlayer.Init();

	//敵マネージャーの初期化
	mEnemyMng.Init();
	//鳩のオブジェクトプールを確保
	mEnemyMng.GeneratePool("Pigeon", 10);
	//鳩2のオブジェクトプールを確保
	mEnemyMng.GeneratePool("Pigeon2", 10);
	//猫のオブジェクトプールを確保
	mEnemyMng.GeneratePool("Cat", 10);
	//猫2のオブジェクトプールを確保
	mEnemyMng.GeneratePool("Cat2", 10);

	//敵の配置情報をファイルから取得する
	if(pStageData)
	{
		CSVFile csvFile;
		csvFile.Load(pStageData->ArrangementFile);
		//CSVデータから敵を配置する
		mEnemyMng.CreateEnemies(csvFile.GetCSVData());
	}

	//マップデータをファイルから取得する
	if(pStageData){
		KeyValueFile mapdataFile;
		mapdataFile.Load(pStageData->MapFile);
		//マップの列数を取得
		int col = mapdataFile.GetInt("Col");
		//マップの行数を取得
		int row = mapdataFile.GetInt("Row");
		//CSVDataからマップを生成する
		mMap.CreateMap(col, row, mapdataFile.GetCSVData("Data"));

		//ゴールを配置
		mGoal.Init();
		CSVData* pCSV = mapdataFile.GetCSVData("Goal");
		if (pCSV != nullptr) {
			float x = pCSV->GetFloat(0);
			float y = pCSV->GetFloat(1);
			mGoal.SetPosition(Vector2f(x, y));
		}
		//スタートを配置
		mStart.Init();
		CSVData* pSCSV = mapdataFile.GetCSVData("Start");
		if (pCSV != nullptr) {
			float x = pSCSV->GetFloat(0);
			float y = pSCSV->GetFloat(1);
			mStart.SetPosition(Vector2f(x, y));
		}
		//プレイヤーの位置を設定
		CSVData* pStartPos = mapdataFile.GetCSVData("Start");
		if (pStartPos != nullptr) {
			float x = pStartPos->GetFloat(0);
			float y = pStartPos->GetFloat(1);
			mPlayer.SetPosition(Vector2f(x, y));
		}
	}

	//UIマネージャーの初期化
	mUIMng.Init();
}

//全オブジェクト更新
void GameObjectUpdate()
{
	//最初にプレイヤーの更新
	mPlayer.Update();

	//プレイヤーがやられていなければカメラを追従させる
	if(mPlayer.IsAlive())
	{
	//プレイヤーの現在位置にカメラの位置を合わせる
		{
			//プレイヤーの現在位置にカメラの位置を合わせる
			Vector2f cameraPosition = mPlayer.GetPosition();
			//マップ全体のサイズを取得
			Vector2f mapSize = mMap.GetMapSize();

			//カメラの移動可能範囲を作る
			float cameraArea_left = WINDOW_WIDTH / 2.0f;					//左端
			float cameraArea_top = -WINDOW_HEIGHT / 2.0f;					//上端
			float cameraArea_right = mapSize.x - WINDOW_WIDTH / 2.0f;		//右端
			float cameraArea_bottom = -mapSize.y + WINDOW_HEIGHT / 2.0f;	//下端

			//カメラ位置がカメラの移動範囲内に収まるように調整
			if (cameraPosition.x < cameraArea_left) {
				cameraPosition.x = cameraArea_left;
			}
			if (cameraPosition.x > cameraArea_right) {
				cameraPosition.x = cameraArea_right;
			}
			if (cameraPosition.y > cameraArea_top) {
				cameraPosition.y = cameraArea_top;
			}
			if (cameraPosition.y < cameraArea_bottom) {
				cameraPosition.y = cameraArea_bottom;
			}

			//カメラの位置を更新
			Camera camera;
			camera.SetPosition(cameraPosition);
			RenderManager_I->SetCamera(camera);
		}
	}

	mBg.Update();
	//マップの更新
	mMap.Update();
	//敵マネージャーの更新
	mEnemyMng.Update();
	//プレイヤーの体力をUIマネージャーに渡す
	mUIMng.SetHp(mPlayer.GetHp());
	mUIMng.SetHp(mPlayer.GetNormalizedHp());
	//UIマネージャーの更新
	mUIMng.Update();
	//ゴール地点を更新
	mGoal.Update();
	mStart.Update();
}

//全オブジェクト描画
void GameObjectRender()
{
	mBg.Render();
	mMap.Render();
	//ゴール地点を描画
	mGoal.Render();
	mStart.Render();
	mPlayer.Render();
	mEnemyMng.Render();
	//UIマネージャーの描画
	/*mUIMng.Render();*/
	
}

//全オブジェクト解放
void GameObjectTerm()
{
	mBg.Term();
	mMap.Term();
	mPlayer.Term();
	mEnemyMng.Term();
	//UIマネージャーの解放
	mUIMng.Term();
	//ゴール地点を描画
	mGoal.Term();
	mStart.Term();
}

//Tilemapクラスのアドレスを返す
Tilemap* GetMap() {
	return &mMap;
}

//プレイヤークラスのポインタをかえす
Player* GetPlayer() {
	return &mPlayer;
}

//敵マネージャークラスのポインタを返す
EnemyMng* GetEnemyMng() {
	return &mEnemyMng;
}

// 商品マネージャーのポインタを返す
Product* GetProduct() {
	return &mProduct;
}