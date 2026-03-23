//GameObjectMng.h

#pragma once

#include "Tilemap/Tilemap.h"

//プレイヤー
#include "Player/Player.h"
//敵マネージャー
#include "Enemy/EnemyMng.h"
//商品
#include "Product/Product.h"
//ステージ情報構造体
#include "Mst/StageData.h"

//全オブジェクト初期化
void GameObjectInit(StageData* pStageData);

//全オブジェクト更新
void GameObjectUpdate();

//全オブジェクト描画
void GameObjectRender();

//全オブジェクト解放
void GameObjectTerm();

//Tilemapのアドレスを返す
Tilemap* GetMap();

//プレイヤークラスのポインタをかえす
Player* GetPlayer();

//敵マネージャークラスのポインタを返す
EnemyMng* GetEnemyMng();

//商品クラスのポインタを返す
Product* GetProduct();