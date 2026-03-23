#include "EnemyPool.h"

//SAFE_DELETEを使いたいので#include
#include"Fwk/Framework.h"

//後片付け
void EnemyPool::Term() {
	//Enemy*配列の各要素にnewされた敵のオブジェクトのアドレスが
	//記録されているので一つ一つDELETEしていく
	for (int i = 0; i < mPool.size(); ++i) {
		SAFE_DELETE(mPool[i]);
	}
	//プールを破棄
	mPool.clear();
}

//オブジェクトを一つ活性化して戻す
Enemy* EnemyPool::Alloc() {

	//戻り値を記録する変数
	Enemy* pResult = nullptr;

	for (int i = 0; i < mPool.size(); ++i) {
		//非アクティブのオブジェクトを見つけたら活性化して戻り値にする
		if (mPool[i]->IsActive() == false) {
			//戻り値にする
			pResult = mPool[i];
			//活性化しておく
			pResult->SetActive(false);
			//ループは抜けてよい
			break;
		}
	}
	return pResult;
}