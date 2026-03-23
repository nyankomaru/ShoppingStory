//Pigeon2.cpp

#include "Pigeon2.h"

//CollisionManagerを使うのでinclude
#include "Fwk/Framework.h"
//EnemyMngを使うのでinclude
#include "GameObjectMng/GameObjectMng.h"
//画面サイズの定数を使うのでinclude
#include "AppDef.h"
//度→ラジアン変換のマクロDegToRedが定義されている
#include "Lib/Math/Math.h"

//初期化
void Pigeon2::Init()
{

	//敵共通の初期化処理
	Enemy::Init();

	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/Pigeon2.png");

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
		mCollision.SetTag("Pigeon2");
		//コリジョンの形状を指定
		mCollision.SetRect(0.0f, 0.0f, 64.f, 64.f);
	}

	//HPを設定
	mHP = 1;
	//初期状態を設定
	mStatus = Status::Waiting;
	//加速度を初期化
	mVelocity = { 0.0f,0.0f };
	//初期の水平移動方向
	mDirection = Direction::Left;
	// Y座標を記録する（画面内に入る前でもOK）
	mInitialY = mPosition.y;
	// 初期化時は未計測
	mHasMeasuredDistance = false;

	// 飛行用パラメータを初期化
	mFlyingTimer = 0.0f;
	mFlyingDuration = 2.0f; // 飛行にかかる時間（秒）
}

//更新
void Pigeon2::Update()
{

	//アクティブでなければ更新処理は行わない
	if (!IsActive()) {
		return;
	}

	//現在の状態で処理を分岐
	switch (mStatus) {
	case Status::Waiting:
		_updateWaiting();
		break;
	case Status::Staying:
		_updateStaying();
		break;
	case Status::Reserving:
		_updateReserving();
		break;
	case Status::Flying:
		_updateFlying();
		break;
	}

	//敵共通の処理
	//これは最新の位置をSprite等に反映させるものなので、
	//派生クラスの更新処理の最後に呼ぶ
	Enemy::Update();
}

//アニメーションの初期化
void Pigeon2::_initAnimation() {

	//UVの幅と高さ
	float uvW = 1.f / 4.f;
	float uvH = 1.f / 1.f;

	//スプライトセル構造体リスト(４パターン分)
	UVRect uvs[] = {
		{uvW * 0.f,0.f,uvW,uvH}, //右向き　パターン１
		{uvW * 1.f,0.f,uvW,uvH}, //右向き　パターン２
		{uvW * 2.f,0.f,uvW,uvH}, //左向き　パターン１
		{uvW * 3.f,0.f,uvW,uvH}, //左向き　パターン２
	};

	//鳩のアイドル時のアニメ
	//右
	UVRect idle_right[] = {
		uvs[0],
		uvs[1],
		{}
	};

	//左
	UVRect idle_left[] = {
		uvs[2],
		uvs[3],
		{}
	};

	//鳩の準備時のアニメ
	//右
	UVRect reserv_right[] = {
		uvs[0],
		uvs[1],
		{}
	};

	//左
	UVRect reserv_left[] = {
		uvs[2],
		uvs[3],
		{}
	};

	//鳩の移動時のアニメ
	UVRect fly_right[] = { uvs[0] };	//右
	UVRect fly_left[] = { uvs[2] };		//左

	//アニメーションデータ
	Animation anim[6];

	//アニメーションデータ作成：アイドル時アニメ
	//右向き
	CreateAnimationUV(anim[0], "idle_right", 3, 0.45f, true, idle_right);
	//左向き
	CreateAnimationUV(anim[1], "idle_left", 3, 0.45f, true, idle_left);

	//アニメーションデータ作成：準備時アニメ
	//右向き
	CreateAnimationUV(anim[2], "reserv_right", 2, 0.2f, true, reserv_right);
	//左向き
	CreateAnimationUV(anim[3], "reserv_left", 2, 0.2f, true, reserv_left);

	//アニメーションデータ作成：飛行時アニメ
	//右向き
	CreateAnimationUV(anim[4], "fly_right", 1, 0.f, false, fly_right);
	//左向き
	CreateAnimationUV(anim[5], "fly_left", 1, 0.f, false, fly_left);

	//アニメーションをスプライトに登録
	for (int i = 0; i < 6; ++i) {
		mSprite.AddAnimation(anim[i]);
	}

	//初期のアニメ設定
	mSprite.PlayAnimation("idle_left");
}

//生成前の処理
void Pigeon2::_updateWaiting() {
	// 画面内に入ったら「Staying」に切り替える
	if (EnemyMng::IsInView(mPosition)) {

		mPosition.x -= 50.f;				//画面端に余裕を持たせて生成する
		mStatus = Status::Staying;			//ステータスをStayingに変更		
		// ここで初めて距離測定もできる
		_SetDistanceBetweenTwo();			// プレイヤーとの距離を記録
		mHasMeasuredDistance = true;

	}
	// 画面外なら何もしない
}

