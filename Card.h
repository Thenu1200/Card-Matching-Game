#ifndef CARD_CLASS_H
#define CARD_CLASS_H

#include "Model.h"

// Card class representing a playable 3D card object
// 3Dカードオブジェクトを表すCardクラス
class Card
{
public:

	// Constructor that loads the card model
	// カードモデルを読み込むコンストラクタ
	Card(const char* modelPath);

	// Updates card state (handles rotation animation)
	// カードの状態を更新する（回転アニメーション処理）
	void Update(float deltaTime);

	// Draws the card
	// カードを描画する
	void Draw(Shader& shader, Camera& camera);

	// Half extents of the card's AABB (local space)
	// カードのAABB半径（ローカル空間）
	glm::vec3 halfExtents = glm::vec3(1.5f, 0.02f, 1.0f);

	// Checks if a ray intersects the card's bounding box
	// レイがカードのバウンディングボックスと交差するか判定する
	bool CheckRayHit(const glm::vec3& rayOrigin, const glm::vec3& rayDir);

	// Returns reference to the card's transform
	// カードのTransform参照を返す
	inline Transform& GetTransform() { return transform; }

	// Sets the matching ID (used for pairing logic)
	// ペア判定用のIDを設定する
	inline void SetMatchingID(int ID) { matchingID = ID; }

	// Returns the matching ID
	// ペア判定用IDを返す
	inline int GetMatchingID() { return matchingID; }

	// Indicates whether the card has been removed
	// カードが削除されたかどうかを示す
	bool isDestroyed = false;

	// Starts the flip animation
	// カードのフリップアニメーションを開始する
	void Flip();

	// Resets the card to initial state
	// カードを初期状態にリセットする
	void Reset();

private:

	// 3D model of the card
	// カードの3Dモデル
	Model model;

	// Transform component (position, rotation, scale)
	// 位置・回転・スケールを管理するTransform
	Transform transform;

	// Current orientation quaternion
	// 現在の回転クォータニオン
	glm::quat currentOrientation;

	// Starting orientation for interpolation
	// 補間開始時の回転クォータニオン
	glm::quat startOrientation;

	// Target orientation for interpolation
	// 補間終了時の回転クォータニオン
	glm::quat targetOrientation;

	// ID used for matching pairs
	// ペア判定用ID
	int matchingID = -1;

	// Timer tracking rotation progress
	// 回転進行を追跡するタイマー
	float rotationTimer = 0.0f;

	// Duration of the flip animation
	// フリップアニメーションの時間
	float rotationDuration = 1.0f;

	// Indicates whether the card is currently rotating
	// カードが現在回転中かどうか
	bool isRotating = false;

	// Indicates whether the card is face up
	// カードが表向きかどうか
	bool isFaceUp = false;

};

#endif