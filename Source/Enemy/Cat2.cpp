//Cat2.cpp

#include "Cat2.h"
//EnemyMngを使うのでinclude
#include "GameObjectMng/GameObjectMng.h"
//CollisionManagerを使うのでinclude
#include "Fwk/Framework.h"

//初期化
void Cat2::Init() {

	printf("Cat2::Init() 呼ばれました\n");

	//敵共通の初期化処理
	Enemy::Init();

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/Cat2.png");

	//スプライトの初期化
	mSprite.Init();
	//テクスチャの設定
	mSprite.SetTexture(mTexture);
	//スプライトのサイズ設定
	mSprite.SetSize(64.0f, 64.0f);
	//テクスチャ描画範囲を指定
	mSprite.SetTexCoord(0.0f, 0.0f, 1.0f, 1.0f);

	//アニメーションの初期化
	_initAnimation();

	//コリジョンの設定
	{
		//コリジョンにタグを設定
		mCollision.SetTag("Cat2");
		//コリジョンの形状を指定
		mCollision.SetRect(0.0f, -16.0f, 64.f, 32.f);
	}

	//HPを設定
	mHP = 1;
	//初期状態を設定
	mStatus = Status::Waiting;
	//加速度を初期化
	mVelocity = { 0.0f,0.0f };
}

//更新
void Cat2::Update()
{

	//アクティブでなければ更新処理は行わない
	if (!IsActive()) {
		return;
	}

	// 画面外に出たら HP をゼロにする
	if (!EnemyMng::IsInView(mPosition)) {
		OnDamaged(1);
		return;
	}

	//現在の状態で処理を分岐
	switch (mStatus) {
	case Status::Waiting:
		_updateWaiting();
		break;
	case Status::Falling:
		_updateFalling();
		break;
	case Status::Walking:
		_updateWalking();
		break;
	}

	//敵共通の処理
	//これは最新の位置をSprite等に反映させるものなので、
	//派生クラスの更新処理の最後に呼ぶ
	Enemy::Update();
}

//アニメーションの初期化
void Cat2::_initAnimation() {

	//UVの幅と高さ
	float uvW = 1.f / 8.f;
	float uvH = 1.f / 1.f;

	//スプライトセル構造体リスト(8パターン分)
	UVRect uvs[] = {
		{uvW * 0.f,0.f,uvW,uvH}, //左向き　歩き１
		{uvW * 1.f,0.f,uvW,uvH}, //左向き　歩き２
		{uvW * 2.f,0.f,uvW,uvH}, //左向き　ジャンプ準備
		{uvW * 3.f,0.f,uvW,uvH}, //左向き　ジャンプ
		{uvW * 4.f,0.f,uvW,uvH}, //右向き　歩き１
		{uvW * 5.f,0.f,uvW,uvH}, //右向き　歩き２
		{uvW * 6.f,0.f,uvW,uvH}, //右向き　ジャンプ準備
		{uvW * 7.f,0.f,uvW,uvH}, //右向き　ジャンプ
	};

	//猫の歩き時のアニメ

	//右
	UVRect walk_left[] = {
		uvs[0],
		uvs[1],
		{}
	};

	//左
	UVRect walk_right[] = {
		uvs[4],
		uvs[5],
		{}
	};

	//猫の準備時のアニメ
	//右
	UVRect reserv_left[] = { uvs[2] };	//右

	//左
	UVRect reserv_right[] = { uvs[6] };	//左

	//猫のジャンプ時のアニメ
	UVRect jamp_left[] = { uvs[3] };	//右
	UVRect jamp_right[] = { uvs[7] };	//左

	//アニメーションデータ
	Animation anim[6];

	//アニメーションデータ作成：歩き時アニメ
	//右向き
	CreateAnimationUV(anim[0], "walk_right", 3, 0.45f, true, walk_right);
	//左向き
	CreateAnimationUV(anim[1], "walk_left", 3, 0.45f, true, walk_left);

	//アニメーションデータ作成：準備時アニメ
	//右向き
	CreateAnimationUV(anim[2], "reserv_right", 1, 0.f, false, reserv_right);
	//左向き
	CreateAnimationUV(anim[3], "reserv_left", 1, 0.f, false, reserv_left);

	//アニメーションデータ作成：ジャンプ時アニメ
	//右向き
	CreateAnimationUV(anim[4], "jump_right", 1, 0.f, false, jamp_right);
	//左向き
	CreateAnimationUV(anim[5], "jump_left", 1, 0.f, false, jamp_left);

	//アニメーションをスプライトに登録
	for (int i = 0; i < 6; ++i) {
		mSprite.AddAnimation(anim[i]);
	}
}