//待機時の処理
void Pigeon2::_updateStaying() {

	//3秒経過するまで繰り返す
	if (mTimer < 4.f) {

		//3秒経過していなければ、プレイヤーと一定の距離を保ちながら。
		//3秒計測する。
		//鳩とプレイヤー、二点間の距離を使う準備
		Vector2f currentPlayerPos = GetPlayer()->GetPosition();	//プレイヤーの座標を一時保存しておく変数の宣言

		//鳩が左向きの処理
		if (mDirection == Direction::Left) {
			//鳩の位置をプレイヤー足す処理
			mPosition.x = currentPlayerPos.x + mDistanceX;
		}
		//鳩が右向きの処理
		else if (mDirection == Direction::Right) {
			//鳩の位置をプレイヤー足す処理
			mPosition.x = currentPlayerPos.x - mDistanceX;
		}

		mTimer += Time_I->GetDeltaTime();		//指定時間経過まで時間を計測し続ける
		
		//アニメーションの更新
		{
			//アイドル時のアニメーション名
			const string animName_idle[] = { "","idle_right","","idle_left" };

			//再生するアニメーション名
			string playAnimName;

			//向きに応じたアイドル時のアニメを選択
			playAnimName = animName_idle[(int)mDirection];

			//再生するアニメを選択
			//第二引数は"既に同じアニメを再生していた場合そのまま継続する"という指定
			mSprite.PlayAnimation(playAnimName, AnimationPlayStyle::Continue);
		}
	}

	//3秒経過したらReservingに移行
	else if (mTimer >= 4.f) {

		mStatus = Status::Reserving;
		mTimer = 0.f;
	}
}

//飛立つ準備の処理
void Pigeon2::_updateReserving() {

	// 回転演出を追加しながら、一定時間経過後に飛行へ移行する

	// Reserving 初回フレームで中心座標を保存（Staying→Reservingで既に保存済みでもOK）
	if (mTimer == 0.0f) {
		mInitialPos = mPosition;  // この位置を中心に回転し、後に飛行開始位置とする
	}

	// カメラ（プレイヤー）に追従させるため、X座標だけ追従させる
	Vector2f playerPos = GetPlayer()->GetPosition();
	if (mDirection == Direction::Left) {
		mInitialPos.x = playerPos.x + mDistanceX;
	}
	else if (mDirection == Direction::Right) {
		mInitialPos.x = playerPos.x - mDistanceX;
	}
	// Yは初期の高さを保つことで「上空回転」を維持

	// 回転角の計算（時間に応じて回転）
	float angle = mReservingRotSpeed * mTimer; // 現在の角度（ラジアン）

	// 回転により位置を更新
	mPosition.x = mInitialPos.x + cosf(angle) * mReservingRadius;
	mPosition.y = mInitialPos.y + sinf(angle) * mReservingRadius;

	// タイマー更新
	mTimer += Time_I->GetDeltaTime();

	//アニメーションの更新
	{
		//アイドル時のアニメーション名
		const string animName_reserv[] = { "","reserv_right","","reserv_left" };

		//再生するアニメーション名
		string playAnimName;

		//向きに応じた準備時のアニメを選択
		playAnimName = animName_reserv[(int)mDirection];

		//再生するアニメを選択
		//第二引数は"既に同じアニメを再生していた場合そのまま継続する"という指定
		mSprite.PlayAnimation(playAnimName, AnimationPlayStyle::Continue);
	}

	// 2秒経過後に Flying に移行
	if (mTimer >= 1.5f) {
		mStatus = Status::Flying;

		// 飛行開始の前準備として、発光を解除
		mSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);

		// 飛行用タイマーも初期化
		mFlyingTimer = 0.0f;

		// Reserving タイマーをリセット（次回の使用に備える）
		mTimer = 0.0f;
	}
}

