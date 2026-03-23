//Player.cpp

#include "Player.h"

//Input_Iを使うためのヘッダファイルをinclude
#include "Fwk/Framework.h"

//GetMapを使う為GameObjectMng.hをinclude
#include "GameObjectMng/GameObjectMng.h"
#include "AppDef.h"
#include <iostream>

//プレイヤーの初期化
void Player::Init()
{
	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/player.png");

	//スプライトの初期化
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(128.0f, 128.0f);
	mSprite.SetVisible(true);
	mSprite.SetRotation(0);

	//Waveデータ読み込み
	mSounds[(int)Sound::Jump].Load("Sound/jumpC.wav");
	mSounds[(int)Sound::Damaged].Load("Sound/damageB.wav");
	//音源
	mSoundSoureces[(int)Sound::Jump].Init(mSounds[(int)Sound::Jump]);
	mSoundSoureces[(int)Sound::Damaged].Init(mSounds[(int)Sound::Damaged]);

	//剣の初期化
	mSword.Init();

	//アニメーションの初期化
	_initAnimation();

	//プレイヤーの初期状態を移動中に設定
	mStatus = Status::Moving;

	//コリジョンの設定
	{
		// 頭部
		//自分はプレイヤーグループのコリジョン
		mHeadCollision.SetGroup((int)CollisionGroup::Player);
		//衝突対象に敵グループのコリジョンを追加
		mHeadCollision.AddHitGroup((int)CollisionGroup::Enemy);
		//衝突対象にゴールグループのコリジョンを追加
		mHeadCollision.AddHitGroup((int)CollisionGroup::Goal);
		//コリジョンにタグを設定
		mHeadCollision.SetTag("PlayerHead");
		//コリジョンのオーナーに自分を設定
		mHeadCollision.SetOwner(this);
		//コリジョンを活性にしておく
		mHeadCollision.SetActive(true);
		//コリジョンの形状を指定(矩形)
		mHeadCollision.SetRect(0.0f, 42.f, 128.0f, 43.0f); // 上部（Yが大きいのが上）
		//コリジョン衝突時のコールバック関数を設定
		mHeadCollision.SetCallbackFunction(CreateCollisionEventCallback(Player::OnHeadHit));
		//コリジョンマネージャーにコリジョンを登録
		CollisionManager_I->Register(&mHeadCollision);

		// 体部
		//自分はプレイヤーグループのコリジョン
		mBodyCollision.SetGroup((int)CollisionGroup::Player);
		//衝突対象に敵グループのコリジョンを追加
		mBodyCollision.AddHitGroup((int)CollisionGroup::Enemy);
		//衝突対象にゴールグループのコリジョンを追加
		mBodyCollision.AddHitGroup((int)CollisionGroup::Goal);
		//コリジョンにタグを設定
		mBodyCollision.SetTag("PlayerBody");
		//コリジョンのオーナーに自分を設定
		mBodyCollision.SetOwner(this);
		//コリジョンを活性にしておく
		mBodyCollision.SetActive(true);
		//コリジョンの形状を指定(矩形)
		mBodyCollision.SetRect(0.0f, 0.f, 128.0f, 42.0f); // 中央
		//コリジョン衝突時のコールバック関数を設定
		mBodyCollision.SetCallbackFunction(CreateCollisionEventCallback(Player::OnBodyHit));
		//コリジョンマネージャーにコリジョンを登録
		CollisionManager_I->Register(&mBodyCollision);

		// 足部
		//自分はプレイヤーグループのコリジョン
		mLegsCollision.SetGroup((int)CollisionGroup::Player);
		//衝突対象に敵グループのコリジョンを追加
		mLegsCollision.AddHitGroup((int)CollisionGroup::Enemy);
		//衝突対象にゴールグループのコリジョンを追加
		mLegsCollision.AddHitGroup((int)CollisionGroup::Goal);
		//コリジョンにタグを設定
		mLegsCollision.SetTag("PlayerLegs");
		//コリジョンのオーナーに自分を設定
		mLegsCollision.SetOwner(this);
		//コリジョンを活性にしておく
		mLegsCollision.SetActive(true);
		//コリジョンの形状を指定(矩形)
		mLegsCollision.SetRect(0.0f, -43.f, 128.0f, 43.0f); // 下部
		//コリジョン衝突時のコールバック関数を設定
		mLegsCollision.SetCallbackFunction(CreateCollisionEventCallback(Player::OnLegsHit));
		//コリジョンマネージャーにコリジョンを登録
		CollisionManager_I->Register(&mLegsCollision);
	}

	//汎用タイマーリセット
	mTimer = 0.0f;
	//汎用ステップカウンタリセット
	mStep = 0;
	//無敵フレームリセット
	mInvicibilityFrames = 0;

	//速度の初期化
	mVelocity = { 0.0f,0.0f };
	//初期の向きを右向きにしておく
	mDirection = Direction::Right;

	//HPを初期化しておく
	mHpMax = 5;		//最大HPを設定
	mHp = mHpMax;	//現在HP＝最大HPにしておく

	//他の部位から奪った回数をリセット
	mForceStealCount = 0;

	GetProduct()->PrintOwnedStatus();
}

