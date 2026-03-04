#include "GameLogic.h"

// Constructor that connects Board, MousePicker, and Camera
// Board・MousePicker・Cameraを関連付けるコンストラクタ
GameLogic::GameLogic(Board& board, MousePicker& mousepicker, Camera& cam)
	: camera(cam), gameBoard(board), picker(mousepicker)
{
}


// Main game update loop
// ゲームのメイン更新処理
void GameLogic::Update(Shader shaderProgram, float deltaTime, GLFWwindow* window)
{
	// Get ray origin from camera position
	// カメラ位置からレイの始点を取得する
	glm::vec3 rayOrigin = camera.cameraPosition;

	// Get ray direction from mouse picker
	// マウスピッカーからレイの方向を取得する
	glm::vec3 rayDir = picker.getCurrentRay();

	// Get total number of cards on board
	// ボード上のカード総数を取得する
	int totalCards = gameBoard.getCards().size();


	// Handle flip delay timer
	// フリップ間の待機タイマーを処理する
	if (!canFlip)
	{
		// Increase wait timer
		// 待機タイマーを加算する
		waitTimer += deltaTime;

		// If wait duration reached, allow flipping again
		// 待機時間を超えたら再びフリップ可能にする
		if (waitTimer >= waitDuration)
		{
			canFlip = true;
			waitTimer = 0.0f;
		}
	}


	// Check ray intersection against all cards
	// すべてのカードに対してレイとの当たり判定を行う
	for (int i = 0; i < totalCards; i++)
	{
		// If ray hits this card
		// レイがこのカードに当たった場合
		if (gameBoard.getCards()[i].CheckRayHit(rayOrigin, rayDir))
		{
			// If left mouse is pressed and flipping is allowed
			// 左クリックされ、フリップ可能な場合
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && canFlip)
			{
				// Compute mirrored index (temporary fix for AABB issue)
				// AABBの不具合による暫定的なミラーインデックス計算
				int oppositeIndex = totalCards - 1 - i;

				// Skip if card already destroyed
				// すでに削除済みのカードはスキップする
				if (gameBoard.getCards()[oppositeIndex].isDestroyed) continue;

				// Flip the selected card
				// 選択されたカードをフリップする
				gameBoard.getCards()[oppositeIndex].Flip();

				// Disable flipping temporarily
				// 一時的にフリップを無効化する
				canFlip = false;

				// Assign flipped card tracking
				// フリップしたカードを記録する
				if (flippedcardone != -1)
					flippedcardtwo = oppositeIndex;
				else
					flippedcardone = oppositeIndex;
			}

			// Only handle first hit
			// 最初に当たったカードのみ処理する
			break;
		}
	}


	// If two cards are flipped and flipping is allowed again
	// 2枚のカードが選択され、再びフリップ可能な場合
	if (flippedcardone != -1 && flippedcardtwo != -1 && canFlip)
	{
		// If matching IDs are equal
		// IDが一致している場合
		if (gameBoard.getCards()[flippedcardone].GetMatchingID() ==
			gameBoard.getCards()[flippedcardtwo].GetMatchingID())
		{
			// Increase score
			// スコアを増加させる
			points++;

			std::cout << points;

			// Mark both cards as destroyed
			// 両方のカードを削除済みにする
			gameBoard.getCards()[flippedcardone].isDestroyed = true;
			gameBoard.getCards()[flippedcardtwo].isDestroyed = true;
		}
		else
		{
			// Flip cards back if not matching
			// 一致しない場合はカードを裏返す
			gameBoard.getCards()[flippedcardone].Flip();
			gameBoard.getCards()[flippedcardtwo].Flip();
		}

		// Reset flipped card tracking
		// フリップ状態をリセットする
		flippedcardone = -1;
		flippedcardtwo = -1;

		// Prevent immediate flipping
		// すぐにフリップできないようにする
		canFlip = false;
	}


	// If all pairs found, reshuffle board
	// すべてのペアを見つけた場合、ボードを再シャッフルする
	if (canFlip && points == (totalCards / 2))
	{
		gameBoard.ReshuffleBoard();
		points = 0;
	}


	// Update and draw remaining cards
	// 残っているカードを更新・描画する
	for (auto& card : gameBoard.getCards())
	{
		// Skip destroyed cards
		// 削除済みカードはスキップする
		if (card.isDestroyed) continue;

		card.Update(deltaTime);
		card.Draw(shaderProgram, camera);
	}
}