// 飛行時の処理（ベジェ曲線を使って移動）
void Pigeon2::_updateFlying() {

	// 飛行開始時に一度だけ行う初期設定
	if (mFlyingTimer == 0.0f) {

		// ベジェ曲線の始点を現在位置に設定（鳩の現在地）
		mStartPos = mPosition;

		// 中間点：プレイヤーの現在位置（ただしYは固定）
		mControlPos = GetPlayer()->GetPosition();
		mInitialControlPos = mControlPos; // Y座標の固定に使う

		// 終点：プレイヤーの位置から見た鳩の左右位置（Xのみ追従）
		mEndPos = mPosition;
		mEndPos.x = mControlPos.x + (mDirection == Direction::Left ? -mDistanceX : mDistanceX);
		mInitialEndPos = mEndPos; // プレイヤーのX移動分を後で加算するため

		// 飛行にかかる時間を設定（秒）
		mFlyingDuration = 1.5f;

		// プレイヤーの位置を記録（X追従用の基準点）
		mPlayerPosOnFlyStart = mControlPos;

	}

	// 飛行時間の進行（フレームごとに加算）
	mFlyingTimer += Time_I->GetDeltaTime();

	// 進行度 t を 0.0 ～ 1.0 の範囲に正規化
	float t = mFlyingTimer / mFlyingDuration;
	if (t > 1.0f) t = 1.0f;

	// なめらか中間減速：最初と最後は t に等しい速さ・中間でのみ遅くなる
	float easedT = t + 0.1f * sinf(t * PI);

	// 現在のプレイヤーX座標との差分を取得
	Vector2f currentPlayerPos = GetPlayer()->GetPosition();
	float playerDeltaX = currentPlayerPos.x - mPlayerPosOnFlyStart.x;

	// 中間点・終点をプレイヤーのX移動に合わせて補正
	mControlPos.x = mInitialControlPos.x + playerDeltaX;
	mControlPos.y = mInitialControlPos.y - 300.0f; // Yは固定高に調整（プレイヤーのジャンプ無視）

	mEndPos.x = mInitialEndPos.x + playerDeltaX;
	// ※Yは使わないので調整不要

	// easedT を使って 2次ベジェ曲線の位置を計算
	// B(t) = (1 - t)^2 * P0 + 2 * (1 - t) * t * P1 + t^2 * P2
	mPosition =
		(1.0f - easedT) * (1.0f - easedT) * mStartPos +
		2.0f * (1.0f - easedT) * easedT * mControlPos +
		easedT * easedT * mEndPos;

	// 画面外へのX方向移動を制限（左右に50pxのマージン）
	float camX = RenderManager_I->GetCamera().GetPosition().x;
	float minX = camX - WINDOW_WIDTH / 2.0f + 50.0f;
	float maxX = camX + WINDOW_WIDTH / 2.0f - 50.0f;
	if (mPosition.x < minX) mPosition.x = minX;
	if (mPosition.x > maxX) mPosition.x = maxX;

	//アニメーションの更新
	{
		//アイドル時のアニメーション名
		const string animName_fly[] = { "","fly_right","","fly_left" };

		//再生するアニメーション名
		string playAnimName;

		//向きに応じた準備時のアニメを選択
		playAnimName = animName_fly[(int)mDirection];

		//再生するアニメを選択
		//第二引数は"既に同じアニメを再生していた場合そのまま継続する"という指定
		mSprite.PlayAnimation(playAnimName, AnimationPlayStyle::Continue);
	}

	// 規定範囲外に出たら飛行を強制終了（Y含めて安全マージンをチェック）
	if (!_IsInSafeView(100.0f)) {

		// 高さを初期値に戻す（地面に埋まったりしないよう）
		mPosition.y = mInitialY;

		// 状態とタイマーのリセット
		mStatus = Status::Staying;
		mTimer = 0.f;
		mFlyingTimer = 0.f;
		return;
	}

	// 飛行が完了（t = 1.0）したら処理を終了・次ステップへ
	if (t >= 1.0f) {

		// 状態を待機に戻し、タイマー初期化
		mStatus = Status::Staying;
		mTimer = 0.f;
		mFlyingTimer = 0.f;

		// 次回飛行の方向を反転（左⇔右）
		mDirection = (mDirection == Direction::Left) ? Direction::Right : Direction::Left;

		// 新たな距離を再測定（次回飛行のため）
		_SetDistanceBetweenTwo();

	}
}

//プレイヤーと鳩の二点間の距離を返す処理
float Pigeon2::_GetDistanceBetweenTwo() {
	return mDistanceX;
}

//プレイヤーと鳩の二点間の距離を測る処理
void Pigeon2::_SetDistanceBetweenTwo() {

	//二点の計算をするための準備をする
	Vector2f currentPlayerPos = GetPlayer()->GetPosition();		//プレイヤーの座標を一時保存しておく変数の宣言

	//プレイヤーと鳩の二点の距離を測る
	mDistanceX = abs(mPosition.x - currentPlayerPos.x);			//X座標の距離を測る		
}

//安全マージン付きの画面内判定（画面からmarginピクセル以上はみ出すとfalse）
bool Pigeon2::_IsInSafeView(float margin) {
	// カメラの中央座標を取得（ワールド座標）
	Vector2f cameraPos = RenderManager_I->GetCamera().GetPosition();

	// 画面の半分の幅と高さを計算（中心からのオフセットとして使用）
	float halfWidth = WINDOW_WIDTH / 2.0f;
	float halfHeight = WINDOW_HEIGHT / 2.0f;

	// カメラを中心とした表示領域の左・右・上・下をマージン付きで定義
	float left = cameraPos.x - halfWidth - margin;
	float right = cameraPos.x + halfWidth + margin;
	float top = cameraPos.y + halfHeight + margin;
	float bottom = cameraPos.y - halfHeight - margin;

	// 現在の鳩の座標が、この領域内にあるかどうかを判定
	return (mPosition.x > left && mPosition.x < right &&
		mPosition.y > bottom && mPosition.y < top);
}