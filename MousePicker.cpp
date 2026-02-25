#include "MousePicker.h"

// Constructor that initializes MousePicker with a reference to the camera
// カメラへの参照を使ってMousePickerを初期化するコンストラクタ
MousePicker::MousePicker(Camera& cam) : camera(cam)
{
	// Store projection matrix from camera
	// カメラから射影行列を取得して保存する
	projMat = camera.getProjectionMatrix();

	// Store view matrix from camera
	// カメラからビュー行列を取得して保存する
	viewMat = camera.getViewMatrix();
}

// Returns the currently calculated ray in world space
// 現在計算されているワールド空間のレイを返す
glm::vec3 MousePicker::getCurrentRay()
{
	return currentRay;
}

// Updates the ray based on the current mouse position
// 現在のマウス位置に基づいてレイを更新する
void MousePicker::Update(GLFWwindow* window)
{
	// Variables to store mouse position
	// マウス座標を格納する変数
	double mouseX, mouseY;

	// Get cursor position from GLFW
	// GLFWからカーソル位置を取得する
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Recalculate ray using current mouse position
	// 現在のマウス座標を使ってレイを再計算する
	currentRay = calculateMouseRay(mouseX, mouseY);
}

// Converts mouse screen coordinates into a world space ray
// マウスのスクリーン座標をワールド空間のレイに変換する
glm::vec3 MousePicker::calculateMouseRay(double mouseX, double mouseY)
{
	// Update matrices from camera
	// カメラから最新の行列を取得する
	viewMat = camera.getViewMatrix();
	projMat = camera.getProjectionMatrix();

	// Convert mouse position to normalized device coordinates (NDC)
	// マウス座標を正規化デバイス座標（NDC）に変換する
	glm::vec2 normalizedCoord = getNormalizedDeviceCoords((float)mouseX, (float)mouseY);

	// Create clip space coordinates
	// クリップ空間座標を作成する
	glm::vec4 clipCoords = glm::vec4(normalizedCoord.x, normalizedCoord.y, -1.0f, 1.0f);

	// Convert clip coordinates to eye (camera) space
	// クリップ座標を視点空間（カメラ空間）に変換する
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);

	// Convert eye space coordinates to world space ray
	// 視点空間座標をワールド空間のレイに変換する
	glm::vec3 worldRay = toWorldCoords(eyeCoords);

	return worldRay;
}

// Converts eye space coordinates into a world space direction
// 視点空間の座標をワールド空間の方向ベクトルに変換する
glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords)
{
	// Invert the view matrix
	// ビュー行列の逆行列を取得する
	glm::mat4 invertedView = inverse(viewMat);

	// Transform eye coordinates into world space
	// 視点空間座標をワールド空間に変換する
	glm::vec4 rayWorld = invertedView * eyeCoords;

	// Normalize the direction vector
	// 方向ベクトルを正規化する
	glm::vec3 mouseRay = normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	return mouseRay;
}

// Converts clip space coordinates into eye space
// クリップ空間座標を視点空間に変換する
glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords)
{
	// Invert projection matrix
	// 射影行列の逆行列を取得する
	glm::mat4 invertedProjection = inverse(projMat);

	// Transform clip coordinates into eye space
	// クリップ座標を視点空間に変換する
	glm::vec4 eyeCoords = invertedProjection * clipCoords;

	// Force direction vector (w = 0 for direction)
	// 方向ベクトルにするため w を 0 に設定する
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

// Converts screen coordinates into normalized device coordinates (-1 to 1)
// スクリーン座標を正規化デバイス座標（-1から1）に変換する
glm::vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY)
{
	// Convert X coordinate
	// X座標を変換する
	float x = (2.0f * mouseX) / camera.width - 1.0f;

	// Convert Y coordinate (invert because screen Y is top-down)
	// Y座標を変換する（スクリーン座標は上から下方向のため反転する）
	float y = 1.0f - (2.0f * mouseY) / camera.height;

	return glm::vec2(x, y);
}