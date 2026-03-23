#pragma once
#include <vector>
#include <string>
#include <cstdlib> // rand, srand
#include <ctime>   // time

// 商品の装着部位
enum class ProductSlot {
	Head,
	Body,
	Leg
};

// 商品情報構造体
struct ProductData {
	ProductSlot slot;
	std::string name;
	int price;
};

// 商品の管理クラス
class Product {
public:
	// 初期化（CSV不要、マスター初期化＋所持品初期化）
	void Init();

	// 商品を追加する（所持品に加える）
	void AddProduct(const ProductData& product);

	// 所持状態の表示（部位別）
	void PrintOwnedStatus();

	// 商品を1つ奪われる（奪われリストに移動）
	bool RemoveOneProduct(ProductSlot slot, ProductData& out);

	// すべての商品を持っていないかどうかチェック
	bool IsAllEmpty() const;

	// 所持中の商品リストを取得（参照）
	const std::vector<ProductData>& GetProducts(ProductSlot slot);

	// 奪われた商品リストを取得（参照）
	const std::vector<ProductData>& GetStolenProducts(ProductSlot slot);

	// 商品情報をすべてリセット（所持・奪われた両方）
	void Reset();

	// スロット enum を文字列に変換する関数
	std::string SlotToString(ProductSlot slot);


	// 奪われた商品の一覧をスロットごとに出力（リザルトやデバッグ用）
	void PrintStolenSummary();

	// 指定スロットに商品があるかどうか
	bool HasProduct(ProductSlot slot);

	void LoseAllProducts();

private:
	// 所持している商品（部位別）
	std::vector<ProductData> _ownedHead;
	std::vector<ProductData> _ownedBody;
	std::vector<ProductData> _ownedLeg;

	// 奪われた商品（部位別）
	std::vector<ProductData> _stolenHead;
	std::vector<ProductData> _stolenBody;
	std::vector<ProductData> _stolenLeg;

	// 商品マスターデータ（全リスト）
	std::vector<ProductData> _masterProducts;

	// 初期の商品マスターデータを作成
	void InitializeMasterProductList();

	// 所持ベクターへの参照を返す
	std::vector<ProductData>& GetOwnedVector(ProductSlot slot);

	// 奪われたベクターへの参照を返す
	std::vector<ProductData>& GetStolenVector(ProductSlot slot);
};