//終了
void Player::Term()
{
	//コリジョンマネージャから登録解除
	CollisionManager_I->Unregister(&mCollision);

	//Swordの後片付け
	mSword.Term();

	mSprite.Term();
	mTexture.Unload();

	//サウンドの解放
	for (int i = 0; i < (int)Sound::Count; ++i) {
		mSounds[i].Unload();
		mSoundSoureces[i].Term();
	}
}

//プレイヤーの更新
void Player::Update()
{

	//現在のステータスによって更新処理を呼び分ける
	switch (mStatus)
	{
	case Status::Moving:
		//現在のステータスが移動中の場合の更新処理
		_updateMoving();
		break;
	case Status::Attack:
		//現在のステータスが攻撃の場合の更新処理
		_updateAttack();
		break;
	case Status::Damaged:
		//現在のステータスがダメージを受けている場合の更新処理
		_updateDamaged();
		break;
	case Status::Dying:
		//現在のステータスがやられ中の場合の更新処理
		_updateDying();
		break;
	case Status::Dead:
		//現在のステータスがやられたの場合の更新処理
		break;
	}

	//やられていない時だけ、落下によるやられ判定を行う
	if (IsAlive()) {
		//タイルマップのサイズを取得する
		Vector2f mapSize = GetMap()->GetMapSize();
		//マップの下限値をY座標が下回ったらマップ外に落下したということで
		//やられた判定にする
		if (mPosition.y < -mapSize.y) {
			GetProduct()->LoseAllProducts();
			_die();
		}
	}

	//やられている状態以外では天井・床の接触をチェックする
	if (IsAlive())
	{
		//下向きに加速しているとき、地面に接していたら
		if (mVelocity.y < 0.0f && _isGround()) {
			//速度のY成分をリセット
			mVelocity.y = 0.0f;
		}
		//上向きに加速しているとき、天井に接していたら
		if (mVelocity.y > 0.0f && _isCeil()) {
			//速度のY成分をリセット
			mVelocity.y = 0.0f;
		}
	}

	//無敵関連の更新処理
	_updateInvincibility();

	//スプライトの位置を更新
	mSprite.SetPosition(mPosition);

	//剣に最新の位置を設定
	mSword.SetPosition(mPosition);

	//スプライトの更新
	mSprite.Update();

	//剣の更新
	mSword.Update();

	//コリジョンの位置更新
	mHeadCollision.SetPosition(mPosition);
	mBodyCollision.SetPosition(mPosition);
	mLegsCollision.SetPosition(mPosition);
}

//プレイヤーの描画
void Player::Render()
{
	mSprite.Draw();

	//剣の描画
	mSword.Render();
}

