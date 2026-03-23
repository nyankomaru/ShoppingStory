#pragma once

//敵プールクラス

//可変長配列std::vectorを使う
#include<vector>

#include "Enemy.h"

class EnemyPool
{
public:
	//初期化
	//class Tは、Init関数の呼び出し時に具体的な敵クラスに置き換わる。
	template <class T>
	void Init(int poolSize) {
		//プールの全要素分の敵オブジェクトを生成
		for (int i = 0; i < poolSize; ++i) {
			//指定された敵クラスの変数を一つ生成
			//Enemy*にキャストできるクラスが指定されたのが前提
			Enemy* pNewEnemy = new T();
			//初期化
			pNewEnemy->Init();
			//最初は非アクティブ
			pNewEnemy->SetActive(false);
			//プールにアドレスを記録しておく
			mPool.push_back(pNewEnemy);
		}
	}
	//後片付け
	void Term();
	//オブジェクトを一つ活性化して戻す
	Enemy* Alloc();

protected:
	//Enemy*型の可変長配列
	std::vector<Enemy*> mPool;
};