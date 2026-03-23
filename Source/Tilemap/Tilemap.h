#pragma once

//SpriteとTextureを使う為のヘッダファイル
#include<Fwk/Graphics.h>

//CSVデータを使う為にinclude
#include "Fwk/File/CSVFile.h"

//Vector2fを使う為のヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

class Tilemap
{
public:
	//初期化
	void Init();

	//終了
	void Term();

	//更新
	void Update();

	//描画
	void Render();

	//タイルサイズを取得
	float GetTileSize();

	//指定された座標にあるタイルが壁であるか否かを戻す
	bool IsWall(Vector2f position);

	//指定した矩形の内側に壁が入り込んでいるかいるかを返す
	bool IsInsideWall(Vector2f position, float width, float height);

	//サイズを指定してマップ生成する
	void CreateMap(int colNum, int rowNum, CSVData* pMapdata);

	//マップ全体のサイズを返す
	Vector2f GetMapSize();

private:

	//指定された列・行が壁であるか否かを戻す
	bool _isWall(int col, int row);

	//マップデータを消去する
	void _clearMapData();

private:
	//画面一枚を描画するために必要なタイル(スプライト)の列数・行数
	enum {
		RENDER_COL_NUM = 16+1,
		RENDER_ROW_NUM = 12+1,
	};

private:
	int mColNum;
	int mRowNum;

	//タイルパターンの画像を読み込む為のテクスチャ
	Texture mTexture;
	//画面一枚分のタイルを表示するためのスプライト
	Sprite mSprite[RENDER_ROW_NUM][RENDER_COL_NUM];

	//マップデータ(タイルID)を収める配列のアドレス
	int** mpMapData;
};