//現在位置を取得する
Vector2f Player::GetPosition() {
	return mPosition;
}

//現在位置を設定する
void Player::SetPosition(Vector2f pos) {
	mPosition = pos;
	mCollision.SetPosition(mPosition);
	mSword.SetPosition(mPosition);
}

//現在の体力を取得する
int Player::GetHp() {
	return mHp;
}

//体力の最大値を1.0とした時の現在の体力を取得する
float Player::GetNormalizedHp() {
	return (float)mHp / (float)mHpMax;
}

//自分がやられ状態でなければtrueを返す
bool Player::IsAlive() {
	return !IsDead();
}

//自分がやられ状態であればtrueを返す
bool Player::IsDead() {
	return (mStatus == Status::Dying || mStatus == Status::Dead);
}

//ゴールに到達したか否かを取得する
bool Player::IsReacheGoal() {
	return(mStatus == Status::Goal);
}

//アニメーションの初期化
void Player::_initAnimation()
{
	//UVの幅と高さ
	float uvW = 1.f / 10.f;
	float uvH = 1.f / 1.f;

	//スプライトセル構造体リスト(８パターン分)
	UVRect uvs[] = {
		{uvW * 0.0f,0.0f,uvW,uvH}, //右向き　パターン１
		{uvW * 1.0f,0.0f,uvW,uvH}, //右向き　パターン２
		{uvW * 2.0f,0.0f,uvW,uvH}, //右向き　パターン３
		{uvW * 3.0f,0.0f,uvW,uvH}, //右向き　パターン４
		{uvW * 4.0f,0.0f,uvW,uvH}, //右向き　パターン５
		{uvW * 5.0f,0.0f,uvW,uvH}, //左向き　パターン１
		{uvW * 6.0f,0.0f,uvW,uvH}, //左向き　パターン２
		{uvW * 7.0f,0.0f,uvW,uvH}, //左向き　パターン３
		{uvW * 8.0f,0.0f,uvW,uvH}, //左向き　パターン４
		{uvW * 9.0f,0.0f,uvW,uvH}, //左向き　パターン５
	};

	//プレイヤーのアイドル時のアニメ
	UVRect idle_right[] = { uvs[0] };	//右
	UVRect idle_left[] = { uvs[5] };	//左

	//プレイヤー移動時のアニメ
	//右
	UVRect walk_right[] = {
		uvs[1],
		uvs[2],
		uvs[3],
		uvs[1],
	};
	//左
	UVRect walk_left[] = {
		uvs[6],
		uvs[7],
		uvs[8],
		uvs[6],
	};

	//ジャンプ時のアニメ
	UVRect jump_right[] = { uvs[4] };	//右
	UVRect jump_left[] = { uvs[9] };	//左

	//アニメーションデータ
	Animation anim[6];

	//アニメーションデータ作成：アイドル時アニメ
	//右向き
	CreateAnimationUV(anim[0], "idle_right", 1, 0.0f, false, idle_right);
	//左向き
	CreateAnimationUV(anim[1], "idle_left", 1, 0.0f, false, idle_left);

	//アニメーションデータ作成：歩行時アニメ
	//右向き
	CreateAnimationUV(anim[2], "walk_right", 5, 0.8f, true, walk_right);
	//左向き
	CreateAnimationUV(anim[3], "walk_left", 5, 0.8f, true, walk_left);

	//アニメーションデータ作成：ジャンプ時アニメ
	//右向き
	CreateAnimationUV(anim[4], "jump_right", 1, 0.0f, false, jump_right);
	//左向き
	CreateAnimationUV(anim[5], "jump_left", 1, 0.0f, false, jump_left);

	//アニメーションをスプライトに登録
	for (int i = 0; i < 6; ++i) {
		mSprite.AddAnimation(anim[i]);
	}

	//初期のアニメ設定
	mSprite.PlayAnimation("idle_right");
}

