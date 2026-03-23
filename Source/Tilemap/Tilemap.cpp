#include "Tilemap.h"
//RenderManagerを使うのでinclude
#include "Fwk/Framework.h"
//ウィンドウサイズの定数を使うのでinclude
#include "AppDef.h"

//定数タイル１枚分のSpriteのサイズ
const float TileSize = 64.0f;

//初期化
void Tilemap::Init()
{
	//マップサイズを初期化
	mColNum = 0;
	mRowNum = 0;
	//マップデータを収める二次元配列のポインタを初期化
	mpMapData = nullptr;

	//テクスチャファイルの読み込み
	mTexture.Load("Images/2dAction/tiles.png");

	//スプライトの初期化と各種設定
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			//初期化
			mSprite[i][n].Init();
			//テクスチャ設定
			mSprite[i][n].SetTexture(mTexture);
			//サイズを設定
			mSprite[i][n].SetSize(TileSize, TileSize);
			//左上をスプライトの中心点とする
			mSprite[i][n].SetPivot(Pivot::TopLeft);
		}
	}
}

//終了
void Tilemap::Term()
{
	//マップデータを解放
	_clearMapData();

	//スプライトの解放
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			mSprite[i][n].Term();
		}
	}
	//テクスチャの破棄
	mTexture.Unload();
}

//更新
void Tilemap::Update()
{
	//マップデータが生成されていなければ何もしないで関数を抜ける
	if (mpMapData == nullptr) {
		return;
	}

	//現在のカメラ位置
	Vector2f cameraPosition = RenderManager_I->GetCamera().GetPosition();

	//カメラ位置をもとに画面の左上に位置するタイルの列・行を求める
	float Left = cameraPosition.x - WINDOW_WIDTH / 2.0f;	//画面左端のｘ座標
	float Top = cameraPosition.y + WINDOW_HEIGHT / 2.0f;	//画面上端のｙ座標
	int LeftCol = (int)(Left / TileSize);	//画面左端のタイルマップの列
	int TopRow = (int)(-Top / TileSize);	//画面上端のタイルマップの行

	//タイル描画用のスプライトに、位置とＵＶ座標を設定する
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {

			//mSprite[i][n]が表示するタイルの列・行
			int col = LeftCol + n;
			int row = TopRow + i;

			//マップデータの領域外を参照しないようにチェック
			if (col < 0 || col >= mColNum || row < 0 || row >= mRowNum) {
				mSprite[i][n].SetVisible(false);
				continue;
			}

			//位置を設定
			mSprite[i][n].SetPosition((float)col * TileSize, (float)-row * TileSize);

			//マップデータからタイルＩＤを取得
			int id = mpMapData[row][col];

			//スプライトのUV座標設定
			float w = 64.f / 384.f;
			float h = 1.f;
			float u = (float)(id % 64) * w;
			float v = (float)(id / 1) * h;
			mSprite[i][n].SetTexCoord(u, v, w, h);

			//表示設定をしておく
			mSprite[i][n].SetVisible(true);
		}
	}
}

//描画
void Tilemap::Render()
{
	//スプライトの描画
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			mSprite[i][n].Draw();
		}
	}
}

//タイルサイズを取得
float Tilemap::GetTileSize() {
	return TileSize;
}

//指定された座標にあるタイルが壁であるか否かを戻す
bool Tilemap::IsWall(Vector2f position) {
	//与えられた座標をタイルサイズで割って端数を切り捨てれば
	//マップデータの列・行となる,ｙ座標は符号に注意
	int col = (int)(position.x / TileSize);
	int row = (int)(position.y / -TileSize);
	//列・行を指定して壁か否かを判定する
	return _isWall(col, row);
}

