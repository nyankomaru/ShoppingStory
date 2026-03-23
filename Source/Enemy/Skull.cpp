#include "Skull.h"

//CollisionManagerを使うのでinclude
#include "Fwk/Framework.h"
//Tilemapを使うのでinclude
#include "GameObjectMng/GameObjectMng.h"

//初期化
void Skull::Init()
{
	//敵共通の初期化処理
	Enemy::Init();

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/skull.png");

	//スプライトの初期化
	mSprite.Init();
	//テクスチャの設定
	mSprite.SetTexture(mTexture);
	//スプライトのサイズ設定
	mSprite.SetSize(64.0f, 64.0f);

	//コリジョンの設定
	{
		//コリジョンにタグを設定
		mCollision.SetTag("Skull");
		//コリジョンの形状を指定
		mCollision.SetCircle(0.0f, 0.0f, 20.0f);
	}

	//アニメーションを初期化
	_initAnimation();

	//HPを設定
	mHP = 3;
	//タイマー初期化
	mTimer = 0.0f;
	//向き
	mDirection = Direction::Left;
	//初期状態
	mStatus = Status::Moving;
}

//更新
void Skull::Update()
{
	//アクティブでなければ更新処理は行わない
	if (!IsActive()) {
		return;
	}

	//現在の状態で処理を分岐
	switch (mStatus) {
	case Status::Moving:
		_updateMoving();
		break;
	case Status::Idle:
		_updateIdle();
		break;
	}

	//敵共通の処理
	//これは最新の位置をSprite等に反映させるものなので、
	//派生クラスの更新処理の最後に呼ぶ
	Enemy::Update();
}

//落下時の処理
void Skull::_updateMoving() {

	//加速度の更新
	_updateVelocity();

	//位置の更新
	mPosition += mVelocity;

	//設置確認用の矩形の中心点を自分の足元に設定
	Vector2f vCheckPos = mPosition + Vector2f(0.0f, -20.0f);
	//衝突判定用の矩形の幅と高さ
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;
	//地面についているか確認する
	bool isGround = GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight);

	//着地していた場合
	if (isGround) {

		//Y座標を調整、がいこつの衝突範囲の下辺とタイルの上辺が重なるようにする
		{
			//タイルサイズ取得
			const float tileSize = GetMap()->GetTileSize();
			//衝突したタイルの行数を計算
			int hitTileRow = (int)((vCheckPos.y - CollisionHeight) / tileSize);
			//衝突したタイルの上辺のＹ座標
			float hitTileY = hitTileRow * tileSize;
			//Ｙ座標を調整
			//衝突したタイルの上辺のＹ座標＋衝突矩形の高さの半分
			mPosition.y = hitTileY + 20.0f;
		}

		//経過時間計測用のタイマーをリセット
		mTimer = 0.0f;
		//アイドルアニメを再生
		mSprite.PlayAnimation("idle");
		//待機状態に遷移
		mStatus = Status::Idle;
	}
}

//待ち時間の更新処理
void Skull::_updateIdle() {

	//プレイヤーの位置
	Vector2f playerPos = GetPlayer()->GetPosition();

	//向きの更新
	mDirection = (playerPos.x < mPosition.x) ? Direction::Left : Direction::Right;
	mSprite.SetFlipX(mDirection == Direction::Right);

	//待機時間(1.0f秒)経過しているか確認
	if (mTimer < 1.0f) {
		//経過時間の計測
		mTimer += Time_I->GetDeltaTime();
	}
	else {
		//一秒経過していた場合

		//プレイヤーとの距離が一定以内であれば
		if ((playerPos - mPosition).GetLength() <= 160.0f) {
			//ジャンプする(上向きの加速を設定する)
			mVelocity.y = 16.0f;
			//ジャンプアニメを再生
			mSprite.PlayAnimation("jump");
			//移動状態に遷移
			mStatus = Status::Moving;
		}
	}
}          

//加速度の更新
void Skull::_updateVelocity() {
	//重力の倍率
	float grabityScale = 5.0f;
	//このフレームでの重力加速量
	float gravity = -9.8f * Time_I->GetDeltaTime() * grabityScale;
	//このフレームでの最大重力
	float maxGravity = -1200.0f * Time_I->GetDeltaTime();

	//重力の計算
	mVelocity.y += gravity;
	if (mVelocity.y < maxGravity) {
		mVelocity.y = maxGravity;
	}
}

//アニメーションの初期化
void Skull::_initAnimation() {

	//ジャンプ時のアニメを追加
	{
		//0度→360度の回転アニメーション
		float rotDeg[] = {
			0.0f,
			360.0f,
		};
		Animation animJumping;
		//回転アニメを生成。フレーム数は２。0.75秒で一回転、ループはON
		CreateAnimationRotation(animJumping, "jump", 2, 0.75, true, rotDeg);
		//Spriteにやられた時にアニメを追加
		mSprite.AddAnimation(animJumping);
	}

	//アイドル時のアニメを追加
	{
		//回転を０度に戻すだけのアニメーション
		float rotDeg[] = {
			0.0f
		};
		Animation animIdle;
		//回転アニメを生成。フレーム数は１。０秒。ループなし
		CreateAnimationRotation(animIdle, "idle", 1, 0.0, false, rotDeg);
		//Spriteにやられた時にアニメを追加
		mSprite.AddAnimation(animIdle);
	}
}

//やられたときに呼び出される
void Skull::OnDefeated() {
	//やられたら鬼火を作りだす
	Vector2f pos = mPosition;
	pos.y += 64.0f;
	GetEnemyMng()->CreateEnemy("Onibi", pos);
}