//現在のステータスが移動中の場合の更新処理
void Player::_updateMoving()
{
	//このフレームでの移動量
	float speed = 240.0f * Time_I->GetDeltaTime();

	//ゲームパッドの入力を取得
	Vector2f vInput;
	Input_I->GetStickInput(0, GAMEPAD_STICK::LEFT, &vInput);

	// 地上にいるかどうか
	bool isGrounded = _isGround();

	// ----------------------------------------
	// 入力取得
	// ----------------------------------------

	// 地上にいるときのみ、入力に応じて移動方向を決定
	if (isGrounded) {
		if (Input_I->IsKeyPressed(VK_LEFT) || Input_I->IsKeyPressed('A')) {
			vInput.x = -1.0f;
		}
		else if (Input_I->IsKeyPressed(VK_RIGHT) || Input_I->IsKeyPressed('D')) {
			vInput.x = 1.0f;
		}
	}

	// 空中での移動処理
	else {
		if (mIsJumping) {
			// ジャンプ中ならジャンプ時のX速度を固定
			mVelocity.x = mJumpVelocityX;
		}
		else {
			// ジャンプしていない状態で空中にいるなら、入力を反映
			_updateVelocity(vInput);
		}
	}

	vInput.y = 0.0f;
	vInput.Normalize();  // 斜め入力対策

	// ----------------------------------------
	// ジャンプ処理
	// ----------------------------------------

	// ジャンプ開始（地上でのみ可能）
	if ((Input_I->IsButtonDown(0, GAMEPAD_BUTTON::B) || Input_I->IsKeyDown(VK_SPACE)) && isGrounded) {
		mVelocity.y = 20.f;  // 固定ジャンプ高さ
		mSoundSoureces[(int)Sound::Jump].Play();

		// 空中移動抑制のためジャンプ時の速度と向きを記録
		mIsJumping = true;
		mJumpVelocityX = vInput.x * speed * 1.45f;
		mJumpDirection = mDirection;  // 向きをジャンプ開始時に固定
	}

	// ----------------------------------------
	// 移動速度の更新
	// ----------------------------------------

	if (isGrounded) {
		// 地上では入力に応じて速度を更新
		_updateVelocity(vInput);
		mIsJumping = false;  // 着地したらジャンプ状態解除
	}
	else {
		// 空中ではジャンプ時のX速度を固定維持（方向転換不可）
		mVelocity.x = mJumpVelocityX;
	}

	// ----------------------------------------
	// 実際の移動反映
	// ----------------------------------------

	_translate(mVelocity);  // 速度ベースの移動

	// ----------------------------------------
	// 向きの更新（地上時のみ）
	// ----------------------------------------

	if (isGrounded) {
		if (vInput.x < 0.0f) mDirection = Direction::Left;
		else if (vInput.x > 0.0f) mDirection = Direction::Right;
	}
	else {
		// 空中ではジャンプ開始時の向きを保持
		mDirection = mJumpDirection;
	}

	//アニメーションの更新
	{
		//アイドル時のアニメーション名
		const string animName_idle[] = { "","idle_right","","idle_left" };
		//歩行時のアニメーション名
		const string animName_walk[] = { "","walk_right","","walk_left" };
		//ジャンプ時のアニメーション名
		const string animName_jump[] = { "","jump_right","","jump_left" };

		//再生するアニメーション名
		string playAnimName;

		if (!isGrounded || mVelocity.y > 0.0f) {
			// 空中にいる、または上昇中 → ジャンプアニメ
			playAnimName = animName_jump[(int)mDirection];
		}
		else if (vInput.x != 0.0f) {
			// 地上で左右に入力がある → 歩行アニメ
			playAnimName = animName_walk[(int)mDirection];
		}
		else {
			// 地上かつ入力がない → アイドルアニメ
			playAnimName = animName_idle[(int)mDirection];
		}

		//再生するアニメを選択
		//第二引数は"既に同じアニメを再生していた場合そのまま継続する"という指定
		mSprite.PlayAnimation(playAnimName, AnimationPlayStyle::Continue);
	}

	//剣を振る
	//if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A)) {
		//mSword.Swing(mDirection);

		//プレイヤーの状態を攻撃中にする
		//mStatus = Status::Attack;
	//}

}

