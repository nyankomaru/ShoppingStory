#include "Start.h"

//CollisionManagerを使う為に必用なのでinclude
#include "Fwk/Framework.h"

//CollisionGroupを使う為に必用
#include "GameDef.h"

//初期化
void Start::Init()
{
	//テクスチャの読み込み
	mTexture.Load("Images/2dAction/Start.png");

	//Spriteの初期化と各種設定
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(128.f, 128.f);
}

//終了
void Start::Term()
{
	mSprite.Term();
	mTexture.Unload();
}

//更新
void Start::Update() {
	//スプライトの更新
	mSprite.Update();
}

//描画
void Start::Render() {
	mSprite.Draw();
}

//位置設定
void Start::SetPosition(Vector2f position) {
	//スプライトの位置を更新
	mSprite.SetPosition(position);
}