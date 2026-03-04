#ifndef GAME_LOGIC_CLASS_H
#define GAME_LOGIC_CLASS_H

#include "Board.h"
#include "MousePicker.h"
#include "Camera.h"
#include <vector>

// GameLogic class responsible for handling gameplay rules and interactions
// ゲームルールとインタラクションを管理するGameLogicクラス
class GameLogic
{
public:

	// Constructor that connects board, mouse picker, and camera
	// Board・MousePicker・Cameraを関連付けるコンストラクタ
	GameLogic(Board& board, MousePicker& mousepicker, Camera& cam);

	// Main update loop handling input, matching logic, and rendering
	// 入力処理・マッチ判定・描画を行うメイン更新処理
	void Update(Shader shaderProgram, float deltaTime, GLFWwindow* window);

private:

	// Index of first flipped card
	// 1枚目にフリップされたカードのインデックス
	int flippedcardone = -1;

	// Index of second flipped card
	// 2枚目にフリップされたカードのインデックス
	int flippedcardtwo = -1;

	// Current player score (number of matched pairs)
	// 現在のスコア（揃えたペア数）
	int points = 0;

	// Whether a card is currently allowed to flip
	// 現在フリップ可能かどうか
	bool canFlip = true;

	// Timer used to delay flipping
	// フリップ遅延用タイマー
	float waitTimer = 0.0f;

	// Duration to wait before allowing next flip
	// 次のフリップを許可するまでの待機時間
	float waitDuration = 2.0f;

	// Reference to the game board
	// ゲームボードへの参照
	Board& gameBoard;

	// Reference to the camera
	// カメラへの参照
	Camera& camera;

	// Reference to the mouse picker (ray casting system)
	// マウスピッカー（レイキャストシステム）への参照
	MousePicker& picker;
};

#endif