//速度を更新する
void Player::_updateVelocity(Vector2f vInput)
{
	//このフレームでの加速涼
	float acceleration = 50.0f * Time_I->GetDeltaTime();
	//このフレームでの最大移動量
	float maxSpeed = 300.0f * Time_I->GetDeltaTime();

	//重力の倍率
	float gravityScale = 5.0f;
	//このフレームでの重力加速量
	float gravity = -9.8f * Time_I->GetDeltaTime() * gravityScale;
	//このフレームでの最大重力
	float maxGravity = -1200.0f * Time_I->GetDeltaTime();

	//摩擦による速度の減衰
	{
		mVelocity.x *= 0.82f;
		//ある程度速度が小さくなったらゼロに丸めてしまってOK
		if (fabsf(mVelocity.x) < 0.001f) {
			mVelocity.x = 0.0f;
		}
	}

	//x軸方向の入力があれば加速する
	if (vInput.x != 0.0f) {
		//速度ベクトルに、入力方向ｘ加速量を足しこむ
		mVelocity.x += vInput.x * acceleration;
		//速さが上限を超えないようにする
		if (fabs(mVelocity.x) > maxSpeed) {
			mVelocity.x = maxSpeed * (mVelocity.x) / (fabsf(mVelocity.x));
		}
	}

	//重力の計算
	mVelocity.y += gravity;
	if (mVelocity.y < maxGravity) {
		mVelocity.y = maxGravity;
	}
}

//現在のステータスが攻撃の場合の更新処理
void Player::_updateAttack()
{
	//速度を更新する
	Vector2f vInput = { 0.0f,0.0f };
	_updateVelocity(vInput);

	//速度を用いて平行移動させる
	_translate(mVelocity);

	//剣が振られていない
	if (!mSword.IsSwinged()) {
		//プレイヤーの状態を移動中にする
		mStatus = Status::Moving;
	}
}

//現在のステータスがダメージを受けている場合の更新処理
void Player::_updateDamaged()
{
	//現在の向きに対応する吹き飛び方向
	const Vector2f KnockbackDirection[4] =
	{
		Vector2f(0.0f,1.0f),		//Front　前向きの時は画面上方向にノックバック
		Vector2f(-1.0f,0.0f),		//Right　右向きの時は画面上方向にノックバック
		Vector2f(0.0f,-1.0f),		//Back　後向きの時は画面上方向にノックバック
		Vector2f(1.0f,0.0f),		//Left　左向きの時は画面上方向にノックバック
	};

	switch (mStep)
	{
	case 0: {
		//このフレームでの移動量
		float speed = 600.0f * Time_I->GetDeltaTime();
		//吹き飛び方向に移動させる
		_translate(KnockbackDirection[(int)mDirection] * speed);
		//経過時間
		mTimer += Time_I->GetDeltaTime();
		//一定時間吹き飛んだら次のステップに進む
		if (mTimer >= 0.05f) {
			mTimer = 0.0f;
			++mStep;
		}
	}break;
	case 1: {
		//一定時間経過するのを待つ
		mTimer += Time_I->GetDeltaTime();
		//時間が経過したら
		if (mTimer >= 0.05f) {
			//無敵フレーム数を設定する
			mInvicibilityFrames = 60;
			//コリジョンを有効に戻す
			mCollision.SetActive(true);
			//プレイヤーの状態を通常移動に戻す
			mStatus = Status::Moving;
		}
	}break;
	}
}

