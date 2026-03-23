#pragma once

//SpriteやTexture,Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

//Position構造体の定義を取り込む
#include "Position.h"

//弾クラス
class Bullet {

public:
	//弾の初期化
	void Init();

	//弾の更新
	void Update();

	//弾の描画
	void Render();

	//弾の後片付け
	void Term();

	//メンバ変数は非公開にしているので、
	//値にアクセスする為の関数(アクセサ)を用意しておく

	//位置の設定
	void SetPosition(Position pos);

	//アクティブ状態の取得/設定
	bool IsActive();
	void SetActive(bool isActive);

private:
	//表示用スプライト
	Sprite mSprite;
	//テクスチャ
	Texture mTexture;
	//オブジェクトの位置
	Position mPosition;
	//活性状態か？
	bool mIsActive;
};