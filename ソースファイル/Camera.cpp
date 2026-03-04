#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	cameraPosition = position;

}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Set view and projection matrices as Identity matrices
	// ビュー行列と投影行列を単位行列に設定する
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::mat4(1.0f);

	/* Create a matrix that transforms coordinates in a way that it looks at a target vector. 
		In this case the target vector is 1 magnitude unit away from the direction the camera is facing.
	*  Arguments:
	*	1. The position of the Camera
	*	2. The position of the target that the camera is looking at
	*		- In this case the target is the position of the camera + the normalized direction the camera is facing.
	*	3. The position's y axis																						*/
	/* ターゲットベクトルを見るように座標を変換する行列を作成する。
		この場合、ターゲットベクトルはカメラが向いている方向から1単位離れた位置にある。
	*  引数:
	*    1. カメラの位置
	*    2. カメラが向いているターゲットの位置
	*        - この場合、ターゲットはカメラの位置 + カメラの向きを正規化した方向となる。
	*    3. 位置の y 軸                                                                                        */
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	/* Create a large frustum that defines the visible space, anything outside the frustum will be clipped.
   arguments:
	1. FOV (field of view) value. Realistic view sets it to 45 degrees
	2. Aspect ratio, dividing the viewport's width from the height
	3. Near plane of the frustum, anything in front of this plane gets clipped
	4. Far plane of the frustum, anything passed this plane gets clipped										*/
	/*
	*  可視空間を定義する大きな視錐台を作成します。視錐台の外側にあるものはすべてクリップされます。
		引数:
	1. FOV（視野角）値。リアルな視界には45度を設定します。
    2. アスペクト比。ビューポートの幅を高さで割った値です。
    3. 視錐台の近面。この面より手前のオブジェクトはクリップされます。
    4. 視錐台の遠面。この面より奥のオブジェクトはクリップされます。
	*/
	projectionMatrix = glm::perspective(glm::radians(FOVdeg), ((float)width / (float)height), nearPlane, farPlane);

	// Sets new camera matrix
	// 新しいカメラ行列を設定します
	cameraMatrix = projectionMatrix * viewMatrix;

}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Passes the Camera matrix to the shader's uniform variable (Exports the camera matrix)
	// カメラ行列をシェーダーのユニフォーム変数に渡す（カメラ行列をエクスポートする）
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	// Handle keyboard movement (WASD)
	// キーボード入力（WASD）による移動処理

	// Move forward
	// 前進
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront;

	// Move backward
	// 後退
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront;

	// Move left (perpendicular to front and up)
	// 左移動（前方向と上方向の外積）
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition  += glm::normalize(glm::cross(cameraUp, cameraFront)) * cameraSpeed;

	// Move right
	// 右移動
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraUp, cameraFront)) * cameraSpeed;

	// Handle mouse rotation when right-click is pressed
	// 右クリック中のマウス回転処理
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Hide cursor while rotating camera
		// カメラ回転中はカーソルを非表示にする
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevent sudden jump on first click
		// 最初のクリック時に急な回転が起きないようにする
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Calculate rotation offsets based on mouse movement
		// マウス移動量から回転量を計算する
		float rotX = mouseSensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = mouseSensitivity * (float)(mouseX - (height / 2)) / height;

		// Calculate right vector (perpendicular axis)
		// 右方向ベクトルを計算する（回転軸）
		glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

		// Create quaternion for pitch (X-axis rotation)
		// ピッチ回転用のクォータニオンを作成（X軸回転）
		glm::quat pitchQuat = glm::angleAxis(glm::radians(-rotX), right);

		// Create quaternion for yaw (Y-axis rotation)
		// ヨー回転用のクォータニオンを作成（Y軸回転）
		glm::quat yawQuat = glm::angleAxis(glm::radians(-rotY), cameraUp);

		// Combine pitch and yaw rotations
		// ピッチとヨーの回転を合成する
		glm::quat orientation = yawQuat * pitchQuat;

		// Apply rotation to camera front direction
		// 回転をカメラの前方向ベクトルに適用する
		cameraFront = glm::normalize(orientation * cameraFront);

		// Reset cursor to center
		// カーソルを画面中央に戻す
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	// Restore cursor when right-click is released
	// 右クリックを離したらカーソルを元に戻す
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}

// Getter for projection matrix
// 投影行列を取得する
glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

// Getter for view matrix
// ビュー行列を取得する
glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

