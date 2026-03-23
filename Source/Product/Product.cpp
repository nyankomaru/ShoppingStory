#include "Product.h"
#include <iostream>
#include "Fwk/Framework.h"

// プレイヤーの商品管理を初期化（マスター作成＋所持品初期化）
void Product::Init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // ランダム用のシード

    Reset();                    // 所持・奪われリストをクリア
    InitializeMasterProductList(); // 商品マスターデータを構築

    // 各部位に5つずつ初期所持品をランダムに追加
    for (ProductSlot slot : { ProductSlot::Head, ProductSlot::Body, ProductSlot::Leg }) {
        std::vector<ProductData> candidates;

        // 該当スロットの全商品を抽出
        for (const auto& product : _masterProducts) {
            if (product.slot == slot) {
                candidates.push_back(product);
            }
        }

        // 重複を許してランダムに5個追加
        for (int i = 0; i < 5; ++i) {
            if (!candidates.empty()) {
                int idx = std::rand() % candidates.size();
                AddProduct(candidates[idx]);
            }
        }
    }
}

// 商品マスターデータ（固定）を初期化
void Product::InitializeMasterProductList() {
    _masterProducts = {
        // 頭装備
        { ProductSlot::Head, "卵", 300 },
        { ProductSlot::Head, "バナナ", 250 },
        { ProductSlot::Head, "米", 4500 },
        { ProductSlot::Head, "もやし", 30 },
        { ProductSlot::Head, "ハチミツ", 450 },
        // 体装備
        { ProductSlot::Body, "豚肉", 350 },
        { ProductSlot::Body, "人参", 200 },
        { ProductSlot::Body, "カレールー", 400 },
        { ProductSlot::Body, "スマートフォン", 128000 },
        { ProductSlot::Body, "牛乳", 280 },

        // 足装備
        { ProductSlot::Leg, "じゃがいも", 220 },
        { ProductSlot::Leg, "玉ねぎ", 192 },
        { ProductSlot::Leg, "牛乳", 200 },
        { ProductSlot::Leg, "ヨーグルト", 180 },
        { ProductSlot::Leg, "冷凍パスタ", 360 }
    };
}

// 商品をプレイヤーの所持品に追加（部位別に格納）
void Product::AddProduct(const ProductData& product) {
    GetOwnedVector(product.slot).push_back(product);
}

// 指定スロットから商品をランダムに1つ奪い、奪われた商品を out に格納して true を返す
// 奪えなかった場合（空）は false を返す
bool Product::RemoveOneProduct(ProductSlot slot, ProductData& out) {
    auto& owned = GetOwnedVector(slot);

    // 所持品がない場合は奪えない
    if (owned.empty()) return false;

    // ランダムに1つ選択
    int idx = std::rand() % owned.size();
    out = owned[idx]; // 奪われた商品情報を out にコピー

    // 所持リストから削除
    owned.erase(owned.begin() + idx);

    // 奪われた商品として stolen リストに追加
    GetStolenVector(slot).push_back(out);

    return true;
}

// スロットを日本語の文字列に変換（表示用）
std::string Product::SlotToString(ProductSlot slot) {
    switch (slot) {
    case ProductSlot::Head: return "頭";
    case ProductSlot::Body: return "体";
    case ProductSlot::Leg:  return "足";
    default: return "不明";
    }
}

// すべてのスロットが空（商品を1つも持っていない）かどうか確認
bool Product::IsAllEmpty() const {
    return _ownedHead.empty() && _ownedBody.empty() && _ownedLeg.empty();
}

// 所持・奪われた商品リストをすべてクリア（再スタート用）
void Product::Reset() {
    _ownedHead.clear();
    _ownedBody.clear();
    _ownedLeg.clear();

    _stolenHead.clear();
    _stolenBody.clear();
    _stolenLeg.clear();
}

// 現在の所持品をスロットごとに表示（デバッグ用）
void Product::PrintOwnedStatus() {
    std::cout << "=== 所持商品一覧 ===\n";

    for (ProductSlot slot : { ProductSlot::Head, ProductSlot::Body, ProductSlot::Leg }) {
        const auto& list = GetOwnedVector(slot);

        std::cout << "[" << SlotToString(slot) << "] ";
        if (list.empty()) {
            std::cout << "なし";
        }
        else {
            for (const auto& item : list)
                std::cout << item.name << " (" << item.price << "円), ";
        }
        std::cout << "\n";
    }
}

// 現在所持している商品リスト（参照）を取得
const std::vector<ProductData>& Product::GetProducts(ProductSlot slot) {
    return GetOwnedVector(slot);
}

// 奪われた商品リスト（参照）を取得
const std::vector<ProductData>& Product::GetStolenProducts(ProductSlot slot) {
    return GetStolenVector(slot);
}

// 指定スロットに商品があるかどうか
bool Product::HasProduct(ProductSlot slot) {
    const auto& list = GetOwnedVector(slot);
    return !list.empty();
}

// 所持商品ベクターへの参照を返す（内部用）
std::vector<ProductData>& Product::GetOwnedVector(ProductSlot slot) {
    switch (slot) {
    case ProductSlot::Head: return _ownedHead;
    case ProductSlot::Body: return _ownedBody;
    case ProductSlot::Leg:  return _ownedLeg;
    default: throw std::runtime_error("無効なProductSlot");
    }
}

// 奪われた商品ベクターへの参照を返す（内部用）
std::vector<ProductData>& Product::GetStolenVector(ProductSlot slot) {
    switch (slot) {
    case ProductSlot::Head: return _stolenHead;
    case ProductSlot::Body: return _stolenBody;
    case ProductSlot::Leg:  return _stolenLeg;
    default: throw std::runtime_error("無効なProductSlot");
    }
}

// 奪われた商品の一覧をスロットごとにログ出力（リザルトやデバッグ用）
void Product::PrintStolenSummary() {
    float x = 50.0f;      // 表示開始X座標
    float y = 50.0f;      // 表示開始Y座標
    float lineHeight = 24.0f; // 行間（ピクセル）

    SetTextColor(1.0f, 1.0f, 1.0f);  // 白
    SetTextSize(20);                // 少し大きめの文字サイズ

    PrintText("=== 奪われた商品一覧 ===", x, y);
    y += lineHeight;

    int totalPrice = 0;
    int count = 0;

    for (ProductSlot slot : { ProductSlot::Head, ProductSlot::Body, ProductSlot::Leg }) {
        const auto& list = GetStolenProducts(slot);

        for (const auto& item : list) {
            if (count >= 15) break;  // 最大15行まで商品を表示

            std::string line = item.name + " (" + std::to_string(item.price) + "円)";
            PrintText(line.c_str(), x, y);
            y += lineHeight;

            totalPrice += item.price;
            ++count;
        }
    }

    // 合計金額（16行目 or 商品の下）
    PrintText(("合計金額: " + std::to_string(totalPrice) + "円").c_str(), x, y);
}

// 全ての所持商品を奪われた扱いにする
void Product::LoseAllProducts() {
    for (ProductSlot slot : { ProductSlot::Head, ProductSlot::Body, ProductSlot::Leg }) {
        auto& owned = GetOwnedVector(slot);
        auto& stolen = GetStolenVector(slot);

        // 所持品を全て stolen 側に移動
        stolen.insert(stolen.end(), owned.begin(), owned.end());
        owned.clear();
    }
}