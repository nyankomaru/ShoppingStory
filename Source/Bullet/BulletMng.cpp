#include "BulletMng.h"

BulletMng bulletMng;

//BulletMngの参照を取得する関数の定義
BulletMng& GetBulletMng() {
	return bulletMng;
}

//弾マネージャの初期化
void BulletMng::Init() {
	for (int i = 0; i < BulletBufferSize; ++i) {
		mBullets[i].Init();
	}
}

//弾マネージャの更新
void BulletMng::Update() {
	for (int i = 0; i < BulletBufferSize; ++i) {
		mBullets[i].Update();
	}
}

//弾マネージャの描画
void BulletMng::Render() {
	for (int i = 0; i < BulletBufferSize; ++i) {
		mBullets[i].Render();
	}
}

//弾マネージャの後片付け
void BulletMng::Term() {
	for (int i = 0; i < BulletBufferSize; ++i) {
		mBullets[i].Term();
	}
}

//指定位置に弾を生成
void BulletMng::GenerateBullet(Position pos)
{
	//空きバッファを探す
	int index = -1;

	for (int i = 0; i < BulletBufferSize; ++i) {
		if (mBullets[i].IsActive() == false) {
			index = i;
			break;
		}
	}

	//空きがなければ何もしないで戻る
	if (index < 0) {
		return;
	}

	//活性状態に設定
	mBullets[index].SetActive(true);

	//位置設定
	mBullets[index].SetPosition(pos);
}