#pragma once

//ゲームプレイ情報クラス
class GamePlayData
{
public:

	//初期化
	void Init();

	//ステージ番号のアクセサ
	int GetStageNo();
	void SetStageNo(int stageNo);

	//クリア済みフラグのアクセサ
	bool IsStageCleared(int stageNo);
	void SetStageState(int stageNo, bool isCleared);

private:

	//最後に選んだステージの番号
	int mStageNo;

	enum {
		CLEAR_FLAG_ELEMENT_NUM = 4
	};
	//クリア済みフラグ
	unsigned char mClearFlags[CLEAR_FLAG_ELEMENT_NUM];
};

//ゲームプレイ情報を取得する
GamePlayData& GetGamePlayData();