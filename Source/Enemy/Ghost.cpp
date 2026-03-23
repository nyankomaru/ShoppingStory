#include "Ghost.h"

//Tile_Iを使うのでinclude
#include "Fwk/Framework.h"

//度→ラジアン変換のマクロDegToRedが定義されている
#include "Lib/Math/Math.h"

//初期化
void Ghost::Init()
{
	//敵共通の初期化処理
	Enemy::Init();

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/ghost.png");

	//スプライトの初期化
	mSprite.Init();
	//テクスチャの設定
	mSprite.SetTexture(mTexture);
	//スプライトのサイズ設定
	mSprite.SetSize(160.0f, 160.0f);

	//コリジョンの設定
	{
		//コリジョンにタグを設定
		mCollision.SetTag("Ghost");
		//コリジョンの形状を指定
		mCollision.SetCircle(0.0f, 0.0f, 40.0f);
	}

	//HPを設定
	mHP = 6;

	//水平方向の秒間移動スピード
	mSpeed = 64.0f;
	//垂直方向の最大変位(振幅)
	mAmp = 120.0f;
	//周期
	mPeriodTime = 3.0f;
	//何周期で進行方向を反転するか
	mTurnPeriod = 3;
	//現在の向き
	mDirection = Direction::Left;
	//タイマー初期化
	mTimer = 0.0f;
}

//更新
void Ghost::Update()
{
	//アクティブでなければ更新処理は行わない
	if (!IsActive()) {
		return;
	}
	
	//経過時間を更新
	mTimer += Time_I->GetDeltaTime();

	Vector2f vDirection = Vector2f((mDirection == Direction::Right) ? 1.0f : -1.0f, 0.0f);

	//反転する位置(端)に到達する時間
	float turnTime = mPeriodTime * mTurnPeriod;
	//反転する時間を超えていたら
	if (mTimer >= turnTime) {
		//原点を折り返し地点に移す
		mOrigin = mOrigin + vDirection * (mSpeed * turnTime);
		//現在時刻から反転までの時間を減らしておく
		mTimer -= turnTime;
		//向きを反転
		mDirection = (mDirection == Direction::Right) ? Direction::Left : Direction::Right;
		//向きベクトルも反転
		vDirection *= -1.0f;
		//向きが右に向いたら絵を左右反転する
		mSprite.SetFlipX(mDirection == Direction::Right);
	}

	//水平方向の移動。現在位置＝原点＋現在時刻の水平方向移動量とする
	mPosition = mOrigin + (vDirection * mSpeed * mTimer);

	//現在の角度θを求める　360度に(現在時刻/周期Ｔ)をかければ現在時刻の角度になる
	float  theta = 360.0f * (mTimer / mPeriodTime);

	//現在位置のｙ成分に垂直方向の変化量(sinθ×mAmp)を加算
	mPosition.y += sinf(DegToRad(theta)) * mAmp;

	//敵共通
	//これは最新の位置をSprite等に反映させるものなので、
	//派生クラスの更新処理の最後に呼ぶ
	Enemy::Update();
}

//生成された時に呼び戻される
void Ghost::OnCreated()
{
	//開始時の座標を原点として記録しておく
	mOrigin = mPosition;
}