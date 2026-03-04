#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

#include "shaderClass.h"

// Camera class definition
// カメラクラスの定義
class Camera
{
	public:
		// Create a camera that is parallel to the x and y axis and facing the -z axis
		// Position of the camera in the world space
		// ワールド空間におけるカメラの位置
		glm::vec3 cameraPosition; 

		// Forward direction vector of the camera
		// Default direction slightly downward toward -Z
		// カメラの前方向ベクトル
		// デフォルトでは -Z 方向（やや下向き）
		glm::vec3 cameraFront = glm::vec3(0.0f, -1.0 / sqrt(2), -1.0f / sqrt(2));

		// Up direction of the camera (world up)
		// カメラの上方向ベクトル（ワールドの上方向）
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// Projection matrix (perspective)
		// 投影行列（透視投影）
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		// Combined camera matrix (projection * view)
		// カメラ行列（投影行列 × ビュー行列）
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		// View matrix
		// ビュー行列
		glm::mat4 viewMatrix = glm::mat4(1.0f);

		// Window dimensions
		// ウィンドウの幅と高さ
		int width;
		int height;

		// Prevent sudden camera jump on first right-click
		// 最初の右クリック時にカメラが急に動くのを防ぐ
		bool firstClick = true;

		// Movement speed of the camera
		// カメラの移動速度
		float cameraSpeed = 0.05f;

		// Mouse sensitivity for rotation
		// マウス回転の感度
		float mouseSensitivity = 40.0f;

		// Constructor
		// Initializes camera with viewport size and position
		// コンストラクタ
		// ビューポートサイズと初期位置を設定する
		Camera(int width, int height, glm::vec3 position);

		// Updates projection and view matrices
		// 投影行列とビュー行列を更新する
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

		// Sends the camera matrix to the shader as a uniform
		// カメラ行列をシェーダーの uniform 変数に送信する
		void Matrix(Shader& shader, const char* uniform);

		// Handles keyboard and mouse input for movement and rotation
		// キーボードおよびマウス入力を処理する
		void Inputs(GLFWwindow* window);


		// Returns projection matrix
		// 投影行列を取得する
		glm::mat4 getProjectionMatrix();

		// Returns projection matrix
		// 投影行列を取得する
		glm::mat4 getViewMatrix();
};

#endif