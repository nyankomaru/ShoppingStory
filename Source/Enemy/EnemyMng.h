//EnemyMng.h

#pragma once

#include <string>
#include <vector>		//可変長配列　std::vector　を使う
#include <map>
using namespace std;

#include"Lib/Math/Vector.h"
using namespace Lib::Math;

//CSVデータを使うのでCSVFile.hを取り込む
#include "Fwk/File/CSVFile.h"

//Enemyクラスの宣言をinclude
#include "Enemy.h"

//敵プールクラスをつかうので#include
#include "EnemyPool.h"

//スポーン待機用
struct EnemySpawnData {
	std::string name;
	Vector2f position;
};

//敵管理クラス
class EnemyMng
{
public:

	//初期化
	void Init();
	//後片付け
	void Term();

	//更新
	void Update();
	//描画
	void Render();

	//オブジェクトプールの生成
	void GeneratePool(string enemyName, int poolSize);
	//指定した敵のオブジェクトプールの解放
	void DestroyPool(string enemyName);
	//全ての敵のオブジェクトプールの解放
	void DestroyPoolAll();

	//敵の生成
	void CreateEnemy(string enemyName, Vector2f position);

	//残りの敵の数
	int GetEnemyCount();

	//CSVデータを敵を生成する
	void CreateEnemies(CSVData* pCsvData);

	//画面内に敵が入ったか判定
	static bool IsInView(const Vector2f& pos);

private:
	//アクティブな敵オブジェクトのポインタ配列
	vector<Enemy*>mActiveEnemies;
	//敵種別ごとのオブジェクトプール
	map<string, EnemyPool>mEnemyPools;
	//未アクティブ敵の待機リスト
	std::vector<EnemySpawnData> mSpawnQueue;
};