//現在のステータスがやられ中の場合の更新処理
void Player::_updateDying() {

	//速度を更新する
	Vector2f vInput = { 0.0f,0.0f };
	_updateVelocity(vInput);

	//直接座標を更新する
	mPosition += mVelocity;

	//加速度のY成分が下向き(落下中)で、カメラが映す下限を下回っていたら
	//ステータスをやられた状態にする
	{
		//メインカメラを取得
		Camera cam = RenderManager_I->GetCamera();
		//カメラ位置のY座標から画面の高さの半分下方向にある位置が
		//ウィンドウの下限と一致する座標
		Vector2f camPos = cam.GetPosition();
		float window_bottom = camPos.y - WINDOW_HEIGHT / 2.0f;
		//プレイヤーのY座標が、カメラの範囲外まで出た場合
		if (mPosition.y < window_bottom) {
			//やられた状態にする
			mStatus = Status::Dead;
			//スプライトを非表示にする
			mSprite.SetVisible(false);
		}
	}
}

//与えた速度で平行移動させる
void Player::_translate(Vector2f vMove)
{
	//タイルのサイズ
	float TileSize = GetMap()->GetTileSize();
	//プレイヤーのタイルとの衝突幅・高さ
	float CollisionSize = 120.0f;
	//コリジョンサイズの半分をよく使うので変数にしておく
	float CollisionSizeHalf = CollisionSize / 2.0f;

	//Ｘ軸方向の移動処理
	if (vMove.x != 0.0f) {
		//現在位置に移動ベクトルのx成分を足して移動先の位置とする
		Vector2f vTargetPos = mPosition + Vector2f(vMove.x, 0.0f);

		//壁との衝突チェック
		if (!GetMap()->IsInsideWall(vTargetPos, CollisionSize, CollisionSize)) {
			//壁に衝突しないのであれば現在位置にそのまま移動先座標を適用
			mPosition = vTargetPos;
		}
		else
		{
			//壁に衝突する場合の処理

			//右向きに移動しようとしていた場合
			if (vMove.x > 0.0f) {
				//衝突したタイルの列数を算出
				int col = (int)((vTargetPos.x + CollisionSizeHalf) / TileSize);
				//衝突したタイルの右端のx座標を使って
				float tile_left = (float)col * TileSize;
				//衝突したタイルの右端に接するようにプレイヤーの位置を調整する
				mPosition.x = tile_left - CollisionSizeHalf;
			}
			//左向きに移動しようとしていた場合
			if (vMove.x < 0.0f) {
				//衝突したタイルの列数を算出
				int col = (int)((vTargetPos.x - CollisionSizeHalf) / TileSize);
				//衝突したタイルの右端のx座標を使って
				float tile_right = (float)col * TileSize + TileSize;
				//衝突したタイルの右端に接するようにプレイヤーの位置を調整する
				mPosition.x = tile_right + CollisionSizeHalf;
			}
		}
	}

	//Y軸方向の移動処理
	if (vMove.y != 0.0f) {

		//現在位置に移動ベクトルのy成分を足して移動先の位置とする
		Vector2f vTargetPos = mPosition + Vector2f(0.0f, vMove.y);

		//壁との衝突チェック
		if (!GetMap()->IsInsideWall(vTargetPos, CollisionSize, CollisionSize))
		{
			//壁に衝突しないのであれば現在位置にそのまま移動先座標を適用
			mPosition = vTargetPos;
		}
		else
		{
			//壁に衝突する場合の処理

			//上向きに移動しようとしていた場合
			if (vMove.y > 0.0f) {
				//衝突したタイルの行数を算出
				int row = (int)((vTargetPos.y + CollisionSizeHalf) / TileSize);
				//衝突したタイルの下端のy座標を使って
				float tile_bottom = (float)row * TileSize - TileSize;
				//衝突したタイルの下端に接するようにプレイヤーの位置を調整する
				mPosition.y = tile_bottom - CollisionSizeHalf;
			}
			//下向きに移動しようとしていた場合
			if (vMove.y < 0.0f) {
				//衝突したタイルの行数を算出
				int row = (int)((vTargetPos.y - CollisionSizeHalf) / TileSize);
				//衝突したタイルの上端のｙ座標を使って
				float tile_top = (float)row * TileSize;
				//衝突したタイルの上端に接するようにプレイヤーの位置を調整する
				mPosition.y = tile_top + CollisionSizeHalf;
			}
		}
	}
}

