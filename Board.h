#ifndef BOARD_CLASS_H
#define BOARD_CLASS_H


#include <vector>
#include <algorithm>
#include <random>
#include <glm/gtx/string_cast.hpp>
#include "Card.h"

// Board class that manages and arranges cards
// カードを管理・配置するBoardクラス
class Board
{
public:

	// Constructor that initializes the board with card models
	// カードモデルを使ってボードを初期化するコンストラクタ
	Board(const char* cardModels[]);

	// Reshuffles cards and resets board state
	// カードを再シャッフルしボード状態をリセットする
	void ReshuffleBoard();

	// Returns reference to the card container
	// カードコンテナへの参照を返す
	inline std::vector<Card>& getCards() { return cards; };

private:

	// Horizontal spacing between cards
	// カード間の横方向の間隔
	float spacingX = 3.0f;

	// Depth spacing between cards
	// カード間の奥行き方向の間隔
	float spacingZ = 4.0f;

	// Number of columns in the grid
	// グリッドの列数
	int columns = 4;

	// Number of rows in the grid
	// グリッドの行数
	int rows = 2;

	// Container holding all cards on the board
	// ボード上のすべてのカードを保持するコンテナ
	std::vector<Card> cards;
};

#endif