#include "StageDataMst.h"

//CSVファイルを使うので
#include "Fwk/File/CSVFile.h"

//ステージ情報のマスタの変数
StageDataMst stageDataMst;

//ステージ情報マスクを取得する
StageDataMst& GetStageDataMst() {
	return stageDataMst;
}

//初期化
void StageDataMst::Init() {

	//データをクリアしておく
	mData.clear();

	//ステージマスターを読み込む
	CSVFile stageMst;
	stageMst.Load("Data/stage_msv.csv");

	//データ数/3がステージ数
	int stageNum = stageMst.GetElementCount() / 3;
	//ステージ情報を読み込んでいく
	for (int i = 0; i < stageNum; ++i) {
		//ステージデータ先頭の要素番号
		int n = i * 3;
		//新しいステージデータ
		StageData newElem;
		//ステージ名
		stageMst.GetString(n, &newElem.Name);
		//マップファイル名
		stageMst.GetString(n+1, &newElem.MapFile);
		//配置ファイル名
		stageMst.GetString(n + 2, &newElem.ArrangementFile);
		//データを追加
		mData.push_back(newElem);
	}
}

//終了
void StageDataMst::Term() {
	//データをクリアしておく
	mData.clear();
}
//データを返す
int StageDataMst::Size() {
	return mData.size();
}

//指定した番号のステージデータを返す
StageData* StageDataMst::Get(int index) {
	//不正な番号が指定された場合はnullptrを返す
	if (index < 0 || index >= mData.size()) {
		return nullptr;
	}
	return &mData[index];
}