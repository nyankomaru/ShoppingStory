#include "Sword.h"

//DebugLogを使う為に必要なのでinclude
#include "Fwk/Framework.h"

//敵のメンバ関数を呼び出すので#include
#include "Enemy/Enemy.h"

//衝突範囲テーブル1 (向き別) 振り降ろした後のパターンの衝突範囲
const Rect hitRects[4] = {
	{{0.0f,-58.0f},32.0f,64.0f},	//front
	{{68.0f,0.0f},64.0f,32.0f},		//right
	{{0.0f,58.0f},32.0f,64.0f},		//back
	{{-68.0f,0.0f},64.0f,32.0f},	//left
};

//衝突範囲テーブル２(向き別)ふり卸中(斜めの時)の衝突範囲
const Rect hitRects2[4] = {
	{{0.0f,0.0f},0.0f,0.0f},		//front(用意ナシ)
	{{70.0f,42.0f},54.0f,32.0f},	//right
	{{0.0f,0.0f},0.0f,0.0f},		//back(用意ナシ)
	{{-74.0f,42.0f},54.0f,52.0f},		//left
};

//初期化
void Sword::Init()
{
	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/sword.png");

	//Spriteの初期化と各種設定
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(128.0f, 64.0f);
	mSprite.SetVisible(false);

	//Waveデータ読み込み
	mSound.Load("Sound/swingC.wav");
	//音源
	mSoundSoure.Init(mSound);

	//アニメーションの初期化
	_initAnimation();

	//コリジョンの設定
	{
		//自分は武器グループのコリジョン
		mCollision.SetGroup((int)CollisionGroup::Weapon);
		//衝突対象に敵グループのコリジョンを追加
		mCollision.AddHitGroup((int)CollisionGroup::Enemy);
		//コリジョンにタグを設定
		mCollision.SetTag("Sword");
		//コリジョンのオーナーに自分を設定
		mCollision.SetOwner(this);
		//コリジョンを非活性にしておく
		mCollision.SetActive(false);
		//コリジョン衝突時のコールバック関数を設定
		mCollision.SetCallbackFunctionEx(CreateCollisionEventCallbackEx(Sword::OnHit));
		//コリジョンマネージャーにコリジョンを登録
		CollisionManager_I->Register(&mCollision);
	}
}

//終了
void Sword::Term()
{
	//コリジョンマネージャから登録解除
	CollisionManager_I->Unregister(&mCollision);

	mSprite.Term();
	mTexture.Unload();

	//効果音の破棄
	mSound.Unload();
	//音源の解放
	mSoundSoure.Term();
}

//更新
void Sword::Update() {
	//スプライトの更新
	mSprite.Update();
}

//描画
void Sword::Render() {
	mSprite.Draw();
}

//位置設定
void Sword::SetPosition(Vector2f position) {
	//スプライトの位置を更新
	mSprite.SetPosition(position);
	//コリジョンの位置も更新
	mCollision.SetPosition(position);
}

//指定の方向に剣を振る
void Sword::Swing(Direction direction) {

	const string animName_swing[] = { "swing_front","swing_right","swing_back","swing_left" };
	mSprite.SetVisible(true);
	mSprite.PlayAnimation(animName_swing[(int)direction]);
	//向きを覚えておく
	mDirection = direction;
	//スイング音の再生
	mSoundSoure.Play();
}

//剣が振られているか
bool Sword::IsSwinged() {
	//アニメーションしていたらtrueを返す
	return mSprite.IsAnimationPlaying();
}