// 地面判定のための共通関数（新規追加）
bool Cat2::_IsOnGround() {
	Vector2f vCheckPos = mPosition + Vector2f(0.0f, -32.0f);  // 足元
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;
	return GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight);
}

//生成前の処理
void Cat2::_updateWaiting() {
	// 画面内に入ったら「Staying」に切り替える
	if (EnemyMng::IsInView(mPosition)) {
	
		//プレイヤーの左右どちらに生成するかの判別
		Vector2f currentPlayerPos = GetPlayer()->GetPosition();				//プレイヤーの座標を一時保存しておく変数の宣言
		if (currentPlayerPos.x < mPosition.x) {			
			mPosition.x -= 50.f;			//画面端に余裕を持たせて生成する
			mDirection = Direction::Left;
			mSprite.PlayAnimation("walk_left");
		}		
		else {
			mPosition.x += 50.f; // 右側にも余裕を持たせて生成
			mDirection = Direction::Right;
			mSprite.PlayAnimation("walk_right");
		}
		mStatus = Status::Falling;			//ステータスをWalkingに変更		
	}
	// 画面外なら何もしない
}

//落下時の処理
void Cat2::_updateFalling() {

	//重力の倍率
	float gravityScale = 5.0f;
	//このフレームでの重力加速量
	float gravity = -9.8f * Time_I->GetDeltaTime() * gravityScale;
	//このフレームでの最大重力
	float maxGravity = -1200.0f * Time_I->GetDeltaTime();

	//重力の計算
	mVelocity.y += gravity;
	if (mVelocity.y < maxGravity) {
		mVelocity.y = maxGravity;
	}

	//位置の更新
	mPosition += mVelocity;

	_CheckRound();	//地面と接しているか確認
}

void Cat2::_updateWalking()
{
	//このフレームでの移動量
	float speed = 40.f * Time_I->GetDeltaTime();
	Vector2f vDirection = Vector2f((mDirection == Direction::Right) ? 1.0f : -1.0f, 0.0f);

	// 距離を先に測る（←これが重要！）
	_SetDistanceBetweenTwo();

	// 近ければ速度三倍
	if (_GetDistanceBetweenTwo() <= 200.f) {
		speed *= 4.f;
	}

	// アニメーション再生
	const string animName_walk[] = { "","walk_right","","walk_left" };
	mSprite.PlayAnimation(animName_walk[(int)mDirection], AnimationPlayStyle::Continue);

	// 移動
	mPosition += vDirection * speed;

	// 地面チェックを追加
	if (!_IsOnGround()) {
		mStatus = Status::Falling;
	}
}

//プレイヤーと猫の二点間の距離を返す処理
float Cat2::_GetDistanceBetweenTwo() {
	return mDistanceX;
}

//プレイヤーと猫の二点間の距離を測る処理
void Cat2::_SetDistanceBetweenTwo() {

	//二点の計算をするための準備をする
	Vector2f currentPlayerPos = GetPlayer()->GetPosition();		//プレイヤーの座標を一時保存しておく変数の宣言

	//プレイヤーと猫の二点の距離を測る
	mDistanceX = abs(mPosition.x - currentPlayerPos.x);			//X座標の距離を測る		
}

//地面と接しているか確認
void Cat2::_CheckRound() {
	Vector2f vCheckPos = mPosition + Vector2f(0.0f, -32.0f);  // 足元の位置
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;

	if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight)) {
		const float tileSize = GetMap()->GetTileSize();
		int hitTileRow = (int)((vCheckPos.y - CollisionHeight) / tileSize);
		float hitTileY = hitTileRow * tileSize;

		// Y位置調整（スプライトの底に合わせる）
		mPosition.y = hitTileY + 32.0f;

		// 状態に応じて復帰
		if (mStatus == Status::Falling) {
			mStatus = Status::Walking;
		}
	}
}