//無敵関連の処理
void Player::_updateInvincibility() {
	//無敵フレーム数が０であれば何もしない
	if (mInvicibilityFrames == 0) {
		return;
	}
	//無敵フレーム数を減らす
	--mInvicibilityFrames;
	//無敵フレームが奇数の時だけプレイヤーを光らせる
	if (mInvicibilityFrames % 2 == 0) {
		mSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
	}
	else {
		mSprite.SetAdditionalColor(0.5f, 0.5f, 0.5f);
	}
}

//衝突イベントのコールバック関数
void Player::OnHit(const Collider& pCollisionA, const Collider& pCollisionB)
{
	//敵に衝突していた場合の処理(ここはタグではなく衝突したコリジョンのグループで見る)
	if (pCollisionB.GetGroup() == (int)CollisionGroup::Enemy
		&& mInvicibilityFrames == 0) {
		//ダメージ状態にする
		mStatus = Status::Damaged;
		//被ダメージ音を再生
		mSoundSoureces[(int)Sound::Damaged].Play();
		//タイマーをリセット
		mTimer = 0.0f;
		//ステップをリセット
		mStep = 0;
		//ノックバック中に敵に衝突しないようにコリジョンを無効にする
		mCollision.SetActive(false);
		//速度をリセット
		mVelocity = { 0.0f,0.0f };
		//自分の体力を減らす
		--mHp;
		//持ち物が０になったらゲームオーバー
		if (GetProduct()->IsAllEmpty()) {
			// 商品全消失 → ゲームオーバー処理へ
			_die();
			return;
		}

		//体力が負数にならないようにしておく
		if (mHp < 0) {
			mHp = 0;
		}
		//残りの体力をデバッグ表示
		string debugStr = "HP=" + to_string(mHp);
		//DebugLog(debugStr);
	}

	//ゴールに衝突していた時の処理
	if (pCollisionB.GetTag() == "Goal") {
		//い攻撃と衝突しないようにコリジョンをOFF
		mCollision.SetActive(false);
		//プレイヤーをゴール到達状態にしておく
		mStatus = Status::Goal;
		//前向きに
		mSprite.PlayAnimation("idle_front");
	}
}

//頭の衝突イベントコールバック関数
void Player::OnHeadHit(const Collider& pCollisionA, const Collider& pCollisionB)
{
	if (mInvicibilityFrames > 0 || mStatus == Status::Damaged)
		return; // すでにダメージ処理中なら無視する

	// 足：小ダメージ（例：1）
	_handlePartHit(pCollisionA, pCollisionB, ProductSlot::Head);
}

//体の衝突イベントコールバック関数
void Player::OnBodyHit(const Collider& pCollisionA, const Collider& pCollisionB)
{
	if (mInvicibilityFrames > 0 || mStatus == Status::Damaged)
		return; // すでにダメージ処理中なら無視する

	// 足：小ダメージ（例：1）
	_handlePartHit(pCollisionA, pCollisionB, ProductSlot::Body);
}

//脚の衝突イベントコールバック関数
void Player::OnLegsHit(const Collider& pCollisionA, const Collider& pCollisionB)
{
	if (mInvicibilityFrames > 0 || mStatus == Status::Damaged)
		return; // すでにダメージ処理中なら無視する

	// 足：小ダメージ（例：1）
	_handlePartHit(pCollisionA, pCollisionB, ProductSlot::Leg);
}

