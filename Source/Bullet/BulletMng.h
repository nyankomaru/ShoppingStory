#pragma once

//Position構造体の定義を取り込む
#include "Position.h"

//メンバ変数にBulletの配列を持つためにinclude
#include "Bullet.h"

class BulletMng
{
public:

	//弾マネージャーの初期化
	void Init();

	//弾マネージャーの更新
	void Update();

	//弾マネージャーの描画
	void Render();

	//弾マネージャーの後片付け
	void Term();

	//指定位置に弾を生成
	void GenerateBullet(Position pos);

private:
	//定数,C言語のenumは型名なしで列挙ができる
	enum {
		BulletBufferSize = 20
	};

	Bullet mBullets[BulletBufferSize];
};

//他のプログラムからBulletMngの変数を参照する為の関数
//変数はbulletMng.cppに宣言する
BulletMng& GetBulletMng();