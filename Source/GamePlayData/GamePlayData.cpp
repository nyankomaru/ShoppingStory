#include "GamePlayData.h"

//ゲームプレイ情報を記録する為の変数
GamePlayData playData;

//ゲームプレイ情報を取得する
GamePlayData& GetGamePlayData() {
	return playData;
}

//初期化
void GamePlayData::Init() {

	mStageNo = 0;

	for (int i = 0; i < CLEAR_FLAG_ELEMENT_NUM; ++i) {
		mClearFlags[i] = 0;
	}
}

//ステージ番号のアクセサ
int GamePlayData::GetStageNo() {
	return mStageNo;
}

void GamePlayData::SetStageNo(int stageNo) {
	mStageNo = stageNo;
}

//クリア済みフラグのアクセサ
bool GamePlayData::IsStageCleared(int stageNo) {
	int elementIndex = stageNo / 8;
	int flagNo = stageNo % 8;
	if (elementIndex < 0 || elementIndex >= CLEAR_FLAG_ELEMENT_NUM) {
		return false;
	}
	return mClearFlags[elementIndex] & (1 << flagNo);
}

void GamePlayData::SetStageState(int stageNo, bool isCleared) {
	int elementIndex = stageNo / 8;
	int flagNo = stageNo % 8;
	if (elementIndex < 0 || elementIndex >= CLEAR_FLAG_ELEMENT_NUM) {
		return;
	}
	if(isCleared) {
		//フラグを立てる
		mClearFlags[elementIndex] |= (1 << flagNo);
	}
	else {
		//フラグを下げる
		mClearFlags[elementIndex] &= ~(1 << flagNo);
	}
}