//今自分が地面に立っているか
bool Player::_isGround() {
	//衝突判定用の矩形の中心点を自分の足元にする
	Vector2f vTargetPos = mPosition + Vector2f(0.0f, -64.0f);
	//衝突判定用の矩形の幅と高さ
	float CollisionWidth =120.0f;
	float CollisionHeight = 1.0f;

	//壁に衝突していたら地面に立っている
	return GetMap()->IsInsideWall(vTargetPos, CollisionWidth, CollisionHeight);
}

//今自分が天井に接しているか
bool Player::_isCeil() {
	//衝突判定用の矩形の中心点を頭の先にする
	Vector2f vTargetPos = mPosition + Vector2f(0.0f, 20.0f);
	//衝突判定用の矩形の幅と高さ
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;
	//壁に衝突していたら天井に接している
	return GetMap()->IsInsideWall(vTargetPos, CollisionWidth, CollisionHeight);
}

//自分をやられ状態にする
void Player::_die() {
	//体力を０にする
	mHp = 0;
	//以降敵と衝突しないようにコリジョンをOFF
	mCollision.SetActive(false);
	//やられた時のアニメ
	mSprite.PlayAnimation("dying");
	//上向きのベロシティを設定する
	mVelocity.y = 20.0f;
	//やられ中状態にしておく
	mStatus = Status::Dying;
}

// 指定された部位（Head/Body/Leg）での衝突処理（敵やゴールとの衝突）
void Player::_handlePartHit(const Collider& selfCollider, const Collider& otherCollider, ProductSlot part) {
	// === ゴールに到達した場合の処理 ===
	if (otherCollider.GetTag() == "Goal") {
		mCollision.SetActive(false);          // 衝突判定を無効にする（ゴール後の無敵処理）
		mStatus = Status::Goal;               // ゴールステータスに移行
		mSprite.PlayAnimation("idle_right");  // アニメーションを停止状態に切り替え
		return;
	}

	// === 敵との衝突処理 ===
	// 敵グループでない、無敵時間中、または以下の状態なら処理しない
	if (otherCollider.GetGroup() != (int)CollisionGroup::Enemy ||
		mInvicibilityFrames > 0 ||
		mStatus == Status::Damaged ||
		mStatus == Status::Dying ||
		mStatus == Status::Dead ||
		mStatus == Status::Goal)
	{
		return;
	}

	// === ダメージ処理開始 ===
	mStatus = Status::Damaged;
	mSoundSoureces[(int)Sound::Damaged].Play();
	mTimer = 0.0f;
	mStep = 0;
	mCollision.SetActive(false);
	mVelocity = { 0.0f, 0.0f };

	ProductData stolenProduct;
	bool stolen = GetProduct()->RemoveOneProduct(part, stolenProduct);

	if (stolen) {
		// 該当部位から奪取できた
		DebugLog("「" + stolenProduct.name + "」(" + std::to_string(stolenProduct.price) + "円)を失いました");
	}
	else {

		// --- 強制奪取が3回以上なら即ゲームオーバー ---
		if (mForceStealCount >= 2) {
			_die();
			return;
		}

		// --- 他の部位から奪取を試みる ---
		std::vector<ProductSlot> otherSlots = { ProductSlot::Head, ProductSlot::Body, ProductSlot::Leg };
		otherSlots.erase(std::remove(otherSlots.begin(), otherSlots.end(), part), otherSlots.end());

		bool altStolen = false;
		for (auto slot : otherSlots) {
			if (GetProduct()->HasProduct(slot)) {
				if (GetProduct()->RemoveOneProduct(slot, stolenProduct)) {
					mForceStealCount++;
					DebugLog("「" 1+ stolenProduct.name + "」(" + std::to_string(stolenProduct.price) + "円)を失いました");
					altStolen = true;
					break;  // 1個だけ奪う
				}
			}
		}

		if (!altStolen) {
		}
	}

	// === ゲームオーバー条件チェック ===
	if (GetProduct()->IsAllEmpty()) {
		DebugLog("商品をすべて失いました");
		_die();
	}
}