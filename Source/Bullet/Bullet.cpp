#include "Bullet.h"

//弾の初期化
void Bullet::Init() {

	//位置の初期化
	mPosition.x = 0.0f;
	mPosition.y = 0.0f;

	//弾のテクスチャの読み込み
	mTexture.Load("Images/Shooting/PlayerBullet.png");
	//弾のスプライトの初期化
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(32.0f, 64.0f);
	mSprite.SetPosition(mPosition.x, mPosition.y);
	
	//初期状態は非アクティブにしておく
	mIsActive = false;
}

//弾の更新
void Bullet::Update() {

	//非アクティブなら処理しない
	if (!mIsActive) {
		return;
	}

	//弾を画面の上方に移動させる
	mPosition.y += 10.0f;

	//画面外に出たら非アクティブにする
	if (mPosition.y > 400.0f) {
		mIsActive = false;
		return;
	}
	//スプライトの位置を更新
	mSprite.SetPosition(mPosition.x, mPosition.y);
}

//弾の描画
void Bullet::Render() {

	//非アクティブなら処理しない
	if (!mIsActive) {
		return;
	}
	//スプライトの描画
	mSprite.Draw();
}

//弾の後片付け
void Bullet::Term() {
	mSprite.Term();
	mTexture.Unload();
}

//位置の設定
void Bullet::SetPosition(Position pos) {
	mPosition = pos;
}

//アクティブ状態の取得/設定
bool Bullet::IsActive() {
	return mIsActive;
}
void Bullet::SetActive(bool isActive) {
	mIsActive = isActive;
}