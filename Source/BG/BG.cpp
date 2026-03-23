#include "BG.h"
//RenderManagerを使うのでinclude
#include "Fwk/Framework.h"
//ウィンドウサイズの定数を使うのでinclude
#include "AppDef.h"

// BGを初期化
void BG::Init() {
    for (int i = 0; i < NUM_BACKGROUND; ++i) {
        bg[i].sprite.Init();

        // テクスチャ読み込み
        mTextures[i].Load(imageFiles[i]);

        // テクスチャ設定
        bg[i].sprite.SetTexture(mTextures[i]);

        // サイズ設定
        bg[i].sprite.SetSize((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

        bg[i].position.x = i * WINDOW_WIDTH;
        bg[i].position.y = -0.5f * WINDOW_HEIGHT;
        bg[i].scrollRate = 0.5f;

        // Pivot の指定（正しい列挙名に置き換えてください）
        bg[i].sprite.SetPivot(Pivot::Left);
        bg[i].sprite.SetPosition(bg[i].position.x, bg[i].position.y);
    }
}

void BG::Update() {
    // カメラからX方向の移動量（プレイヤーの動きに応じたカメラ移動）を取得
    Vector2f camPos = RenderManager_I->GetCamera().GetPosition();
    // ここではカメラのX座標を利用。parallaxRate をかけて背景の移動に反映します。
    // 背景の移動オフセット（ここではパララックスとして半分の速度で動く）
    float offset = camPos.x * 0.5f;

    // 背景全体の横幅
    float totalWidth = WINDOW_WIDTH * NUM_BACKGROUND;

    for (int i = 0; i < NUM_BACKGROUND; ++i) {
        // 各背景の元々の初期オフセットは i * WINDOW_WIDTH です。
        // ここからカメラのオフセットを引いた値を現在の位置とします。
        float newX = i * WINDOW_WIDTH - offset;

        // ループさせるために、newX が画面の左端から完全に外れた場合（例：newX < -WINDOW_WIDTH）
        // または右端を超えた場合、totalWidth を加減してループさせます。
        while (newX < -WINDOW_WIDTH) {
            newX += totalWidth;
        }
        while (newX >= totalWidth - WINDOW_WIDTH) {
            newX -= totalWidth;
        }

        bg[i].position.x = newX;
        bg[i].sprite.SetPosition(bg[i].position.x, bg[i].position.y);
    }
}

void BG::Render() {
    for (int i = 0; i < NUM_BACKGROUND; ++i) {
        bg[i].sprite.Draw();
    }
}

void BG::Term() {

    for (int i = 0; i < NUM_BACKGROUND; ++i) {
        // スプライトのリソース解放
        bg[i].sprite.Term();
    }
    for (int i = 0; i < (int)Images::Count; ++i) {
        mTextures[i].Unload();
    }
}