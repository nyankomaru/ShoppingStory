#include "Slime.h"

//CollisionManagerを使うのでinclude
#include "Fwk/Framework.h"
//Tilemapを使うのでinclude
#include "GameObjectMng/GameObjectMng.h"

//初期化
void Slime::Init()
{
	//敵共通の初期化処理
	Enemy::Init();

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/slime.png");

	//スプライトの初期化
	mSprite.Init();
	//テクスチャの設定
	mSprite.SetTexture(mTexture);
	//スプライトのサイズ設定
	mSprite.SetSize(64.0f, 64.0f);
	//テクスチャ描画範囲を指定
	mSprite.SetTexCoord(0.0f, 0.0f, 0.5f, 0.5f);

	//コリジョンの設定
	{
		//コリジョンにタグを設定
		mCollision.SetTag("Slime");
		//コリジョンの形状を指定
		mCollision.SetRect(0.0f, 0.0f, 40.0f, 40.0f);
	}

	//HPを設定
	mHP = 3;
	//初期状態を設定
	mStatus = Status::Falling;
	//加速度を初期化
	mVelocity = { 0.0f,0.0f };
	//初期の水平移動方向
	mDirection = Direction::Left;
}

//更新
void Slime::Update()
{
	//アクティブでなければ更新処理は行わない
	if (!IsActive()) {
		return;
	}

	//現在の状態で処理を分岐
	switch (mStatus) {
	case Status::Falling:
		_updateFalling();
		break;
	case Status::Moving:
		_updateMoving();
		break;
	}

	//敵共通の処理
	//これは最新の位置をSprite等に反映させるものなので、
	//派生クラスの更新処理の最後に呼ぶ
	Enemy::Update();
}

//落下時の処理
void Slime::_updateFalling() {

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

	//自分の足元の座標
	Vector2f vCheckPos = mPosition + Vector2f(0.0f, -20.0f);
	//タイルマップと衝突判定を行う矩形の幅と高さ
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;

	//足元が壁に衝突していたらY座標を調整して状態を水平移動中に遷移する
	if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight)) {
		//タイルサイズ取得
		const float tileSize = GetMap()->GetTileSize();
		//衝突した足元の座標からタイル行数を計算(マイナス値になるが、座標に戻すのでそのままでよい)
		int hitTileRow = (int)((vCheckPos.y - CollisionHeight) / tileSize);
		//衝突したタイルのY座標(＝そのタイルの上辺の座標)
		float hitTileY = hitTileRow * tileSize;
		//スライムのY座標は衝突したタイルの上辺からコリジョンの高さの半分上がった位置にする
		mPosition.y = hitTileY + 20.0f;
		//状態を移動中にしておく
		mStatus = Status::Moving;
	}
}

//水平移動時の処理
void Slime::_updateMoving() {

	//このフレームでの移動量
	const float speed = 64.0f * Time_I->GetDeltaTime();
	//移動方向を示す単位ベクトル(Right＝(1.0f,0.0f)Left=(-1.0f,0.0f))にする
	Vector2f vDirection = Vector2f((mDirection == Direction::Right) ? 1.0f : -1.0f, 0.0f);
	//移動させる
	mPosition += vDirection * speed;

	//向きを反転するか？
	bool isFlipX = false;

	//壁に衝突するかチェック
	{
		//衝突探知位置(vDirectionが1.0であれば右側、-1.0であれば左側になる)
		Vector2f vCheckPos = mPosition + (vDirection * 32.0f);
		//衝突探知範囲
		float CollisionWidth = 1.0f;
		float CollisionHeight = 40.0f;
		//壁に衝突していたら移動方向反転フラグをON
		if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight)) {
			isFlipX = true;
		}
	}
	//落とし穴があるかチェック
	{
		//衝突探知位置(vDirectionが1.0であれば右側、-1.0であれば左側になる)
		Vector2f vCheckPos = mPosition + (vDirection * 32.0f);
		//足元をチェックするので探知位置も足元になるようY座標を下げる
		vCheckPos.y -= 20.0f;
		//衝突探知範囲
		float CollisionWidth = 1.0f;
		float CollisionHeight = 1.0f;
		//足元が壁でなければ移動方向反転フラグON
		if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight) == false) {
			isFlipX = true;
		}
	}

	//移動方向の反転フラグがONになっていれば
	if (isFlipX) {
		//向きを反転する
		mDirection = (mDirection == Direction::Right) ? Direction::Left : Direction::Right;
	}
}          