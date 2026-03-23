#pragma once

//方向を示す定数
enum class Direction {
	Front,
	Right,
	Back,
	Left,
};

//衝突グループを示す定数
enum class CollisionGroup {
	Player,
	Weapon,
	Enemy,
	Goal,
};

//シーン種別
enum class SceneType {
	None = -1,
	Title,
	InGame,
	GameOver,
	GameClear,
	Result,
	StageSelect,
};