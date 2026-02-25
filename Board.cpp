#include "Board.h"

// Constructor that builds the board and initializes card positions
// ボードを構築し、カードの初期配置を行うコンストラクタ
Board::Board(const char* cardModels[]) {

	// Create a temporary pool containing duplicated model paths for pairing
	// ペアを作るためにモデルパスを複製した一時プールを作成する
	std::vector<std::pair<const char*, int>> cardPool; // pair of model path + matchingID

	// Duplicate each model so every card has a matching pair
	// 各モデルを複製して必ずペアになるようにする
	for (int i = 0; i < 4; i++)
	{
		// First card of the pair
		// ペアの1枚目
		cardPool.push_back({ cardModels[i], i });

		// Second card of the pair
		// ペアの2枚目
		cardPool.push_back({ cardModels[i], i });
	}

	// Create random number generator
	// 乱数生成器を作成する
	std::random_device rd;
	std::mt19937 g(rd());

	// Shuffle the card pool randomly
	// カードプールをランダムにシャッフルする
	std::shuffle(cardPool.begin(), cardPool.end(), g);

	// Create Card objects from shuffled pool
	// シャッフルされたプールからCardオブジェクトを作成する
	for (auto& pair : cardPool)
	{
		// Construct card using model path
		// モデルパスを使ってカードを生成する
		Card card(pair.first);

		// Assign matching ID for pair checking
		// ペア判定用のIDを設定する
		card.SetMatchingID(pair.second);

		// Add card to board list
		// カードをボードのリストに追加する
		cards.push_back(card);
	}

	int index = 0;

	// Position cards in a grid layout
	// カードをグリッド状に配置する
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			// Calculate centered grid position
			// 中央揃えになるように位置を計算する
			glm::vec3 position(
				(col - 1.5f) * spacingX,
				0.0f,
				(row - 0.5f) * spacingZ
			);

			// Apply position to card transform
			// カードのTransformに位置を設定する
			cards[index].GetTransform().SetPos(position);

			index++;
		}
	}
}


// Reshuffles the board and resets all cards
// ボードを再シャッフルし、すべてのカードをリセットする
void Board::ReshuffleBoard()
{
	// Create random number generator
	// 乱数生成器を作成する
	std::random_device rd;
	std::mt19937 g(rd());

	// Shuffle the card order
	// カードの順番をシャッフルする
	std::shuffle(cards.begin(), cards.end(), g);

	// Reposition cards back into grid layout
	// カードを再びグリッド状に配置する
	int index = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			// Calculate centered grid position dynamically
			// 動的に中央揃え位置を計算する
			glm::vec3 position(
				(col - (columns - 1) / 2.0f) * spacingX,
				0.0f,
				(row - (rows - 1) / 2.0f) * spacingZ
			);

			// Apply position to card transform
			// カードのTransformに位置を設定する
			cards[index].GetTransform().SetPos(position);

			index++;
		}
	}

	// Reset all cards to face-down state
	// すべてのカードを裏向き状態にリセットする
	for (auto& card : cards)
	{
		card.Reset();
	}
}