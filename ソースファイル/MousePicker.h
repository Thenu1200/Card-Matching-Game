#ifndef MOUSE_PICKER_CLASS_H
#define MOUSE_PICKER_CLASS_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Transform.h"

// MousePicker class responsible for generating a ray from the mouse position
// マウス位置からレイを生成するMousePickerクラス
class MousePicker
{
public:

	// Constructor that takes a reference to the camera
	// カメラへの参照を受け取るコンストラクタ
	MousePicker(Camera& cam);

	// Returns the current world-space ray
	// 現在のワールド空間レイを返す
	glm::vec3 getCurrentRay();

	// Updates the ray based on the current mouse position
	// 現在のマウス位置に基づいてレイを更新する
	void Update(GLFWwindow* window);

private:

	// Current ray direction in world space
	// ワールド空間での現在のレイ方向
	glm::vec3 currentRay;

	// Projection matrix
	// 射影行列
	glm::mat4 projMat;

	// View matrix
	// ビュー行列
	glm::mat4 viewMat;

	// Reference to the camera
	// カメラへの参照
	Camera& camera;

	// Calculates a world-space ray from mouse coordinates
	// マウス座標からワールド空間レイを計算する
	glm::vec3 calculateMouseRay(double mouseX, double mouseY);

	// Converts eye-space coordinates into world-space direction
	// 視点空間座標をワールド空間の方向ベクトルに変換する
	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

	// Converts clip-space coordinates into eye-space
	// クリップ空間座標を視点空間に変換する
	glm::vec4 toEyeCoords(glm::vec4 clipCoords);

	// Converts screen coordinates into normalized device coordinates
	// スクリーン座標を正規化デバイス座標に変換する
	glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY);

};

#endif