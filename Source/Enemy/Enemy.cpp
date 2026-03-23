//Enemy.cpp

#include "Enemy.h"

//コリジョングループの列挙型を使うために
#include "GameDef.h"
//CollisionManagerを使うのでinclude
#include "Fwk/Framework.h"
#include "EnemyMng.h"
#include "GameObjectMng/GameObjectMng.h"

//初期化
void Enemy::Init()
{
	//状態をアクティブに
	mIsActive = false;

	//被ダメージ音を読み込む
	mSound.Load("Sound/damageE.wav");
	//音源の初期化
	mSoundSource.Init(mSound, 1);

	//コリジョンの設定(敵の共通項目だけ)
	{
		//自分は敵グループのコリジョン
		mCollision.SetGroup((int)CollisionGroup::Enemy);
		//衝突対象にプレイヤーグループのコリジョンを追加
		mCollision.AddHitGroup((int)CollisionGroup::Player);
		//コリジョンのオーナーに自分を設定
		mCollision.SetOwner(this);
		//コリジョンを活性状態にしておく
		mCollision.SetActive(true);
		//コリジョンマネージャーにコリジョンを登録
		CollisionManager_I->Register(&mCollision);
	}

	//ダメージ時に付ける赤色
	mDamagedColor = 0.0f;
}

//後片付け
void Enemy::Term()
{
	//コリジョンマネージャーから登録解除
	CollisionManager_I->Unregister(&mCollision);

	mSprite.Term();
	mTexture.Unload();

	//サウンドの破棄
	mSound.Unload();
	//音源の解放
	mSoundSource.Term();
}

//更新
void Enemy::Update()
{
	 // アクティブでない場合、描画処理を行わない
    if (!IsActive() || !EnemyMng::IsInView(mPosition)) {
        return;  // 画面外なら更新せず、描画もしない
    }

    // スプライトを更新(アニメーション)
    mSprite.Update();
    // スプライトの位置を更新
    mSprite.SetPosition(mPosition);
    // コリジョンの位置を更新
    mCollision.SetPosition(mPosition);

    // ダメージ時に加算する色(赤)の強さが０でなければ
    if (mDamagedColor > 0.0f) {
        mDamagedColor -= Time_I->GetDeltaTime() * 4.0f;
        if (mDamagedColor < 0.0f) {
            mDamagedColor = 0.0f;
        }
        mSprite.SetAdditionalColor(mDamagedColor, 0.0f, 0.0f);
    }
}

//描画
void Enemy::Render()
{
	//アクティブでなければ描画処理は行わない
	if (!IsActive()) {
		return;
	}

	mSprite.Draw();
}

//アクティブか？
bool Enemy::IsActive() {
	return mIsActive;
}

//アクティブ状態の設定
void Enemy::SetActive(bool isActive) {
	mIsActive = isActive;
	//コリジョンも合わせておく
	mCollision.SetActive(isActive);
}

//現在位置を取得
Vector2f Enemy::GetPosition() {
	return mPosition;
}

//現在位置を設定
void Enemy::SetPosition(Vector2f position) {
	mPosition = position;
	mSprite.SetPosition(mPosition);
	//コリジョンの位置も更新
	mCollision.SetPosition(mPosition);
}

//ダメージを受ける
void Enemy::OnDamaged(int damage) {
	mHP -= damage;
	if (mHP <= 0) {
		SetActive(false);
		//やられた時の関数を呼び出す
		//(派生クラスでOnDefeated上書きしていた場合にその関数が呼ばれる)
		OnDefeated();
	}
	//ダメージ時に加算する赤色の強さを最大値(1.0)に設定
	mDamagedColor = 1.0f;
	mSprite.SetAdditionalColor(mDamagedColor, 0.0f, 0.0f);
}

//生成された時に呼び戻す
//派生クラスで上書きして使う
void Enemy::OnCreated() {
	SetActive(true); // ← これがないと、描画もUpdateもスルーされる！
}

//やられたときに呼び出される
//派生クラスで上書きして使う
void Enemy::OnDefeated() {
	//なし
}