//指定された列・行が壁であるか否かを戻す
bool Tilemap::_isWall(int col,int row)
{
	//不正値チェック
	if (col < 0 || col >= mColNum || row < 0 || row>=mRowNum) {
		return false;
	}
	//指定の列・行のタイルIDを取得する
	int id = mpMapData[row][col];
	//タイルID1,2番を壁と判定する
	if (id == 1) {
		return true;
	}
	if (id == 2) {
		return true;
	}
	if (id == 5) {
		return true;
	}
	return false;
}
//指定した矩形の内側に入り込んでいるかを戻す
bool Tilemap::IsInsideWall(Vector2f position, float width, float height)
{
	//矩形範囲内のすべてのタイルについて、壁であるか、矩形範囲に入り込んでいるかを判定する
	//まずは矩形の四隅の座標を作る(中心点±幅の半分、中心点±高さの半分)
	float left = position.x - width / 2.0f;
	float right = position.x + width / 2.0f;
	float top = position.y + height / 2.0f;
	float bottom = position.y - height / 2.0f;

	//矩形の四隅の座標をそれぞれマップデータの列・行に変更
	//これをもとに左上→右下までのタイルを1つずつ衝突判定していく
	int col_left = (int)(left / TileSize);
	int row_top = (int)(top / -TileSize);
	int col_right = (int)(right / TileSize);
	int row_bottom = (int)(bottom / -TileSize);

	for (int row = row_top; row <= row_bottom; ++row) {
		for (int col = col_left; col <= col_right; ++col) {

			//壁でなければ次のタイルの判定を処理する
			if (!_isWall(col, row)) {
				continue;
			}

			//列と行から、壁と判定したタイルの中心座標を作る
			//列×タイルサイズ、行×タイルサイズでタイルの左上の座標になり、
			//そこにタイルサイズの半分を＋X方向、ーY方向に足すことでタイルの中心座標(x,y)となる
			float tile_x = (float)col * TileSize + (TileSize * 0.5);
			float tile_y = (float)row * -TileSize - (TileSize * 0.5);

			//壁の中心点の座標と引数で指定された矩形の中心点の座標との差の大きさを得る
			float dx = fabsf(tile_x - position.x);
			float dy = fabsf(tile_y - position.y);

			//二つの矩形が入り込んでいるかをチェックする
			if (dx < (width + TileSize) * 0.5f
				&& dy < (height + TileSize) * 0.5f) {
				return true;
			}
		}
	}

	//チェックに一度もひっからなければ、
	//矩形の中に壁は入り込んでいない
	return false;
}

//マップをCSVDataから生成する
void Tilemap::CreateMap(int colNum, int rowNum, CSVData* pMapdata) 
{
	//不正値チェック
	if (pMapdata == nullptr) {
		return;
	}

	//現在のマップデータをリセット
	_clearMapData();

	//列数・行数を記録
	mColNum = colNum;
	mRowNum = rowNum;

	//列数ｘ行数分の配列を確保する
	mpMapData = new int* [mRowNum];
	for (int i = 0; i < mRowNum; ++i) {
		mpMapData[i] = new int[mColNum];
	}

	//行数分繰り返し
	for (int i = 0; i < mRowNum; ++i) {
		//列数分繰り返し
		for (int n = 0; n < mColNum; ++n) {
			//n列i行目のタイルIDをCSVDataから取得
			int id = pMapdata->GetInt(n + (i * mColNum));
			//マップデータのn列i行目のタイルIDを上書き
			mpMapData[i][n] = id;
		}
	}
}

//マップ全体のサイズを取得する
Vector2f Tilemap::GetMapSize() {
	Vector2f result(
		mColNum * TileSize,
		mRowNum * TileSize
	);
	return result;
}

//マップデータを消去する
void Tilemap::_clearMapData() {
	//マップデータが確保されていることを確認して
	if (mpMapData != nullptr) {
		//先にmpMapData[0]~mpMapData[mRowNum-1]までの配列を解放する
		for (int i = 0; i < mRowNum; ++i) {
			SAFE_DELETE_ARRAY(mpMapData[i]);
		}
		//最後にint*配列であるmpMapDataを解放する
		SAFE_DELETE_ARRAY(mpMapData);
		//マップデータを消したので、マップの列数・行数もリセットしておく
		mRowNum = 0;
		mColNum = 0;
	}
}