//EnemyMng.cpp

#include"EnemyMng.h"

//SAFE_DELETEを使う為に#include
#include"Lib/LibDef.h"

//生成する敵のクラスの宣言をinclude
#include "Pigeon.h"
#include "Pigeon2.h"
#include "Cat.h"
#include "Cat2.h"

//RenderMangerを使うのでinclude
#include "Fwk/Framework.h"

//画面サイズの定数を使うのでinclude
#include "AppDef.h"

//初期化
void EnemyMng::Init() {
	//アクティブな敵のポインタの配列を空にしておく
	mActiveEnemies.clear();
}

//後片付け
void EnemyMng::Term() {
	mActiveEnemies.clear();
	//全ての敵プールを破棄
	DestroyPoolAll();
}

//更新
void EnemyMng::Update() {
	
	// 画面内に入った敵をスポーン（アクティブ化）
	for (int i = 0; i < mSpawnQueue.size();) {
		const auto& data = mSpawnQueue[i];
		if (IsInView(data.position)) {
			CreateEnemy(data.name, data.position);  // アクティブ化
			mSpawnQueue.erase(mSpawnQueue.begin() + i); // 待機リストから削除
		}
		else {
			++i; // 次のチェックへ
		}
	}

	// アクティブな敵の更新
	for (int i = 0; i < mActiveEnemies.size(); ++i) {
		mActiveEnemies[i]->Update();
	}

	// 非アクティブになった敵を削除
	for (int i = 0; i < mActiveEnemies.size();) {
		if (mActiveEnemies[i]->IsActive()) {
			++i;
		}
		else {
			mActiveEnemies.erase(mActiveEnemies.begin() + i);
		}
	}
}

//描画
void EnemyMng::Render() {
	//アクティブな敵をすべて描画
	for (int i = 0; i < mActiveEnemies.size(); ++i) {
		//描画
		mActiveEnemies[i]->Render();
	}
}

//オブジェクトプールの生成
void EnemyMng::GeneratePool(string enemyName, int poolSize) {

	//鳩のプール生成
	if (enemyName == "Pigeon") {
		//生成する敵クラスにPigeonを追加
		mEnemyPools[enemyName].Init<Pigeon>(poolSize);
	}
	//鳩2のプール生成
	if (enemyName == "Pigeon2") {
		//生成する敵クラスにPigeon2を追加
		mEnemyPools[enemyName].Init<Pigeon2>(poolSize);
	}
	//猫のプール生成
	if (enemyName == "Cat") {
		//生成する敵クラスにCatを追加
		mEnemyPools[enemyName].Init<Cat>(poolSize);
	}
	//猫のプール生成
	if (enemyName == "Cat2") {
		//生成する敵クラスにCatを追加
		mEnemyPools[enemyName].Init<Cat2>(poolSize);
	}
}

//全ての敵のオブジェクトプールの破棄
void EnemyMng::DestroyPoolAll() {

	//鳩のバッファを破棄
	DestroyPool("Pigeon");
	//鳩2のバッファを破棄
	DestroyPool("Pigeon2");
	//猫のバッファを破棄
	DestroyPool("Cat");
	//猫2のバッファを破棄
	DestroyPool("Cat2");
}

// 指定した敵のオブジェクトプールの破棄
void EnemyMng::DestroyPool(string enemyName) {

	//敵のプール破棄
	if (mEnemyPools.count(enemyName) > 0) {
		mEnemyPools[enemyName].Term();
		mEnemyPools.erase(enemyName);
	}
}


//敵の生成
void EnemyMng::CreateEnemy(string enemyName, Vector2f position) {

	//指定の敵プールから非アクティブのオブジェクトを取得する
	Enemy* pEnemy = nullptr;

	//敵プールから空きオブジェクトを取得
	if (mEnemyPools.count(enemyName) > 0) {
		pEnemy = mEnemyPools[enemyName].Alloc();
	}

	//非アクティブの敵オブジェクトがなければ生成失敗
	if (pEnemy == nullptr) {
		return;
	}

	//位置を設定
	pEnemy->SetPosition(position);

	//生成時の関数を呼び戻す
	pEnemy->OnCreated();

	//アクティブな敵配列に追加しておく
	mActiveEnemies.push_back(pEnemy);
}

//残りの敵の数
int EnemyMng::GetEnemyCount() {
	return (int)mActiveEnemies.size();
}

//CSVデータから敵を生成する
void EnemyMng::CreateEnemies(CSVData* pCsvData) 
{

	//nullチェック
	if (pCsvData == nullptr) return;

	int enemyNum = pCsvData->GetElementCount() / 3;		//敵の数を取得

	//取得した数敵を生成する
	for (int i = 0; i < enemyNum; ++i) {
		int index = i * 3;
		std::string name;
		pCsvData->GetString(index, &name);
		Vector2f pos;
		pos.x = pCsvData->GetFloat(index + 1);
		pos.y = pCsvData->GetFloat(index + 2);

		// アクティブにせず、待機リストに追加する
		mSpawnQueue.push_back({ name, pos });
	}
}

//画面内に敵が入ったか判定
bool EnemyMng::IsInView(const Vector2f& pos)
{
	//カメラの座標を取得
	Vector2f cameraPos = RenderManager_I->GetCamera().GetPosition();

	//カメラの移動可能範囲を作る
	float left = cameraPos.x - WINDOW_WIDTH / 2.0f;
	float right = cameraPos.x + WINDOW_WIDTH / 2.0f;
	float top = cameraPos.y + WINDOW_HEIGHT / 2.0f;
	float bottom = cameraPos.y - WINDOW_HEIGHT / 2.0f;

	//カメラの座標を返す
	return (pos.x >= left && pos.x <= right && pos.y <= top && pos.y >= bottom);
}