//アニメーションの初期化
void Sword::_initAnimation() {

	//パターン1つ分のUV座標上の幅と高さ
	float uvW = 1.0f / 4.0f;
	float uvH = 1.0f / 2.0f;

	//８パターン分のUV座標とPivot
	SpriteCell cells[8] = {
		{{uvW * 0.0f,0.0f,uvW,uvH},{0.5f,-0.5f}},	//０　上
		{{uvW * 1.0f,0.0f,uvW,uvH},{0.0f,0.0f}},	//1　右上
		{{uvW * 2.0f,0.0f,uvW,uvH},{0.0f,0.5f}},	//2　右
		{{uvW * 3.0f,0.0f,uvW,uvH},{0.0f,1.0f}},	//3　右下

		{{uvW * 0.0f,0.5f,uvW,uvH},{0.5f,1.5f}},	//4　下
		{{uvW * 1.0f,0.5f,uvW,uvH},{1.0f,1.0f}},	//5　左下
		{{uvW * 2.0f,0.5f,uvW,uvH},{1.0f,0.5f}},	//6　左
		{{uvW * 3.0f,0.5f,uvW,uvH},{1.0f,0.0f}},	//7　左上
	};

	//UV・Pivot設定リスト　右に剣を振る
	SpriteCell swing_right[]{
		cells[0],
		cells[1],
		cells[2],
		{}
	};

	//UV・Pivot設定リスト　前方に剣を振る
	SpriteCell swing_front[]{
		cells[2],
		cells[3],
		cells[4],
		{}
	};

	//UV・Pivot設定リスト　左に剣を振る
	SpriteCell swing_left[]{
		cells[0],	//上
		cells[7],	//右上
		cells[6],	//左
		{}
	};

	//UV・Pivot設定リスト　後に剣を振る
	SpriteCell swing_back[]{
		cells[6],
		cells[7],
		cells[0],
		{}
	};

	//キーフレーム時刻
	float keyTimes[] = {
		0.0f,
		0.05f,
		0.10f,
		0.25,
	};

	//アニメーションデータ作成
	Animation anim[4];
	CreateAnimationSpriteCell(anim[0], "swing_front", 4, keyTimes, false, swing_front);
	CreateAnimationSpriteCell(anim[1], "swing_right", 4, keyTimes, false, swing_right);
	CreateAnimationSpriteCell(anim[2], "swing_back", 4, keyTimes, false, swing_back);
	CreateAnimationSpriteCell(anim[3], "swing_left", 4, keyTimes, false, swing_left);

	for (int i = 0; i < 4; ++i) {
		//各アニメーションにイベントのコールバック関数を登録する
		anim[i].SetEventCallback(CreateAnimationEventCallback(Sword::OnAnimationEvent));
		//アニメーションをスプライトに追加
		mSprite.AddAnimation(anim[i]);
	}

	//剣のスプライトにアニメーションを追加する
	for (int i = 0; i < 4; ++i) {
		mSprite.AddAnimation(anim[i]);
	}

	//描画プライオリティ上げておく
	mSprite.SetPriority(10);
}

//アニメーションイベントのコールバック関数
void Sword::OnAnimationEvent(const AnimationEvent& animEvent)
{
	switch (animEvent.EventType) {
		//アニメーション開始
		case AnimationEventType::Begin:
		break;
		//アニメーション振興
		case AnimationEventType::KeyframeProgressed:
		{
			//振り下ろしたパターンのキーフレームに到達したら
			if (animEvent.KeyFrameIndex == 1) {
				//衝突を活性状態にする
				mCollision.SetActive(true);
				//パターンに合わせた衝突範囲を設定する
				mCollision.SetRect(hitRects2[(int)mDirection]);
			}
			//振り下ろしたパターンのキーフレームに到達したら
			if (animEvent.KeyFrameIndex == 2) {
				//衝突を活性状態にする
				mCollision.SetActive(true);
				//パターンに合わせた衝突範囲を設定する
				mCollision.SetRect(hitRects[(int)mDirection]);
			}
		}break;

		//アニメーション完了
		case AnimationEventType::Completed:
		{
			//剣を振り下ろし終えたのでコリジョンを非活性にする
			mCollision.SetActive(false);
		}break;
	}
}

//衝突イベントのコールバック関数
void Sword::OnHit(const CollisionEvent& collisionEvent) {

	//衝突した瞬間のイベントのみ処理をさせる
	if (collisionEvent.EventType == CollisionEventType::Enter) {
		//相手が敵である場合
		if (collisionEvent.ColliderB->GetGroup() == (int)CollisionGroup::Enemy) {
			Enemy* pEnemy = (Enemy*)(collisionEvent.ColliderB->GetOwner());
			pEnemy->OnDamaged(1);
		}
	}
}