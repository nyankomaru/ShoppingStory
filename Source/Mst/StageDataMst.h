#pragma once

//ステージ情報構造体を使う為
#include "StageData.h"

//std::vectorを使う為
#include <vector>
using namespace std;

//ステージ情報マスタークラス
class StageDataMst
{
public:
	//初期化
	void Init();
	//終了
	void Term();
	//データ数を返す
	int Size();
	//指定した番号のステージデータを返す
	StageData* Get(int index);

private:
	//ステージ情報リスト
	vector<StageData>mData;
};

//ステージ情報マスタを取得する
StageDataMst& GetStageDataMst();